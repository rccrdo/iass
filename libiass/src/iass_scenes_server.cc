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

#include "iass_scenes_server.hh"

iass_scenes_server* iass_scenes_server::st_instance = 0;
ACE_Thread_Mutex iass_scenes_server::st_mutex;

iass_scenes_server* iass_scenes_server::instance()
{
	/* queue concurrent threads */
	st_mutex.acquire();
	if (st_instance==0) st_instance = new iass_scenes_server();
	st_mutex.release();
	return st_instance;
}


iass_scenes_server::iass_scenes_server() {
	std::cout << " * initing component: iass_scenes_server\n";
}


iass_scenes_server::~iass_scenes_server() {}


iass_scene_stats** iass_scenes_server::get_scene_stats(void) {
	access_mutex.acquire();
	/* find the number of existing scenes */
	unsigned int scenes_num = 0;
	for (unsigned int i=0; i < scene_mngrs.size(); i++)
		if (scene_exists(i)) scenes_num++;

	if (scenes_num==0) {
		access_mutex.release();
		return NULL;
	}

	unsigned int stats_cur = 0;
	/* malloc a NULL terminated vector of iass_scene_stats* */
	iass_scene_stats** stats_array = (iass_scene_stats**) malloc( (scenes_num+1)*sizeof(iass_scene_stats*) );
	memset(stats_array, 0, (scenes_num+1)*sizeof(iass_scene_stats*) );

	/* add scenes stats to stats_array */
	for (unsigned int i=0; i < scene_mngrs.size(); i++) {
		if (scene_exists(i)) {
			stats_array[stats_cur] = scene_mngrs[i]->get_stats();
			stats_cur++;
		}
	}
	access_mutex.release();
	return stats_array;
}



iass_tunnel_idl* iass_scenes_server::new_scene() throw () {
	access_mutex.acquire();
	unsigned int id = get_free_scene_slot();
	iass_server_scene_mngr* new_scene_mngr = new iass_server_scene_mngr(scene_impl_t_server_opal_ode, id);
	scene_mngrs[id] = new_scene_mngr;
	iass_tunnel_idl* tunnel = new_scene_mngr->subscribe_scene_master();
	access_mutex.release();
	return tunnel;
}


unsigned int iass_scenes_server::get_free_scene_slot(void)
{
	for (unsigned int index = 0; index < scene_mngrs.size(); index ++ ) 
	{ // we search for a free slot
		if (scene_mngrs[index] == NULL) return index;
	}
	// if we didn't found a free slot than we push a new space/slot in the vector
	scene_mngrs.push_back(NULL);
	return (scene_mngrs.size() -1 );
}


bool iass_scenes_server::scene_exists(unsigned int id)
{
	if ( id >= scene_mngrs.size() ) return false;
	if (scene_mngrs[id] == NULL) return false;
	return true;
}


CORBA::Boolean iass_scenes_server::subscribe_spectator(CORBA::UShort _scene_id, const iass_corba_raw_stream& _ior) throw () {
	CORBA::Object* obj;
	iass_tunnel_idl* subscriber_tunnel;
	std::string ior_string;
	bool success = false;

	/* avoid changes on the scenes until release */
	access_mutex.acquire();
	if (!scene_exists(_scene_id)) {
		std::cout << "warning iass_scenes_server::subscribe_spectator(CORBA::UShort _scene_id, const iass_corba_raw_stream& _ior), _scene_id "
			  << _scene_id << " is an invalid id\n";
		access_mutex.release();
		return false;
	}

	if (_ior.length()==0) {
		std::cout << "warning iass_scenes_server::subscribe_spectator(CORBA::UShort _scene_id, const iass_corba_raw_stream& _ior), _ior has length zero\n";
		access_mutex.release();
		return false;
	}

	if (_ior.get_buffer() == NULL) {
		std::cout << "warning iass_scenes_server::subscribe_spectator(CORBA::UShort _scene_id, const iass_corba_raw_stream& _ior), _ior.get_buffer() is a NULL ptr\n";
		access_mutex.release();
		return false;
	}

	ior_string.assign((const char*) _ior.get_buffer(), _ior.length());
	obj = iass_corba_orb_mngr::instance()->obj_from_ior(ior_string);

	if (!obj) {
		access_mutex.release();
		return false;
	}

	try {
		subscriber_tunnel = iass_tunnel_idl::_narrow(obj);
	}
	catch (CORBA::Exception &) {
		std::cout << "warning iass_scenes_server::subscribe_spectator(CORBA::UShort _scene_id, const iass_corba_raw_stream& _ior), exception on _narrow\n";
		delete obj;
		access_mutex.release();
		return false;
	}

	success = scene_mngrs[_scene_id]->subscribe_spectator(subscriber_tunnel);

	if (!success) {
		CORBA::release(subscriber_tunnel);
		delete obj;
	}
	else {
		std::cout << "warning iass_scenes_server::subscribe_spectator(CORBA::UShort _scene_id, const iass_corba_raw_stream& _ior), tunnel still works if a I delete obj ?\n";
		delete obj;   
	}
	access_mutex.release();
	return success;
}


void iass_scenes_server::unsubscribe_spectator(CORBA::UShort _scene_id, const iass_corba_raw_stream& _ior) throw () {
	std::string ior_string;

	/* avoid changes on the scenes until release */
	access_mutex.acquire();
	if (!scene_exists(_scene_id)) {
		std::cout << "warning iass_scenes_server::unsubscribe_spectator(CORBA::UShort _scene_id, const iass_corba_raw_stream& _ior), _scene_id "
			  << _scene_id << " is an invalid id\n";
		access_mutex.release();
		return;
	}

	if (_ior.length()==0) {
		std::cout << "warning iass_scenes_server::unsubscribe_spectator(CORBA::UShort _scene_id, const iass_corba_raw_stream& _ior), _ior has length zero\n";
		access_mutex.release();
		return;
	}

	if (_ior.get_buffer() == NULL) {
		std::cout << "warning iass_scenes_server::unsubscribe_spectator(CORBA::UShort _scene_id, const iass_corba_raw_stream& _ior), _ior.get_buffer() is a NULL ptr\n";
		access_mutex.release();
		return;
	}

	ior_string.assign((const char*) _ior.get_buffer(), _ior.length());
	scene_mngrs[_scene_id]->unsubscribe_spectator(ior_string);
	return;
}


