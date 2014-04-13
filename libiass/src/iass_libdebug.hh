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

#ifndef iass_libdebug_hh
#define iass_libdebug_hh

/* main debug switches */
#define debug_libiass_force_all
//#define debug_libiass_force_none

/* check main switches consistency */
#ifdef debug_libiass_force_all 
#ifdef debug_libiass_force_none
#warning "iass_libdebug.hh: both debug_libiass_force_all and debug_libiass_force_none flags are defined, using only debug_libiass_force_all"
#undef debug_libiass_force_none
#endif
#else
#ifndef debug_libiass_force_none
#warning "iass_libdebug.hh: none of debug_libiass_force_all and debug_libiass_force_none flags are defined, using only debug_libiass_force_none"
#define debug_libiass_force_none
#endif
#endif


#ifndef debug_libiass_force_none
#undef debug_iass_ipc_block
#undef debug_iass_ipc_tunnel
#undef debug_iass_ipc_tunnel_client
#undef debug_iass_queue
#undef debug_iass_queue_ts
#undef debug_iass_scene_update
#undef debug_iass_packet
#undef debug_iass_sanity_check
#undef debug_iass_serialize_helpers

/* make assert(expr) a no-op */
#define NDEBUG
#endif

#ifdef debug_libiass_force_all
#define debug_iass_ipc_block
#define debug_iass_ipc_tunnel
#define debug_iass_ipc_tunnel_client
#define debug_iass_queue
#define debug_iass_queue_ts
#define debug_iass_scene_update
#define debug_iass_packet
#define debug_iass_sanity_check
#define debug_iass_serialize_helpers

/* use assert(expr) */
#undef NDEBUG
#endif

#include <assert.h>


#endif /*iass_libdebug_hh*/

