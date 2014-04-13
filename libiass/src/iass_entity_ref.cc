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

#include "iass_entity_ref.hh"


iass_entity_ref::iass_entity_ref(const char* buf, unsigned int len) {
	path = NULL;
	node_type = NULL;
	node_id = NULL;
	serialized = NULL;

	/* check arguments*/
	if (!buf) {
		std::cout << "warning iass_entity_ref(const char* buf, unsigned int len), buf is a NULL ptr\n";
		return;
	}
	if (len < (IASS_ENTITY_REF_SERIALIZED_BYTES_PER_FIELD*3) ) {
		std::cout << "warning iass_entity_ref(const char* buf, unsigned int len), len is " << len << ", minimum size for a serialized iass_entity_ref obj is " << (IASS_ENTITY_REF_SERIALIZED_BYTES_PER_FIELD*3) << "\n";
		return;
	}

	/* find the number of chars forming the path */
	unsigned int temp ;
	temp = iass_unserialize_uint16 (buf);
	if (temp > ( len - (IASS_ENTITY_REF_SERIALIZED_BYTES_PER_FIELD*3) ) ){
		std::cout << " temp = " << temp << "\n";
		std::cout << "warning iass_entity_ref(const char* buf, unsigned int len), corrupted content(path)\n";
		return;
	}
	path = new iass_cstring(buf + (IASS_ENTITY_REF_SERIALIZED_BYTES_PER_FIELD*3), temp);   


	/* find the number of chars forming the node_type */
	temp = iass_unserialize_uint16 (buf +2);
	if (temp > ( len - (IASS_ENTITY_REF_SERIALIZED_BYTES_PER_FIELD*3) - path->len) ) {
		std::cout << "warning iass_entity_ref(const char* buf, unsigned int len), corrupted content(node_type)\n";
		return;
	}
	node_type = new iass_cstring(buf + (IASS_ENTITY_REF_SERIALIZED_BYTES_PER_FIELD*3) + path->len, temp);   


	/* find the number of chars forming the node_id */
	temp = iass_unserialize_uint16 (buf +4);
	if (temp > ( len - (IASS_ENTITY_REF_SERIALIZED_BYTES_PER_FIELD*3) - path->len - node_type->len) ) {
		std::cout << "warning iass_entity_ref(const char* buf, unsigned int len), corrupted content(node_id)\n";
		return;
	}
	node_id = new iass_cstring(buf + (IASS_ENTITY_REF_SERIALIZED_BYTES_PER_FIELD*3) + path->len + node_type->len, temp);   


	/* save the serialized string */
	serialized = new iass_cstring(buf, len);
}


iass_entity_ref::iass_entity_ref(const char* path_buf, unsigned int path_len,
                                 const char* node_type_buf, unsigned int node_type_len,
				 const char* node_id_buf, unsigned int node_id_len  ) {
	path = NULL;
	node_type = NULL;
	node_id = NULL;
	serialized = NULL;

	if (!path_buf) {
		std::cout << "warning iass_entity_ref(const char* path_buf, unsigned int path_len, const char* note_type_buf, unsigned int note_type_len, const char* note_id_buf, unsigned int note_id_len), path_buf is a NULL ptr\n";
		return;
	}
	if (!node_type_buf) {
		std::cout << "warning iass_entity_ref(const char* path_buf, unsigned int path_len, const char* note_type_buf, unsigned int note_type_len, const char* note_id_buf, unsigned int note_id_len), node_type_buf is a NULL ptr\n";
		return;
	}
	if (!node_id_buf) {
		std::cout << "warning iass_entity_ref(const char* path_buf, unsigned int path_len, const char* note_type_buf, unsigned int note_type_len, const char* note_id_buf, unsigned int note_id_len), node_id_buf is a NULL ptr\n";
		return;
	}

//  unsigned int serialized_len = IASS_SERIALIZED_LEN_INT16*3 + path_len + node_type_len + node_id_len; 

	std::cout << "warning iass_entity_ref(const char* path_buf, unsigned int path_len, const char* note_type_buf, unsigned int note_type_len, const char* note_id_buf, unsigned int note_id_len), doing nothing!! CRASH :) !!\n";
}


iass_entity_ref::~iass_entity_ref() {
	if (path) delete path;
	if (node_type) delete node_type;
	if (node_id) delete node_id;
	if (serialized) delete serialized;
}


bool iass_entity_ref::consistency_check(void) {
	return (path && node_type && node_id && serialized);
}

