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

#include "iass_contact_geometry_opal.hh"

iass_contact_geometry_opal::iass_contact_geometry_opal() {
	/* by default be a box with all sides of 10cm= */
	opal_shape = new opal::BoxShapeData();
	((opal::BoxShapeData*)opal_shape)->dimensions.x = 0.1f;
	((opal::BoxShapeData*)opal_shape)->dimensions.y = 0.1f;
	((opal::BoxShapeData*)opal_shape)->dimensions.z = 0.1f;
}


iass_contact_geometry_opal::~iass_contact_geometry_opal() {
	delete opal_shape;
}


void iass_contact_geometry_opal::set_box (double _x, double _y, double _z) {
	/* check for bad parameters */
	if ( (_x<=0) or (_y<=0) or (_z<=0) ) {
		std::cout << "warning iass_contact_geometry_opal::set_box, sides lenghts <= 0\n";
		return;
	}

	if (opal_shape->getType()!=opal::BOX_SHAPE) {
		/* free old opal shape and create a new one */
		delete opal_shape;
		opal_shape = new opal::BoxShapeData();
	}

	/* only change sides */
	((opal::BoxShapeData*)opal_shape)->dimensions.x = _x;
	((opal::BoxShapeData*)opal_shape)->dimensions.y = _y;
	((opal::BoxShapeData*)opal_shape)->dimensions.z = _z;
}


void iass_contact_geometry_opal::set_sphere (double r) {
	/* check for bad parameters */
	if (r<=0) {
		std::cout << "warning iass_contact_geometry_opal::set_sphere, radius <= 0\n";
		return;
	}

	if (opal_shape->getType()!=opal::SPHERE_SHAPE) {
		/* free old opal shape and create a new one */
		delete opal_shape;
		opal_shape = new opal::SphereShapeData();
	}

	/* only change sides */
	((opal::SphereShapeData*)opal_shape)->radius = r;
	return;
}



iass_contact_geometry_type iass_contact_geometry_opal::get_type(void) {
	switch (opal_shape->getType()) {
	case (opal::BOX_SHAPE) : { return IASS_CGEOM_BOX; }
	case (opal::SPHERE_SHAPE) : { return IASS_CGEOM_SPHERE; }
	default : {
		std::cout << "warning iass_contact_geometry_opal::get_type, unknown type\n";
		return IASS_CGEOM_BOX;
	}
	}
}


void iass_contact_geometry_opal::get_basic_type_data(double* x, double* y, double* z, double* w) {
	/* in future if contact geometry is a mesh print a warning if we check for basic types's data */

	switch (opal_shape->getType()) {
	case (opal::BOX_SHAPE) : {
		if ( (x==NULL) or (y==NULL) or (z==NULL) ) {
			std::cout << " WARNING iass_contact_geometry_opal::get_basic_type_data(IASS_CGEOM_BOX), NULL ptr(s)\n";
			return;
		}
		*x = ((opal::BoxShapeData*)opal_shape)->dimensions.x;
		*y = ((opal::BoxShapeData*)opal_shape)->dimensions.y;
		*z = ((opal::BoxShapeData*)opal_shape)->dimensions.z;
		return;
	}

	case (opal::SPHERE_SHAPE) : {
		if (x==NULL) {
			std::cout << " WARNING iass_contact_geometry_opal::get_basic_type_data(IASS_CGEOM_SPHERE), NULL ptr\n";
			return;
		}
		*x = ((opal::SphereShapeData*)opal_shape)->radius;
		return;
	}
	default : {
		std::cout << " WARNING iass_contact_geometry_opal::get_basic_type_data, unknown type\n";
		return;
	}
	}
}


 
opal::ShapeData* iass_contact_geometry_opal::g_opal_shape(void) {
	return opal_shape;
}

