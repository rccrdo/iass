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

#ifndef iass_scene_opal_hh
#define iass_scene_opal_hh

#include <string>
#include <iostream>
#include <sstream>

#include <vector>
#include <queue>
#include "iass_entity_factory_basictype_to_opal.hh"
#include "iass_server_scene.hh"
#include "iass_entity.hh"
#include "iass_filesystem.hh"
#include "iass_configs.hh"
#include "iass_entity_ref.hh"
#include "iass_memblock.hh"
#include "iass_collection.hh"
#include "iass_sharedfile_data.hh"
#include <opal/opal.h>


class iass_scene_opal : public iass_server_scene
{
/* functions */
public:
	iass_scene_opal(iass_scene_stats* stats);

	void main_loop_jobs (void) {};

	opal::Simulator* internal_opal_simulator(void);


/* members */
private:
	iass_collection<iass_entity*> entities_collection;
	iass_filesystem* filesystem;

	opal::Simulator* simulator;
};

#endif /*iass_scene_opal_hh*/

