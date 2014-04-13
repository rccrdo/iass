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

#include "iass_ipctunnel.hh"

iass_ipctunnel::iass_ipctunnel() {
	tunnel_data.mutex.acquire();
	tunnel_data.is_running = false;
	tunnel_data.keep_running = false;
	tunnel_data.mutex.release();

	std::cout << "warning iass_ipctunnel(), who frees data passed from corba to the implementation of the push method ?\n";
}

unsigned int iass_ipctunnel::pull(iass_ipcblock** buf, unsigned int buf_len) {
	unsigned int copied_len;

	assert(buf);
	assert(buf_len);
	copied_len = tunnel_data.ipc_data_queue.top(buf, buf_len);
	if (copied_len) tunnel_data.ipc_data_queue.pop(copied_len);
	return copied_len;
}


void iass_ipctunnel::push(const iass_corba_raw_stream& stream) throw() {
	if (!stream.get_buffer()) {
		std::cout << "warning iass_ipctunnel::push(const iass_corba_raw_stream& stream), stream.get_buffer() is a NULL ptr\n";
		return;
	}
	if (!stream.length()) {
		std::cout << "warning iass_ipctunnel::push(const iass_corba_raw_stream& stream), stream.length() is a NULL ptr\n";
		return;
	}


	/* queue received stream */
	tunnel_data.mutex.acquire();
	tunnel_data.incoming_queue.push((const char*)stream.get_buffer(), stream.length() );

	if (!tunnel_data.is_running)
		std::cout << "warning iass_ipctunnel::push(const iass_corba_raw_stream& stream), received stream while not running\n";

	/* check the incoming_queue isn't becoming too big */
	if (tunnel_data.incoming_queue.size() > IPC_TUNNEL_INCOMING_QUEUE_MAX_LEN) {
		/* if this happens there must be some problem around or cpu charge is too high ... */
		std::cout << "warning iass_ipctunnel::push(const iass_corba_raw_stream& stream), incoming_queue.size() is "
			  << tunnel_data.incoming_queue.size() << ", max allowed is "<< IPC_TUNNEL_INCOMING_QUEUE_MAX_LEN << "\n";
		tunnel_data.incoming_queue.pop(IPC_TUNNEL_BUFFERS_LEN);
	}
	tunnel_data.mutex.release();
}


void iass_ipctunnel::run_separate_thread(void) {
	tunnel_data.mutex.acquire();
	if (tunnel_data.is_running) {
		std::cout << "warning iass_ipctunnel::run_separate_thread(), already running\n";
		tunnel_data.mutex.release();
		return;
	}

	/* if we get here tunnel isn't running */
	tunnel_data.keep_running = true;
	/* release mutex as it's used also in internal_run_separate_thread */
	tunnel_data.mutex.release();

	/* spawn thread */
	ACE_Thread_Manager::instance()->spawn (iass_ipctunnel::internal_run_separate_thread,
					       (void*) &tunnel_data,
					       THR_DETACHED | THR_SCOPE_SYSTEM ); 

	/* wait until tunnel is effectively running */
	while (true) {
		tunnel_data.mutex.acquire();
		if (tunnel_data.is_running) {
			tunnel_data.mutex.release();
			break;
		}
		tunnel_data.mutex.release();
		/* retry in one usec */
		usleep(1);
	}
}


bool iass_ipctunnel::is_running(void) {
	tunnel_data.mutex.acquire();
	bool return_value = tunnel_data.is_running;
	tunnel_data.mutex.release();
	return return_value;
}


void iass_ipctunnel::stop(void) {
	tunnel_data.mutex.acquire();
	tunnel_data.keep_running = false;
	tunnel_data.mutex.release();
}


void* iass_ipctunnel::internal_run_separate_thread(void* args) {
	iass_ipctunnel_data* tunnel_data;
	iass_loop_sleeper loop_sleeper(IPC_TUNNEL_MAINLOOP_FREQUENCY);
	char popped_buffer[IPC_TUNNEL_BUFFERS_LEN];
	char content_buffer[IPC_TUNNEL_BUFFERS_LEN];

	assert(args);
	tunnel_data = (iass_ipctunnel_data*) args;
	tunnel_data->mutex.acquire();
	tunnel_data->is_running = true;
	tunnel_data->mutex.release();

	/* loop unti tunnel_data->keep_running == false
	 * ! condition is tested after acquireing the mutex */
	while (true) {
		tunnel_data->mutex.acquire();

		/* search a valid iass_packet in the incoming_queue */
		while (tunnel_data->incoming_queue.size() > IASS_PACKET_MIN_LEN) {
			unsigned int cur = 0;
			unsigned int popped_len;

			/* try to pop IPC_TUNNEL_BUFFERS_LEN bytes from the queue */
			popped_len = tunnel_data->incoming_queue.top(popped_buffer, IPC_TUNNEL_BUFFERS_LEN);
			assert(popped_len >= IASS_PACKET_MIN_LEN);

			/* scan the popped_len chars in the popped_buffer for a packet */
			while (cur <= (popped_len - IASS_PACKET_MIN_LEN )) {
				unsigned int packet_begin_cur;
				unsigned int packet_end_cur;
				unsigned int discardable_bytes;
				discardable_bytes = iass_packet_search_stream(popped_buffer + cur,
									      (popped_len - cur),
									      packet_begin_cur,
									      packet_end_cur );
				/* if packet_end_cur!=0 then a packet was found in the stream */
				if (packet_end_cur) {
					unsigned int content_len;
					iass_packet_info info;
					iass_ipcblock* ipc_data;

					assert( (packet_end_cur - packet_begin_cur +1) < IPC_TUNNEL_BUFFERS_LEN );
					content_len = iass_packet_unserialize(info, content_buffer, popped_buffer + cur + packet_begin_cur);
					ipc_data = new iass_ipcblock;
					if (ipc_data->unserialize(content_buffer, content_len)) tunnel_data->ipc_data_queue.push(&ipc_data, 1);
					else delete ipc_data;
				}
				cur += discardable_bytes;
			}
			/* pop discardable chars in the stream */
			tunnel_data->incoming_queue.pop(cur);
		}

		/* check the keep_running flag */
		if (!tunnel_data->keep_running) {
			tunnel_data->mutex.release();
			break;
		}

		tunnel_data->mutex.release();
		/* try to keep this loop running at IPC_TUNNEL_MAINLOOP_FREQUENCY Hz */
		loop_sleeper.sleep();
	}

	/* clear the is_running flag*/
	tunnel_data->mutex.acquire();
	tunnel_data->is_running = false;
	tunnel_data->mutex.release();

/* kill thread */
	std::cout << "warning iass_ipctunnel::internal_run_separate_thread(void* args), exiting.. should join thread (?)\n";
	return 0;
}

