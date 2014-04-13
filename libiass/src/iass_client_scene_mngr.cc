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

#include "iass_client_scene_mngr.hh"


iass_client_scene_mngr::iass_client_scene_mngr(iass_scene_impl_type t, iass_ipctunnel_client* _server__scene_tunnel)
	: iass_scene_mngr(0 /* fake id, not important for clients */) {
	assert(_server__scene_tunnel);
	server__scene_tunnel = _server__scene_tunnel;

	switch (t) {
	case (scene_impl_t_client_ogre3d) : {
		/* fake the scene obj for now */
		// _scene = 1;
		// entity_factory = NULL;
		break;
	}
	default: {
		// _scene = 1;
		// entity_factory = NULL;
		std::cout << "warning iass_client_scene_mngr(constr.), t has bad value "<< (int) t << "\n";
	}
	}
}


iass_client_scene_mngr::~iass_client_scene_mngr() {}


void iass_client_scene_mngr::set_name (iass_cstring* name) {
	assert(name);
	scene_stats.name.assign((const char*)name->buf, name->len);
}


iass_tunnel_idl* iass_client_scene_mngr::subscribe_scene_master(void) {
	if (!_scene) {
		std::cout << "warning iass_client_scene_mngr::subscribe_scene_master(void), _scene is a NULL ptr\n";
		return NULL;
	}

	mutex.acquire();
	/* start the tunnel loop if this is the first subscriber */
	if (!subscribed_scene_masters) {
		run_separate_thread_ipc_resolver();
		_incoming_tunnel.run_separate_thread();
	}
	else {
		std::cout << "warning iass_client_scene_mngr::subscribe_scene_master(void), cannot subscribe more thn one\n";
		mutex.release();
		return NULL;
	}

	subscribed_scene_masters++;
	mutex.release();
	return _incoming_tunnel._this();
}


void iass_client_scene_mngr::unsubscribe_scene_master(void) {
	if (!_scene) {
		std::cout << "warning iass_client_scene_mngr::unsubscribe_scene_master(void), _scene is a NULL ptr\n";
		return;
	}

	mutex.acquire();
	if (!subscribed_scene_masters) {
		std::cout << "warning iass_client_scene_mngr::unsubscribe_scene_master(void), no scene master subscribed\n";
		mutex.release();
		return;
	}
	subscribed_scene_masters --;
	/* stop the tunnel loop : on the spectator side there cannot be more than one master at time */
	_incoming_tunnel.stop();
	ipc_resolver_stop();
	mutex.release();
}


void iass_client_scene_mngr::implementor_resolve_ipc_request(iass_ipcblock* data) {
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
		std::cout << "warning iass_client_scene_mngr::implementor_resolve_ipc_request(iass_ipcblock* data), int8_vector has size zero\n";
		return;
	}
	if (int16_vector_size == 0 ) {
		std::cout << "warning iass_client_scene_mngr::implementor_resolve_ipc_request(iass_ipcblock* data), int16_vector has size zero\n";
		return;
	}

	uint8_array = (uint8_t*) data->int8_vector_ptr();
	uint16_array = (uint16_t*) data->int16_vector_ptr();
	content_t = uint8_array[0];
  

/*  if (content_t == (uint8_t) ipc_block_content__server_scene_mngr_request) {
// process request 
return;
}
else if (content_t == (uint8_t) ipc_block_content__scene_mngr_request) {
// process request in parent 
iass_scene_mngr::scene_mngr_resolve_ipc_request(data);
return;
}
else {
*/
	std::cout << "warning iass_client_scene_mngr::implementor_resolve_ipc_request(iass_ipcblock* data), content_t is "
		  << (int) content_t <<", not suitable as request for the iass_client_scene_mngr or iass_scene_mngr\n";
//  }
}




