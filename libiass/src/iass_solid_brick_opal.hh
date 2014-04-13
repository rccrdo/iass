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


#ifndef iass_solid_brick_opal_hh
#define iass_solid_brick_opal_hh

#include "iass_contact_geometry.hh"
#include "iass_material_instance.hh"
#include "iass_entity.hh"
#include "iass_solid_brick.hh"



class iass_solid_brick_opal : public iass_solid_brick
{
/* functions */
public:
	iass_solid_brick_opal();
	~iass_solid_brick_opal();


	/* set and get brick's name */
	void set_name(std::string _name);
	std::string get_name(void);

	void set_contact_geometry(iass_contact_geometry* _cgeom);
	iass_contact_geometry* get_contact_geometry (void);

	void set_material(iass_material_instance* m);
	iass_material_instance* get_material (void);


	void s_transform(const opal::Matrix44r& t);
	void g_transform(opal::Matrix44r& t);

	void s_position(const opal::Vec3r& v);
	void g_position(opal::Vec3r& v);

	void s_quaternion(const opal::Quaternion& q);
	void g_quaternion(opal::Quaternion& q);

private:
	/* transform representing obj position and rotation */
	opal::Matrix44r transform;

	iass_contact_geometry* contact_geometry;
	iass_material_instance* material;

	std::string name;
};

#endif /*iass_solid_brick_opal_hh*/

