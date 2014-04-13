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

#ifndef iass_scene_spectators_mngr_hh
#define iass_scene_spectators_mngr_hh

#include <iostream>
#include <vector>
#include "iass_libdebug.hh"
#include "iass_ipcblock.hh"
#include "iass_packet.hh"
#include "iass_loop_sleeper.hh"
#include "iass_queue.hh"
#include "idls/iass_tunnel_idlC.h"
#include <ace/Thread_Mutex.h>
#include <ace/Thread_Manager.h>

#define SCENE_SPECTATOR_MNGR_PUSH_TO_QUEUE_BUFFER 100000 /* bytes */
#define SCENE_SPECTATOR_MNGR_BROADCAST_LOOP_FREQUENCY 100 /*hz*/

class iass_scene_spectators_mngr
{
/* functions */
public:
	~iass_scene_spectators_mngr();
	unsigned int subscribe(iass_tunnel_idl* spectator_tunnel);
	void unsubscribe(std::string& ior);
  
	void broadcast(iass_ipcblock* ipc_data);

private:
	unsigned int spectators_find_free_slot(void);
	bool spectator_id_is_valid(unsigned int spectator_id);

	inline void push_to_output_queue(iass_ipcblock* ipc_data);

	/* spectator broadcast_loop */
	void run_separate_thread_broadcast_loop(void);  
	bool broadcast_loop_is_running(void);
	void broadcast_loop_stop(void);
	/* args is of type ((void*) iass_scene_mngr*) */
	static void* internal_run_separate_thread_broadcast_loop(void* args);
	void broadcast_loop(void);


/* members */
protected:
	ACE_Thread_Mutex spectators_mutex;
	std::vector <iass_tunnel_idl*> spectators;

	/* output queue */
	/* this buffer is used only by the push_to_output_queue method */
	char push_buffer[SCENE_SPECTATOR_MNGR_PUSH_TO_QUEUE_BUFFER];
	char packet_buffer[SCENE_SPECTATOR_MNGR_PUSH_TO_QUEUE_BUFFER];
	ACE_Thread_Mutex output_queue_mutex;  
	/* only valid entire packet must be pushed to this queue */
	iass_queue<char> output_queue;


	/* broadcaster stuff */
	ACE_Thread_Mutex broadcast_loop_mutex;
	bool _broadcast_loop_is_running;
	bool broadcast_loop_keep_running;
};

#endif /*iass_scene_spectators_mngr_hh*/

