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

#include "iass_materials_store.hh"


iass_materials_store* iass_materials_store::st_instance = 0;
ACE_Thread_Mutex iass_materials_store::st_mutex;

iass_materials_store* iass_materials_store::instance()
{
	/* queue concurrent threads */
	st_mutex.acquire();
	if (st_instance==0) st_instance = new iass_materials_store();
	st_mutex.release();
	return st_instance;
}


iass_materials_store::iass_materials_store() {
	std::cout << " * initing component: iass_materials_store\n";

	/*iass_material* mat = new iass_material();
	  mat->set_name(IASS_XML_TREE_PATH_MATERIAL);
	  mat->set_path(iass_entity_ref_INTERNAL_PATH);
	  store_material(mat);*/

}


iass_material_instance* iass_materials_store::material_instance(iass_entity_ref* _xml_ref) {
  
	if (_xml_ref == NULL) {
		std::cout << "warning iass_materials_store::material_instance(iass_entity_ref* _xml_ref), _xml_ref is a NULL ptr\n";
		return NULL;
	}

	/* check if material pointed by xml_ref is already loaded */
	iass_cstring* cs_serialized_ref = _xml_ref->serialized;
	std::string serialized_ref;
	serialized_ref.assign((const char*)cs_serialized_ref->buf, cs_serialized_ref->len);  

	xml_refs_map_iter = xml_refs_map.find(serialized_ref);
	if( xml_refs_map_iter != xml_refs_map.end() ) {
		/* material already in use */
		return new iass_material_instance(materials_vector[xml_refs_map_iter->second]);
	}

	/* else we gotta load it */
	std::cout << "warning iass_materials_store::material_instance, not capable to load materials yet; _xml_ref is:"
		  << serialized_ref << "\n";
	return NULL;
}


void iass_materials_store::store_material(iass_material* mat) {
	if (!mat) {
		std::cout << "warning iass_materials_store::store_material(iass_material* mat), mat is a NULL ptr\n";
		return;
	}

	iass_entity_ref* xml_ref = mat->xml_ref();
	iass_cstring* cs_serialized_ref = xml_ref->serialized;
	std::string serialized_ref;
	serialized_ref.assign((const char*)cs_serialized_ref->buf, cs_serialized_ref->len);  

	/* see if we already have this material */
	xml_refs_map_iter = xml_refs_map.find(serialized_ref);
	if( xml_refs_map_iter != xml_refs_map.end() ) {
		std::cout << "warning iass_materials_store::store_material(iass_material* mat), material: "
			  << serialized_ref << ", already present\n";
		return;
	}

	/* store new material */
	materials_vector.push_back(mat);
	xml_refs_map.insert( std::make_pair(serialized_ref, materials_vector.size()-1) ); 
}

