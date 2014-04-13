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

#ifndef iass_server_scene_hh
#define iass_server_scene_hh

#include "iass_scene.hh"
#include <string>
#include "iass_scene_spectators_mngr.hh"
#include "iass_ipcblock.hh"


class iass_server_scene : public iass_scene
{
/* functions */
public:
	iass_server_scene(iass_scene_stats* stats);
	virtual ~iass_server_scene();

	unsigned int subscribe_spectator(iass_tunnel_idl* spectator_tunnel);
	void unsubscribe_spectator(std::string& ior);

	void broadcast_spectators(iass_ipcblock* ipc_data);


/* members */
private:
	iass_scene_spectators_mngr spectators_mngr;

};

#endif /*iass_server_scene_hh*/

