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

#ifndef iass_scene_mngr_hh
#define iass_scene_mngr_hh

#include <iostream>
#include "iass_ipctunnel.hh"
#include "iass_ipc_helpers.hh"
#include "iass_scene.hh"
#include "iass_scene_stats.hh"
#include "iass_entity_factory.hh"

#define SCENE_MNGR_IPC_RESOLVER_LOOP_FREQUENCY 100 /*Hz*/
#define SCENE_MNGR_IPC_RESOLVER_IPC_DATA_BUFFER_LEN 1000

class iass_scene_mngr
{
/* functions */
public:
	iass_scene_mngr(unsigned int id);
	virtual ~iass_scene_mngr();

	iass_scene_stats* get_stats(void);


	virtual void set_name(iass_cstring* name) = 0;
	virtual iass_tunnel_idl* subscribe_scene_master(void) = 0;
	virtual void unsubscribe_scene_master(void) = 0;


protected:
	void run_separate_thread_ipc_resolver(void);  
	bool ipc_resolver_is_running(void);
	void ipc_resolver_stop(void);
	/* args is of type ((void*) iass_scene_mngr*) */
	static void* internal_run_separate_thread_ipc_resolver(void* args);

	void scene_mngr_resolve_ipc_request(iass_ipcblock* data);

private:
	void ipc_resolver_loop(void);  
	virtual void implementor_resolve_ipc_request(iass_ipcblock* data) = 0;


/* members */
protected:
	iass_entity_factory* entity_factory;
	iass_scene* _scene;
	iass_scene_stats scene_stats;

	ACE_Thread_Mutex mutex;
	unsigned int subscribed_scene_masters;


	/* incoming queue ipc_resolver */
	iass_ipctunnel _incoming_tunnel;
	ACE_Thread_Mutex ipc_resolver_mutex;
	bool _ipc_resolver_is_running;
	bool ipc_resolver_keep_running;
};

#endif /*iass_scene_mngr_hh*/

