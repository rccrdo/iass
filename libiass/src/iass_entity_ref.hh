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

#ifndef iass_entity_ref_hh
#define iass_entity_ref_hh

#include <string>
#include <sstream>
#include <iostream>
#include "iass_cstring.hh"
#include "iass_libdebug.hh"
#include "iass_serialize_helpers.hh"

/* ==================== TO REMOVE ASAP ====================== */
/* fake path */
#define IASS_ENTITY_REF_BASICTYPE_PATH "basic-type-internal-path"
//                                      123456789012345678901234 24
/* fake type */
#define IASS_ENTITY_REF_BASICTYPE_NODE_TYPE_PHYSICS "basic-type-physics"

#define IASS_ENTITY_REF_BASICTYPE_NODE_TYPE_MATERIAL "basic-type-materials"
//                           01234567890123456789 20

/* basic objs */
#define IASS_ENTITY_REF_BASICTYPE_NODE_ID_PHY_BOX "box"
#define IASS_ENTITY_REF_BASICTYPE_NODE_ID_PHY_SPHERE "sphere"

/* materials */
#define IASS_ENTITY_REF_BASICTYPE_NODE_ID_MAT_DEFAULT "default"
/* ========================================================== */


/* used to serialize iass_entity_ref to a string */
#define IASS_ENTITY_REF_SERIALIZED_BYTES_PER_FIELD 2

/* a cstring serialized is like :
 *
 * BBBBBB...n...|...m...|...q...
 * 012345
 * - bytes 01 are the least significant bytes of a unsigned int representing the length of the path string, n
 * - bytes 23 are the least significant bytes of a unsigned int representing the length of the node_type string, m
 * - bytes 45 are the least significant bytes of a unsigned int representing the length of the node_type string, q
 *   after those bytes the raw strings follow 
 *   ! the symbols '|' are just aestetic and are not present in the serialized stream   
 */

class iass_entity_ref
{
/* functions */
public:
	iass_entity_ref(const char* buf, unsigned int len);
	iass_entity_ref(const char* path_buf, unsigned int path_len,
			const char* note_type_buf, unsigned int note_type_len,
			const char* note_id_buf, unsigned int note_id_len  );
	iass_entity_ref(iass_cstring* cs);
	~iass_entity_ref();

	/* check basic consistency */
	bool consistency_check(void);

/* members */
public:
	/* path to file on filesystem */
	iass_cstring* path;

	/* node_type
	 * ie. in a materials library there are many nodes of type material: <material> ...stuff... </material> */
	iass_cstring* node_type;

	/* node_id
	 * ie. in a materials library it would be the name of a meterial in a node of type material: <material id="wood"> ...stuff... </material> */
	iass_cstring* node_id;

	/* a iass_entity_ref object serialized in iass_cstring */
	iass_cstring* serialized;
};


#endif /*iass_entity_ref_hh*/

