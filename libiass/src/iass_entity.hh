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

#ifndef iass_entity_hh
#define iass_entity_hh

#include <string>
#include <opal/Vec3r.h>
#include <opal/Quaternion.h>
#include <opal/Matrix44r.h>

/*  Valid iass_entity objs types */
enum iass_entity_type {
	iass_entity_NONE
};


class iass_entity
{
/* functions */
public:
	iass_entity();
	virtual ~iass_entity() {};

	/* set and get entity's id */
	void set_id(int i);
	int get_id(void);

	/* set and get entity's name */
	void set_name(std::string _name);
	std::string get_name(void);

	/* get entity's type */
	iass_entity_type get_type(void);


	virtual opal::Matrix44r& transform(void) = 0;
	virtual void transform(opal::Matrix44r& t) = 0;

	virtual opal::Vec3r& position(void) = 0;
	virtual void position(opal::Vec3r& v) = 0;

	virtual opal::Quaternion& quaternion(void) = 0;
	virtual void quaternion(opal::Quaternion& q) = 0;


/* members */
protected:
	iass_entity_type ent_t;

private:
	std::string name;
	int ent_id;
};

#endif /*iass_entity_hh*/

