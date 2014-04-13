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

#include "iass_server_scene.hh"

iass_server_scene::iass_server_scene(iass_scene_stats* stats) : iass_scene(stats) {


}

iass_server_scene::~iass_server_scene() {} 


unsigned int iass_server_scene::subscribe_spectator(iass_tunnel_idl* spectator_tunnel) {
	return spectators_mngr.subscribe(spectator_tunnel);
}


void iass_server_scene::unsubscribe_spectator(std::string& ior) {
	spectators_mngr.unsubscribe(ior);
}


void iass_server_scene::broadcast_spectators(iass_ipcblock* ipc_data) {
	assert(ipc_data);
	spectators_mngr.broadcast(ipc_data);
}
