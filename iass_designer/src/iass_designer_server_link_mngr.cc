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

#include "iass_designer_server_link_mngr.hh"

iass_designer_server_link_mngr* iass_designer_server_link_mngr::st_instance = 0;
ACE_Thread_Mutex iass_designer_server_link_mngr::st_mutex;

iass_designer_server_link_mngr* iass_designer_server_link_mngr::instance()
{
	/* queue concurrent threads */
	st_mutex.acquire();
	if (st_instance==0) st_instance = new iass_designer_server_link_mngr();
	st_mutex.release();
	return st_instance;
}




iass_ipctunnel_client* iass_designer_server_link_mngr::new_scene(void) {
	server_link_mutex.acquire();
	iass_scenes_server_idl* scenes_server;
	iass_tunnel_idl* remote_scene_inbox_tunnel = NULL;

  
	/* detach all users of the current tunnel_client */
	if (current_scene_tunnel) {

		/* !!!!!!!!!!!!!!!!!!!!!! */

		/* stop the tunnel_client internal loop/thread */
		current_scene_tunnel->stop();
		while(current_scene_tunnel->is_running()) usleep(1);

		delete current_scene_tunnel; 
		current_scene_tunnel = NULL;
	}
  
	scenes_server = find_scenes_server();
	if (!scenes_server) {
		server_link_mutex.release();
		return NULL;
	}

	/* make the new_scene request on the server */
	try {
		remote_scene_inbox_tunnel = scenes_server->new_scene();
	}
	catch (CORBA::Exception &) {
		std::cout << "warning iass_designer_server_link_mngr::new_scene(void), remote new_scene request failed\n";
	}

	/* check if a valid obj was returned */
	if (!remote_scene_inbox_tunnel) {
		server_link_mutex.release();    
		return NULL;
	}

	/* create a new iass_ipctunnel_client to handle the connection with the scene */
	/* the internal loop of the tunnel handler get's automatically started in his constructor */
	current_scene_tunnel = new iass_ipctunnel_client(remote_scene_inbox_tunnel);
	current_scene_tunnel->run_separate_thread();

	server_link_mutex.release();
	return current_scene_tunnel;
}






/* *********    private    ********* */

iass_designer_server_link_mngr::iass_designer_server_link_mngr() {
	current_scene_tunnel = NULL;
}


iass_scenes_server_idl* iass_designer_server_link_mngr::find_scenes_server(void) {
	CORBA::Object_ptr obj = NULL;
	iass_scenes_server_idl* scenes_server = NULL;
	std::string ns_scenes_server_name("");

	/* find the naming_service and get the iass_scenes_server component remote obj */
	if (iass_configs::instance()->get_parameter(SCENES_SERVER_NAMING_SERVICE_ID, &ns_scenes_server_name) != 0 )
		ns_scenes_server_name = SCENES_SERVER_NAMING_SERVICE_ID;
	obj = iass_corba_orb_mngr::instance()->naming_service_obj(ns_scenes_server_name);

	/* check if naming_service search succeded */
	if (!obj) return NULL;

	try {
		scenes_server = iass_scenes_server_idl::_narrow(obj);
	}
	catch (CORBA::Exception &) {
		std::cout << "warning iass_spectator_mngr::find_scenes_server(void), exception on _narrow\n";
		delete obj;
		return NULL;
	}
	return scenes_server;
}
