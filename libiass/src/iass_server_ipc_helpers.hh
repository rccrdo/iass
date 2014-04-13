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

#ifndef iass_ipc_server_side_helpers_hh
#define iass_ipc_server_side_helpers_hh

#include <iostream>
#include <stdint.h>
#include "iass_libdebug.hh"
#include "iass_ipc_helpers.hh"
#include "iass_entity_ref.hh"


/* *******    iass_server_scene_mngr    ******** */
/* limit the numer of scene_mngr_content types to 65Ksomething */
enum iass_ipcblock_server_scene_mngr_request_type   {
	server_scene_mngr_request__base 			= (uint16_t) 0,
	server_scene_mngr_request__realize_physic_brick	= (uint16_t) 1,
	server_scene_mngr_request__last			= (uint16_t) 2,
};


inline iass_ipcblock* iass_ipc__server_scene_mngr__realize_physic_brick(iass_entity_ref& ref);


/* *******    iass_server_scene_mngr    ******** */

iass_ipcblock* iass_ipc__server_scene_mngr__realize_physic_brick(iass_entity_ref& ref) {
	iass_ipcblock* obj;
	iass_cstring* serialized_ref;

	obj = new iass_ipcblock;
	obj->push_int8 (ipc_block_content__server_scene_mngr_request);
	obj->push_int16 (server_scene_mngr_request__realize_physic_brick);

	serialized_ref = new iass_cstring(ref.serialized->buf, ref.serialized->len);
	obj->push_cstring(serialized_ref);
	return obj;
}



#endif /*iass_ipc_server_side_helpers_hh*/

