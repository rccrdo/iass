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

#ifndef iass_scenes_server_hh
#define iass_scenes_server_hh

#include <string.h>
#include <iostream>
#include <vector>
#include <string>
#include "iass_corba_orb_mngr.hh"
#include "idls/iass_scenes_server_idlS.h"
#include "iass_server_scene_mngr.hh"
#include <ace/Thread_Mutex.h>


class iass_scenes_server : public POA_iass_scenes_server_idl {

/* functions */
public:
	/* get singletone instance */
	static iass_scenes_server* instance();

	/* returns a NULL terminated vector of iass_scene_stats* 
	 * or simply NULL when there is no scene at all 
	 * ! returned array must be freed by caller
	 * ! contents of array must not be freed */
	iass_scene_stats** get_scene_stats(void);

private:
	iass_scenes_server ();
	~iass_scenes_server ();

	/* acquire the access_mutex before calling these ! */
	unsigned int get_free_scene_slot(void);
	bool scene_exists(unsigned int id);


/* CORBA rpc methods */
public:
	iass_tunnel_idl* new_scene() throw ();

	iass_tunnel_idl* subscribe_scene_master(CORBA::UShort scene_id, iass_corba_raw_stream& pwd) throw () {return NULL;}
	void unsubscribe_scene_master(CORBA::UShort scene_id, const iass_corba_raw_stream& pwd) throw () {}


	CORBA::Boolean subscribe_spectator(CORBA::UShort _scene_id, const iass_corba_raw_stream& _ior) throw ();
	void unsubscribe_spectator(CORBA::UShort _scene_id, const iass_corba_raw_stream& _ior) throw ();


/* members */
private:
	/* scenes hosted at this server */
	std::vector<iass_server_scene_mngr*> scene_mngrs;
	ACE_Thread_Mutex access_mutex;

	/* singletone stuff */
	static iass_scenes_server* st_instance;
	static ACE_Thread_Mutex st_mutex;
};

#endif /*iass_scenes_server_HH*/
