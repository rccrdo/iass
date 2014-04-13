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

#ifndef iass_ipc_helpers_hh
#define iass_ipc_helpers_hh

#include <iostream>
#include <stdint.h>
#include "iass_libdebug.hh"
#include "iass_ipcblock.hh"
#include "iass_cstring.hh"


/* limit the numer of content types to 256 */
enum iass_ipcblock_content_type   {
	ipc_block_content__base				= (uint8_t) 0,
	ipc_block_content__scene_mngr_request			= (uint8_t) 1,
	ipc_block_content__scene_mngr_server_side_request	= (uint8_t) 2,
	ipc_block_content__last				= (uint8_t) 3
};

inline bool iass_ipcblock_content_type_is_valid(iass_ipcblock_content_type t);



/* *************    iass_scene_mngr    ************* */
/* limit the numer of scene_mngr_content types to 65Ksomething */
enum iass_ipcblock_scene_mngr_request_type   {
	scene_mngr_request__base 		= (uint16_t) 0,
	scene_mngr_request__set_name		= (uint16_t) 1,
	scene_mngr_request__last		= (uint16_t) 2
};

inline bool iass_ipcblock_scene_mngr_request_type_is_valid(iass_ipcblock_scene_mngr_request_type t);

inline iass_ipcblock* iass_ipc__scene_mngr__set_name(iass_cstring* name);












bool iass_ipcblock_content_type_is_valid(iass_ipcblock_content_type t) {
	return ((t > ipc_block_content__base) and (t < ipc_block_content__last));
}




/* *************    iass_scene_mngr    ************* */
bool iass_ipcblock_scene_mngr_request_type_is_valid(iass_ipcblock_scene_mngr_request_type t) {
	return ((t > scene_mngr_request__base) and (t < scene_mngr_request__last));
}


iass_ipcblock* iass_ipc__scene_mngr__set_name(iass_cstring* name) {
	iass_ipcblock* obj;
	assert(name);

	obj = new iass_ipcblock;
	obj->push_int8 (ipc_block_content__scene_mngr_request);
	obj->push_int16 (scene_mngr_request__set_name);
	obj->push_cstring(name);

	return obj;
}


#endif /*iass_ipc_helpers_hh*/

