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

#include "iass_solid_brick_opal.hh"

iass_solid_brick_opal::iass_solid_brick_opal()
{
	name = "Untitled Solid Brick";
	contact_geometry = NULL;
}


iass_solid_brick_opal::~iass_solid_brick_opal() {}


void iass_solid_brick_opal::set_contact_geometry(iass_contact_geometry* _cgeom) {
	if (_cgeom==NULL) {
		std::cout << "warning iass_solid_brick_opal::set_contact_geometry(NULL ptr)\n";
		return;
	}
  
	/* delete actual contact geometry if any*/
	if (contact_geometry) delete contact_geometry;

	/* save new ptr */
	contact_geometry = _cgeom;
	return;
}


iass_contact_geometry* iass_solid_brick_opal::get_contact_geometry (void){
	return contact_geometry;
}


void iass_solid_brick_opal::set_material(iass_material_instance* m) {
	if (m==NULL) {
		std::cout << "warning iass_solid_brick_opal::set_material(NULL ptr)\n";
		return;
	}
	material = m;
}


iass_material_instance* iass_solid_brick_opal::get_material (void) {
	return material;
}



void iass_solid_brick_opal::set_name(std::string _name) {
	name = _name.c_str();
}

std::string iass_solid_brick_opal::get_name(void) {
	return name.c_str();
}



void iass_solid_brick_opal::s_transform(const opal::Matrix44r& t) {
	transform = t;
};

void iass_solid_brick_opal::g_transform(opal::Matrix44r& t) {
	t = transform;
};


void iass_solid_brick_opal::s_position(const opal::Vec3r& v) {
	transform.setPosition(v.x, v.y, v.z);
};

void iass_solid_brick_opal::g_position(opal::Vec3r& v) {
	opal::Point3r p= transform.getPosition();
	v.x = p[0];
	v.y = p[1];
	v.z = p[2];
};


void iass_solid_brick_opal::s_quaternion(const opal::Quaternion& q) {
	transform.setQuaternion(q[0], q[1], q[2], q[3]);
};

void iass_solid_brick_opal::g_quaternion(opal::Quaternion& q) {
	q = transform.getQuaternion();
};



