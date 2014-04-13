/*
 * Copyright (c) 2006-2007 Riccardo Lucchese, riccardo.lucchese at gmail.com
 * 
 * This software is provided 'as-is', without any express or implied
 * warranty. In no event will the authors be held liable for any damages
 * arising from the use of this software.
 * 
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 * 
 *    1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 * 
 *    2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 
 *    3. This notice may not be removed or altered from any source
 *    distribution.
 */

#include "iass_ipctunnel_client.hh"

iass_ipctunnel_client::iass_ipctunnel_client(iass_tunnel_idl* dest) {
	assert(dest);
	mutex.acquire();
	_is_running = false;
	keep_running = false;
	tunnel = dest;
	mutex.release();
}


iass_ipctunnel_client::~iass_ipctunnel_client() {
	stop();

	/* whait the internal thread to really stop */
	while(true) {
		mutex.acquire();
		if (!_is_running) {
			break;
		}
		mutex.release();
		usleep(1);
	}
	/* release tunnel corba-obj */
	CORBA::release(tunnel);
	mutex.release();
}


void iass_ipctunnel_client::push(iass_ipcblock** buf, unsigned int len) {
	assert(buf);
	assert(len);

#ifdef debug_iass_ipctunnel_client
	/* check for NULL ptrs */
	for (unsigned int i=0; i < len; i++) assert(buf[i]);
#endif 

	mutex.acquire();
	incoming_queue.push(buf, len);

	/* check if we are able to follow the rythm :) 
	 * ! this shouldn't ever be triggered */
	if (incoming_queue.size() > IPC_TUNNEL_CLIENT_INCOMING_QUEUE_MAX_LEN ) {
		std::cout << "warning iass_ipctunnel_client::push(iass_ipcblock** buf, unsigned int len), unable to follow the streaming rythm\n";
		/* discard 1/5 of the queue */
		iass_memblock<iass_ipcblock*> temp(incoming_queue.size() / 5);
		incoming_queue.top(temp.buf, temp.reserved);
		incoming_queue.pop(temp.reserved);
		for (unsigned int i=0; i< temp.reserved; i++) if (temp.buf[i]) delete temp.buf[i];
	}
	mutex.release();
}


void iass_ipctunnel_client::run_separate_thread(void) {
	mutex.acquire();
	if (_is_running) {
		std::cout << "warning iass_ipctunnel_client::run_separate_thread(), already running\n";
		mutex.release();
		return;
	}

	/* if we get here tunnel isn't running */
	keep_running = true;
	/* release mutex as it's used also in internal_run_separate_thread */
	mutex.release();

	/* spawn thread */
	ACE_Thread_Manager::instance()->spawn (iass_ipctunnel_client::internal_run_separate_thread,
					       (void*) this,
					       THR_DETACHED | THR_SCOPE_SYSTEM ); 

	/* wait until tunnel is effectively running */
	while (true) {
		mutex.acquire();
		if (_is_running) {
			mutex.release();
			break;
		}
		mutex.release();
		/* retry in one usec */
		usleep(1);
	}
}


bool iass_ipctunnel_client::is_running(void) {
	mutex.acquire();
	bool return_value = _is_running;
	mutex.release();
	return return_value;
}


void iass_ipctunnel_client::stop(void) {
	mutex.acquire();
	keep_running = false;
	mutex.release();
}


void* iass_ipctunnel_client::internal_run_separate_thread(void* args) {
	iass_loop_sleeper loop_sleeper(IPC_TUNNEL_CLIENT_MAINLOOP_FREQUENCY);
	iass_memblock<iass_ipcblock*> dequeued_ipc_blocks(IPC_TUNNEL_CLIENT__BUFFERS_LEN);
	iass_cstring temp_content_buf(IPC_TUNNEL_CLIENT__BUFFERS_LEN);
	iass_cstring serialized_packets_buf(IPC_TUNNEL_CLIENT__BUFFERS_LEN);
	iass_ipctunnel_client* obj_data;
	iass_corba_raw_stream corba_stream;

	assert(args);
	obj_data = (iass_ipctunnel_client*)args;
	obj_data->mutex.acquire();
	obj_data->_is_running = true;
	obj_data->mutex.release();

	/* loop until keep_running is false
	 * ! condition is tested after acquiring the mutex */
	while (true) {
		obj_data->mutex.acquire();
		serialized_packets_buf.len = 0;

		/* process incoming queue only if it's not empty */
		if (obj_data->incoming_queue.size()) {
			// try to copy IPC_TUNNEL_CLIENT__BUFFERS_LEN elements from the queue 
			dequeued_ipc_blocks.len = obj_data->incoming_queue.top(dequeued_ipc_blocks.buf,
									       dequeued_ipc_blocks.reserved);

			for (unsigned int i=0; i < dequeued_ipc_blocks.len; i++) {
				unsigned int current_packet_len;
				/* no nedd to check for NULL ptrs: it's done in the push method */

				/* check that the length of the serialized data is not more than the reserved space */
				assert(dequeued_ipc_blocks.buf[i]->serialized_len() < temp_content_buf.reserved);
				temp_content_buf.len = dequeued_ipc_blocks.buf[i]->serialize(temp_content_buf.buf);

				/*check current packet serialized length */
				assert(iass_packet_serialized_max_len(temp_content_buf.len) < (IPC_TUNNEL_CLIENT__BUFFERS_LEN - serialized_packets_buf.len));
        
				current_packet_len = iass_packet_serialize(serialized_packets_buf.buf + serialized_packets_buf.len,
									   temp_content_buf.buf, temp_content_buf.len);
				serialized_packets_buf.len += current_packet_len;

				/* free memory */
				delete dequeued_ipc_blocks.buf[i];

				/* if this is not the last cycle of the loop check if we still have space on buffers */
				if ( (i!=(dequeued_ipc_blocks.len - 1) ) and 
				     (dequeued_ipc_blocks.buf[i+1]->serialized_len() > (IPC_TUNNEL_CLIENT__BUFFERS_LEN - serialized_packets_buf.len)) ) {
					/* break the serializing loop and set len so that unserialized struc won't be popped */
					dequeued_ipc_blocks.len = i +1;
					break;
				}
			}
			/* pop serialized structs from the incoming queue */
			obj_data->incoming_queue.pop(dequeued_ipc_blocks.len);

			if (serialized_packets_buf.len) {
				corba_stream.length(serialized_packets_buf.len);
				memcpy(corba_stream.get_buffer(), serialized_packets_buf.buf, serialized_packets_buf.len);
				try {
					obj_data->tunnel->push(corba_stream);
				}
				catch (CORBA::Exception &) {
					std::cout << "warning iass_ipctunnel_client::internal_run_separate_thread(void* args), exception on push\n";
				}
			}

		}
		/* check the keep_running flag */
		if (!obj_data->keep_running) {
			obj_data->mutex.release();
			break;
		}

		obj_data->mutex.release();
		/* try to keep this loop running at IPC_TUNNEL_CLIENT_MAINLOOP_FREQUENCY Hz */
		loop_sleeper.sleep();
	}

	/* clear the _is_running flag*/
	obj_data->mutex.acquire();
	obj_data->_is_running = false;
	obj_data->mutex.release();

/* kill thread */
	std::cout << "warning iass_ipctunnel_client::internal_run_separate_thread(void* args), exiting.. should join thread (?)\n";
	return 0;
}




