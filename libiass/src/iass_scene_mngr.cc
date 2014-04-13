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

#include "iass_scene_mngr.hh"


iass_scene_mngr::iass_scene_mngr(unsigned int id)
{
	subscribed_scene_masters = 0;
	_ipc_resolver_is_running = false;
	ipc_resolver_keep_running = false;

	/* defaults */
	scene_stats.name = "Untitled Scene";
	scene_stats.id = id;
}


iass_scene_mngr::~iass_scene_mngr()
{
	mutex.acquire();
	if (subscribed_scene_masters) {
		_incoming_tunnel.stop();
	}

	if (_scene) delete _scene;
	mutex.release();
}


iass_scene_stats* iass_scene_mngr::get_stats(void) {
	return &scene_stats;
}


void iass_scene_mngr::run_separate_thread_ipc_resolver(void) {
	assert(_scene);

	ipc_resolver_mutex.acquire();
	if (_ipc_resolver_is_running) {
		std::cout << "warning iass_scene_mngr::run_separate_thread_ipc_resolver(void), already running\n";
		ipc_resolver_mutex.release();
		return;
	}

	/* if we get here the ipc_resolver_loop isn't running */
	ipc_resolver_keep_running = true;
	/* release mutex as it's used also in internal_run_separate_thread_ipc_resolver */
	ipc_resolver_mutex.release();

	/* spawn thread */
	ACE_Thread_Manager::instance()->spawn (iass_scene_mngr::internal_run_separate_thread_ipc_resolver,
					       (void*) this,
					       THR_DETACHED | THR_SCOPE_SYSTEM ); 

	/* wait until ipc_resolver is effectively running */
	while (true) {
		ipc_resolver_mutex.acquire();
		if (_ipc_resolver_is_running) {
			ipc_resolver_mutex.release();
			break;
		}
		ipc_resolver_mutex.release();
		/* retry in one usec */
		usleep(1);
	}
}


bool iass_scene_mngr::ipc_resolver_is_running(void) {
	ipc_resolver_mutex.acquire();
	bool return_value = _ipc_resolver_is_running;
	ipc_resolver_mutex.release();
	return return_value;
}


void iass_scene_mngr::ipc_resolver_stop(void) {
	ipc_resolver_mutex.acquire();
	ipc_resolver_keep_running = false;
	ipc_resolver_mutex.release();
}


void* iass_scene_mngr::internal_run_separate_thread_ipc_resolver(void* args) {
	iass_scene_mngr* this_scene_mngr;

	assert(args);
	this_scene_mngr = (iass_scene_mngr*) args;
	this_scene_mngr->ipc_resolver_loop();
  
	/* kill thread */
	std::cout << "warning iass_scene_mngr::internal_run_separate_thread_ipc_resolver(void* args), exiting.. should join thread (?)\n";
	return 0;
}


void iass_scene_mngr::ipc_resolver_loop(void) {
	iass_loop_sleeper loop_sleeper(SCENE_MNGR_IPC_RESOLVER_LOOP_FREQUENCY);
	iass_ipcblock* ipc_data_buffer[SCENE_MNGR_IPC_RESOLVER_IPC_DATA_BUFFER_LEN];

	ipc_resolver_mutex.acquire();
	_ipc_resolver_is_running = true;
	ipc_resolver_mutex.release();

	/* main ipc_resolver_loop */
	while (true) {
		unsigned int pulled_len;

		pulled_len = _incoming_tunnel.pull (ipc_data_buffer, SCENE_MNGR_IPC_RESOLVER_IPC_DATA_BUFFER_LEN);
		for (unsigned int i = 0; i < pulled_len; i++) {
			implementor_resolve_ipc_request (ipc_data_buffer[i]);
			delete ipc_data_buffer[i];
		}

		/* check the keep_running flag */
		if (!ipc_resolver_keep_running) break;

		/* sleep a bit */
		loop_sleeper.sleep();
	}

	/* exiting.. clear the is_running flag */
	ipc_resolver_mutex.acquire();
	_ipc_resolver_is_running = false;
	ipc_resolver_mutex.release();
}

void iass_scene_mngr::scene_mngr_resolve_ipc_request(iass_ipcblock* data) {
	uint8_t content_t;
	uint16_t request_t;
	unsigned int int8_vector_size;
	unsigned int int16_vector_size;
	uint8_t* uint8_array;
	uint16_t* uint16_array;

	assert(data);

	int8_vector_size = data->int8_vector_size();
	int16_vector_size = data->int16_vector_size();

	if (int8_vector_size == 0 ) {
		std::cout << "warning iass_scene_mngr::scene_mngr_resolve_ipc_request(iass_ipcblock* data), int8_vector has size zero\n";
		return;
	}
	if (int16_vector_size == 0 ) {
		std::cout << "warning iass_scene_mngr::scene_mngr_resolve_ipc_request(iass_ipcblock* data), int16_vector has size zero\n";
		return;
	}

	uint8_array = (uint8_t*) data->int8_vector_ptr();
	uint16_array = (uint16_t*) data->int16_vector_ptr();
	content_t = uint8_array[0];
  
	if (content_t!= (uint8_t) ipc_block_content__scene_mngr_request) {
		std::cout << "warning iass_scene_mngr::scene_mngr_resolve_ipc_request(iass_ipcblock* data), content_t is " << (int) content_t 
			  <<", not suitable for the iass_scene_mngr\n";
		return;
	}

	request_t = uint16_array[0];
	switch (request_t){
	case (scene_mngr_request__set_name) : {
		unsigned int cstring_vector_size;

		cstring_vector_size = data->cstrings_vector_size();
		if (cstring_vector_size!=1) {
			std::cout << "warning iass_scene_mngr::scene_mngr_resolve_ipc_request(iass_ipcblock* data), __set_name, cstrings_vector"
				  << " size is invalid " << cstring_vector_size << "\n";
			break;
		}
		iass_cstring** cstring_array = data->cstrings_vector_ptr();
		iass_cstring* name = cstring_array[0];
 
		/* finally set the name */
		set_name(name);
		break;
	}
    
	default : {
		std::cout << "warning iass_scene_mngr::scene_mngr_resolve_ipc_request(iass_ipcblock* data), request_t is " << request_t 
			  <<", not a valid request for the iass_scene_mngr\n";
	}
	}
}


