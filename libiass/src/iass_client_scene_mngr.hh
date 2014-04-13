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

#ifndef iass_client_scene_mngr_hh
#define iass_client_scene_mngr_hh

#include <string>
#include "iass_scene_mngr.hh"
#include "iass_client_scene.hh"
#include "iass_ipctunnel_client.hh"


#define SCENE_MNGR_IPC_RESOLVER_LOOP_FREQUENCY 100 /*Hz*/
#define SCENE_MNGR_IPC_RESOLVER_IPC_DATA_BUFFER_LEN 1000

class iass_client_scene_mngr : public iass_scene_mngr
{
/* functions */
public:
	iass_client_scene_mngr(iass_scene_impl_type t, iass_ipctunnel_client* _server__scene_tunnel);
	virtual ~iass_client_scene_mngr();

	void set_name(iass_cstring* name);
	iass_tunnel_idl* subscribe_scene_master(void);
	void unsubscribe_scene_master(void);


	void implementor_resolve_ipc_request(iass_ipcblock* data);


/* members */
private:
	iass_ipctunnel_client* server__scene_tunnel;

};

#endif /*iass_client_scene_mngr_hh*/

