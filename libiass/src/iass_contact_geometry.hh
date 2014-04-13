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

#ifndef iass_contact_geometry_hh
#define iass_contact_geometry_hh

/* valid contact geometries types */
enum iass_contact_geometry_type {
	IASS_CGEOM_BOX,
	IASS_CGEOM_SPHERE
};


class iass_contact_geometry
{
/* functions */
public:
//  iass_contact_geometry();
	virtual ~iass_contact_geometry() {};

	/* set geometry to a basic type */
	virtual void set_box (double x, double y, double z) = 0;
	virtual void set_sphere (double r) = 0;

	/* get geometry type */
	virtual iass_contact_geometry_type get_type(void) = 0;

	/* get data for basic types */
	virtual void get_basic_type_data(double* x, double* y, double* z, double* w) = 0;

};

#endif /*iass_contact_geometry_hh*/

