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

#include "iass_entity_factory_basictype_to_opal.hh"

iass_entity_factory_basictype_to_opal::iass_entity_factory_basictype_to_opal(opal::Simulator* _simulator) {
	simulator = NULL;

	assert(_simulator);

	simulator = _simulator;
	filesystem = iass_filesystem::instance();
	materials_store = iass_materials_store::instance();
}


iass_entity* iass_entity_factory_basictype_to_opal::build_entity(iass_entity_ref& ref) {

	/* physics::solids */
	if (strcmp((const char*)ref.node_type->buf, IASS_ENTITY_REF_BASICTYPE_NODE_TYPE_PHYSICS )==0) {
		std::cout << "warning iass_entity_factory_basictype_to_opal::build_entity : physics\n";

		if (strcmp((const char*)ref.node_id->buf, IASS_ENTITY_REF_BASICTYPE_NODE_ID_PHY_BOX )==0) {
			std::cout << "warning iass_entity_factory_basictype_to_opal::build_entity : physics : box\n";

			/* create a box solid_brick */
//      iass_solid_brick* brick = new iass_solid_brick;
//      iass_contact_geometry* cgeom = new iass_contact_geometry;
//      cgeom->set_box ( 0.2f, 0.2f, 0.2f);
//      brick->set_contact_geometry(cgeom);

			/* at this point we should read from file and find a ref to a material... we fake this*/
//      iass_entity_ref mat_ref("23*20*fake-internal-path-to-basic-typesfake_basic_materialsdefault", strlen("23*20*fake-internal-path-to-basic-typesfake_basic_materialsdefault")
//      iass_material_instance mat = materials_store->get_instance(mat_ref);
//      brick->set_material(mat);

			/* create the solid entity and add the brick */
//      iass_solid* solid = new iass_solid;
//      solid->attach_brick(brick);
//      return (iass_entity*) solid;
			return NULL;
		}
		else if (strcmp((const char*)ref.node_id->buf, IASS_ENTITY_REF_BASICTYPE_NODE_ID_PHY_SPHERE )==0) {
			std::cout << "warning iass_entity_factory_basictype_to_opal::build_entity : physics : sphere\n";
			return NULL;
		}

	}
	else std::cout << "warning iass_entity_factory_basictype_to_opal::build_entity, unknown node_type "
		       << ref.node_type->buf << "\n";
	return NULL;
}

