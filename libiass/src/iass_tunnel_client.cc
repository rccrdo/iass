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

#include "iass_tunnel_client.hh"

iass_tunnel_client::iass_tunnel_client(iass_tunnel_idl* dest) {
	assert(dest);

	tunnel_client_data.mutex.acquire();
	tunnel_client_data.is_running = false;
	tunnel_client_data.keep_running = false;
	tunnel_client_data.tunnel = dest;
	tunnel_client_data.mutex.release();

	/* enter loop */
	run_separate_thread();
}

void iass_tunnel_client::push(char* buf, unsigned int len) {
	assert(buf);
	assert(len);

	tunnel_client_data.mutex.acquire();  
	tunnel_client_data.incoming_queue.push(buf, len);
	tunnel_client_data.mutex.release();
}


void iass_tunnel_client::run_separate_thread(void) {
	tunnel_client_data.mutex.acquire();
	if (tunnel_client_data.is_running) {
		std::cout << "warning iass_tunnel_client::run_separate_thread(), already running\n";
		tunnel_client_data.mutex.release();
		return;
	}

	/* if we get here tunnel isn't running */
	tunnel_client_data.keep_running = true;
	/* release mutex as it's used also in internal_run_separate_thread */
	tunnel_client_data.mutex.release();

	/* spawn thread */
	ACE_Thread_Manager::instance()->spawn (iass_tunnel_client::internal_run_separate_thread,
					       (void*) &tunnel_client_data,
					       THR_DETACHED | THR_SCOPE_SYSTEM ); 

	/* wait until tunnel is effectively running */
	while (true) {
		tunnel_client_data.mutex.acquire();
		if (tunnel_client_data.is_running) {
			tunnel_client_data.mutex.release();
			break;
		}
		tunnel_client_data.mutex.release();
		/* retry in one usec */
		usleep(1);
	}
}


bool iass_tunnel_client::is_running(void) {
	tunnel_client_data.mutex.acquire();
	bool return_value = tunnel_client_data.is_running;
	tunnel_client_data.mutex.release();
	return return_value;
}


void iass_tunnel_client::stop(void) {
	tunnel_client_data.mutex.acquire();
	tunnel_client_data.keep_running = false;
	tunnel_client_data.mutex.release();
}


void* iass_tunnel_client::internal_run_separate_thread(void* args) {
	iass_tunnel_client_data* tunnel_client_data;
	iass_loop_sleeper loop_sleeper(TUNNEL_CLIENT_MAINLOOP_FREQUENCY);
	char packets_buffer[TUNNEL_CLIENT_BUFFERS_LEN];

	assert(args);
	tunnel_client_data = (iass_tunnel_client_data*) args;
	tunnel_client_data->mutex.acquire();
	tunnel_client_data->is_running = true;
	tunnel_client_data->mutex.release();

	/* loop unti tunnel_client_data->keep_running == false
	 * ! condition is tested after acquiring the mutex */
	while (true) {
		tunnel_client_data->mutex.acquire();

		/* search a valid iass_packet in the incoming_queue */
		while (tunnel_client_data->incoming_queue.size()) {
			unsigned int popped_len;

			/* try to pop TUNNEL_CLIENT_BUFFERS_LEN bytes from the queue */
			popped_len = tunnel_client_data->incoming_queue.top(packets_buffer, TUNNEL_CLIENT_BUFFERS_LEN);

			iass_corba_raw_stream temp(popped_len);
			memcpy(temp.get_buffer(), packets_buffer, popped_len);
			try {
				tunnel_client_data->tunnel->push(temp);
			}
			catch (CORBA::Exception &) {
				std::cout << "warning iass_tunnel_client::internal_run_separate_thread(void* args), exception pushing to the tunnel\n";
			}

			/* pop pushed chars */
			tunnel_client_data->incoming_queue.pop(popped_len);
		}

		/* check the keep_running flag */
		if (!tunnel_client_data->keep_running) {
			tunnel_client_data->mutex.release();
			break;
		}

		tunnel_client_data->mutex.release();
		/* try to keep this loop running at TUNNEL_CLIENT_MAINLOOP_FREQUENCY Hz */
		loop_sleeper.sleep();
	}

	/* clear the is_running flag*/
	tunnel_client_data->mutex.acquire();
	tunnel_client_data->is_running = false;
	tunnel_client_data->mutex.release();

	/* kill thread */
	std::cout << "warning iass_tunnel_client::internal_run_separate_thread(void* args), exiting.. should join thread (?)\n";
	return 0;
}

