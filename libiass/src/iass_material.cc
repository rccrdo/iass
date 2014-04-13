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

#include "iass_material.hh"

iass_material::iass_material(iass_entity_ref* _xml_ref) {
	/* dafaults */
	mat_xml_ref = NULL;
	mat_instances = 0;

	/* physics defaults */
	mat_hardness = 0.5;
	mat_friction = 0.5;
	mat_bounciness = 0.5;
	mat_density = 0.5;

	if (_xml_ref == NULL) {
		std::cout << "warning iass_material(iass_entity_ref& _xml_ref), _xml_ref is a NULL ptr\n";
	}
	else mat_xml_ref = new iass_entity_ref((const char*) _xml_ref->serialized->buf, _xml_ref->serialized->len);
}


iass_material::~iass_material() {
	mat_mutex.acquire();
	if (mat_xml_ref) delete mat_xml_ref;
	mat_mutex.release();
}


iass_entity_ref* iass_material::xml_ref(void) {
	mat_mutex.acquire();
	return mat_xml_ref;
	mat_mutex.release();
}


void iass_material::hardness(double v) {
	if ((v>=0.f) and (v<=1.0f)) {
		mat_mutex.acquire();
		mat_hardness = v;
		mat_mutex.release();
	}
	else std::cout << "warning iass_material::hardness(" << v << "), out of valid reanges\n";
}


void iass_material::friction(double v) {
	if ((v>=0.f) and (v<=1.0f)) {
		mat_mutex.acquire();
		mat_friction = v;
		mat_mutex.release();
	}
	else std::cout << "warning iass_material::friction(" << v << "), out of valid reanges\n";
}


void iass_material::bounciness(double v) {
	if ((v>=0.f) and (v<=1.0f)) {
		mat_mutex.acquire();
		mat_bounciness = v;
		mat_mutex.release();
	}
	else std::cout << "warning iass_material::bounciness(" << v << "), out of valid reanges\n";
}


void iass_material::density(double v) {
	if ((v>=0.f) and (v<=1.0f)) {
		mat_mutex.acquire();
		mat_density = v;
		mat_mutex.release();
	}
	else std::cout << "warning iass_material::density(" << v << "), out of valid reanges\n";
}


double iass_material::hardness(void) {
	mat_mutex.acquire();
	double ret = mat_hardness;
	mat_mutex.release();
	return ret;
}


double iass_material::friction(void){
	mat_mutex.acquire();
	double ret = mat_friction;
	mat_mutex.release();
	return ret;
}


double iass_material::bounciness(void) {
	mat_mutex.acquire();
	double ret = mat_bounciness;
	mat_mutex.release();
	return ret;
}


double iass_material::density(void) {
	mat_mutex.acquire();
	double ret = mat_density;
	mat_mutex.release();
	return ret;
}


unsigned int iass_material::num_instances(void) {
	mat_mutex.acquire();
	unsigned int ret = mat_instances;
	mat_mutex.release();
	return ret;
}

void iass_material::inc_instances_count(void) {
	mat_mutex.acquire();
	mat_instances++;
	mat_mutex.release();
}

void iass_material::dec_instances_count(void) {
	mat_mutex.acquire();
	if (mat_instances==0) {
		std::cout << "warning iass_material::dec_instances_count, mat_instances is already 0 !\n";
		return;
	}
	mat_instances--;
	mat_mutex.release();
}
