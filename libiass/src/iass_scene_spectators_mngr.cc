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

#include "iass_scene_spectators_mngr.hh"

iass_scene_spectators_mngr::~iass_scene_spectators_mngr() {
	_broadcast_loop_is_running = false;
	broadcast_loop_keep_running = false;
}


unsigned int iass_scene_spectators_mngr::subscribe(iass_tunnel_idl* spectator_tunnel) {
	unsigned int new_id;

	spectators_mutex.acquire();
	new_id =  spectators_find_free_slot();
	spectators[new_id] = spectator_tunnel;
	spectators_mutex.release();
	return new_id;
}


void iass_scene_spectators_mngr::unsubscribe(std::string& ior) {
//  unsigned int spectator_id;
	spectators_mutex.acquire();

	/* find the id using a map or something */
	std::cout << "iass_scene_spectators_mngr::unsubscribe_spectator(unsigned int spectator_id), doing nothing\n";

/*  std::cout << "iass_scene_spectators_mngr::unsubscribe_spectator(unsigned int spectator_id), delete or corba::release?\n";
    CORBA::release(spectators[spectator_id]);
    spectators[spectator_id] = NULL;*/
	spectators_mutex.release();
}


unsigned int iass_scene_spectators_mngr::spectators_find_free_slot(void) {
	for (unsigned int i = 0; i < spectators.size(); i++ ) 
	{ // we search for a free slot
		if (spectators[i] == NULL) return i;
	}
	// if we didn't found a free slot than we push a new space/slot in the vector
	spectators.push_back(NULL);
	return (spectators.size() -1 );
}


bool iass_scene_spectators_mngr::spectator_id_is_valid(unsigned int spectator_id) {
	if ( spectator_id >= spectators.size() ) return false;
	if (spectators[spectator_id] == NULL) return false;
	return true;
}


void iass_scene_spectators_mngr::broadcast(iass_ipcblock* ipc_data) {
	output_queue_mutex.acquire();
	push_to_output_queue(ipc_data);
	output_queue_mutex.release();
}


void iass_scene_spectators_mngr::push_to_output_queue(iass_ipcblock* ipc_data) {
	unsigned int content_len;
	unsigned int packet_len;

	/* fast path */
	if (ipc_data->serialized()) {
		assert (ipc_data->serialized()->len < SCENE_SPECTATOR_MNGR_PUSH_TO_QUEUE_BUFFER );
		packet_len = iass_packet_serialize(packet_buffer, (const char*)ipc_data->serialized()->buf, ipc_data->serialized()->len);
		output_queue.push(packet_buffer, packet_len);
		return;
	}

	/*slow path */
	assert (ipc_data->serialized_len() < SCENE_SPECTATOR_MNGR_PUSH_TO_QUEUE_BUFFER );
	content_len = ipc_data->serialize(push_buffer);
	packet_len = iass_packet_serialize(packet_buffer, push_buffer, content_len);
	output_queue.push(packet_buffer, packet_len);
}


void iass_scene_spectators_mngr::run_separate_thread_broadcast_loop(void) {

	broadcast_loop_mutex.acquire();
	if (_broadcast_loop_is_running) {
		std::cout << "warning iass_scene_spectators_mngr::run_separate_thread_broadcast_loop(void), already running\n";
		broadcast_loop_mutex.release();
		return;
	}

	/* if we get here the ipc_resolver_loop isn't running */
	broadcast_loop_keep_running = true;
	/* release mutex as it's used also in internal_run_separate_thread_ipc_resolver */
	broadcast_loop_mutex.release();

	/* spawn thread */
	ACE_Thread_Manager::instance()->spawn (iass_scene_spectators_mngr::internal_run_separate_thread_broadcast_loop,
					       (void*) this,
					       THR_DETACHED | THR_SCOPE_SYSTEM ); 

	/* wait until ipc_resolver is effectively running */
	while (true) {
		broadcast_loop_mutex.acquire();
		if (_broadcast_loop_is_running) {
			broadcast_loop_mutex.release();
			break;
		}
		broadcast_loop_mutex.release();
		/* retry in one usec */
		usleep(1);
	}
}


bool iass_scene_spectators_mngr::broadcast_loop_is_running(void) {
	broadcast_loop_mutex.acquire();
	bool return_value = _broadcast_loop_is_running;
	broadcast_loop_mutex.release();
	return return_value;
}


void iass_scene_spectators_mngr::broadcast_loop_stop(void) {
	broadcast_loop_mutex.acquire();
	broadcast_loop_keep_running = false;
	broadcast_loop_mutex.release();
}


void* iass_scene_spectators_mngr::internal_run_separate_thread_broadcast_loop(void* args) {
	iass_scene_spectators_mngr* this_scene_spectators_mngr;

	assert(args);
	this_scene_spectators_mngr = (iass_scene_spectators_mngr*) args;
	this_scene_spectators_mngr->broadcast_loop();
  
	/* kill thread */
	std::cout << "warning iass_scene_spectators_mngr::internal_run_separate_thread_broadcast_loop(void* args), exiting.. should join thread (?)\n";
	return 0;
}


void iass_scene_spectators_mngr::broadcast_loop(void) {
	iass_loop_sleeper loop_sleeper( 2 /*SCENE_SPECTATOR_MNGR_BROADCAST_LOOP_FREQUENCY*/);

	broadcast_loop_mutex.acquire();
	_broadcast_loop_is_running = true;
	broadcast_loop_mutex.release();

	/* main ipc_resolver_loop */
	while (true) {
		/* stop operations on the queue or buffers */
		spectators_mutex.acquire();
		output_queue_mutex.acquire();

		/* push stuff to the tunnel clients wich will push them to the buggers */
		std::cout << "warning iass_scene_spectators_mngr::broadcast_loop(void), doing nothing\n";

		spectators_mutex.release();
		output_queue_mutex.release();

		/* check the keep_running flag */
		if (!broadcast_loop_keep_running) break;

		/* sleep a bit */
		loop_sleeper.sleep();
	}

	/* exiting.. clear the is_running flag */
	broadcast_loop_mutex.acquire();
	_broadcast_loop_is_running = false;
	broadcast_loop_mutex.release();
}










