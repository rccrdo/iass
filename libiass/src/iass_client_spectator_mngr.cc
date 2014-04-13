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

#include "iass_client_spectator_mngr.hh"

iass_client_spectator_mngr* iass_client_spectator_mngr::st_instance = 0;
ACE_Thread_Mutex iass_client_spectator_mngr::st_mutex;

iass_client_spectator_mngr* iass_client_spectator_mngr::instance()
{
	/* queue concurrent threads */
	st_mutex.acquire();
	if (st_instance==0) st_instance = new iass_client_spectator_mngr();
	st_mutex.release();
	return st_instance;
}


iass_client_spectator_mngr::iass_client_spectator_mngr() {
	std::cout << " * initing component: iass_client_spectator_mngr\n";
	subscribe_mutex.acquire();
	subscribed = false;
	subscribe_mutex.release();
}


bool iass_client_spectator_mngr::subscribe_spectator(unsigned int scene_id, std::string tunnel_ior) {
	std::string ns_scenes_server_name("");
	iass_scenes_server_idl* scenes_server;
 
	subscribe_mutex.acquire();
	if (subscribed) {
		std::cout << "warning iass_client_spectator_mngr::subscribe_spectator(...), already subscribed to a scene\n";
		subscribe_mutex.release();
		return false;
	}

	scenes_server = find_scenes_server();
	if (!scenes_server) {
		subscribe_mutex.release();
		return false;
	}

	/* finally make the request */
	subscriber_ior.length(tunnel_ior.size());
	memcpy (subscriber_ior.get_buffer(), tunnel_ior.data(), tunnel_ior.size());
	try {
		scenes_server->subscribe_spectator(scene_id, subscriber_ior);
	}
	catch (CORBA::Exception &) {
		std::cout << "warning iass_client_spectator_mngr::subscribe_spectator(...), exception on request\n";
		CORBA::release(scenes_server);
		subscriber_ior.length(0);
		subscribe_mutex.release();
		return false;
	}


	std::cout << "warning iass_client_spectator_mngr::subscribe_spectator(unsigned int scene_id, std::string tunnel_ior), should CORBA::release(server) ?\n";
	CORBA::release(scenes_server);

	subscribed_scene_id = scene_id;
	subscribed = true;
	subscribe_mutex.release();
	return subscribed;
}



void iass_client_spectator_mngr::unsubscribe_spectator(void) {
	std::string ns_scenes_server_name("");
	iass_scenes_server_idl* scenes_server;

	subscribe_mutex.acquire();
	if (!subscribed) {
		std::cout << "warning iass_client_spectator_mngr::unsubscribe_spectator(void), not subscribed\n";   
		subscribe_mutex.release();
		return;
	}

	scenes_server = find_scenes_server();
	if (!scenes_server) {
		subscribe_mutex.release();
		return;
	}

	/* finally make the request */
	try {
		scenes_server->unsubscribe_spectator(subscribed_scene_id, subscriber_ior);
	}
	catch (CORBA::Exception &) {
		std::cout << "warning iass_client_spectator_mngr::unsubscribe_spectator(void), exception on request\n";
		CORBA::release(scenes_server);
		subscribe_mutex.release();
		return;
	}

	std::cout << "warning iass_client_spectator_mngr::unsubscribe_spectator(void), should CORBA::release(server) ?\n";
	CORBA::release(scenes_server);
	subscriber_ior.length(0);
	subscribed = false;
	subscribe_mutex.release();
	return;
}


iass_scenes_server_idl* iass_client_spectator_mngr::find_scenes_server(void) {
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
		std::cout << "warning iass_client_spectator_mngr::find_scenes_server(void), exception on _narrow\n";
		delete obj;
		return NULL;
	}
	return scenes_server;
}

