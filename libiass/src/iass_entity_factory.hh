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

#ifndef iass_entity_factory_hh
#define iass_entity_factory_hh

#include "iass_entity.hh"
#include "iass_entity_ref.hh"

class iass_entity_factory
{
/* functions */
public:
	virtual ~iass_entity_factory() {};

	virtual iass_entity* build_entity(iass_entity_ref& xml_ref) = 0;

};

#endif /*iass_entity_factory_hh*/

