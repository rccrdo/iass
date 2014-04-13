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

#include "iass_entity.hh"

iass_entity::iass_entity()
{
	ent_t = iass_entity_NONE;
	ent_id = -1;
	name = "Untitled Entity";
}


void iass_entity::set_id(int i) {
	ent_id = i;
}


int iass_entity::get_id(void) {
	return ent_id;
}


void iass_entity::set_name(std::string _name) {
	name.assign(_name);
}


std::string iass_entity::get_name(void) {
	return name.c_str();
}

 
iass_entity_type iass_entity::get_type(void) {
	return ent_t;
}
