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

#include "iass_ipcblock.hh"

iass_ipcblock::iass_ipcblock() {
	_serialized = NULL;
	dirty = false;
}

iass_ipcblock::~iass_ipcblock() {
	clear();
}


unsigned int iass_ipcblock::serialized_len(void) {
	unsigned int len =0;
	/* (number of elements * 2bytes) + (len of every vector for the size of the respective type) */
	len += IASS_SERIALIZED_LEN_INT16 + int8_vector.size() * IASS_SERIALIZED_LEN_INT8      /* int8_t  */
		+ IASS_SERIALIZED_LEN_INT16 + int16_vector.size() * IASS_SERIALIZED_LEN_INT16  /* int16_t */
		+ IASS_SERIALIZED_LEN_INT16 + int32_vector.size() * IASS_SERIALIZED_LEN_INT32  /* int32_t */
		+ IASS_SERIALIZED_LEN_INT16 + float_vector.size() * IASS_SERIALIZED_LEN_FLOAT        /* floats  */
		+ IASS_SERIALIZED_LEN_INT16 + double_vector.size() * IASS_SERIALIZED_LEN_DOUBLE;     /* doubles */

	/* (number of elements * 2bytes) + ( save the len of every cstring in 2 bytes ) */
	len += IASS_SERIALIZED_LEN_INT16 + cstrings_vector.size() * IASS_SERIALIZED_LEN_INT16;       /* cstrings */
	/* (add the sum of legths of the cstrings */
	for (unsigned int i=0; i < cstrings_vector.size(); i++) len+= cstrings_vector[i]->len;

	return len;
}


unsigned int iass_ipcblock::serialize(char* dest) {
	unsigned int cur = 0;
	assert(dest);
	/* if ipc_block is empty there is nothing to serialize */

	/* int8_vector */
	iass_serialize_uint16 (dest, ((uint16_t) (int8_vector.size() & 0xFFFF)));
	cur += IASS_SERIALIZED_LEN_INT16;
	for (unsigned int i=0; i < int8_vector.size(); i++) {
		iass_serialize_int8(dest + cur, int8_vector[i]);
		cur += IASS_SERIALIZED_LEN_INT8;
	}

	/* int16_vector */
	iass_serialize_uint16 (dest + cur, ((uint16_t) (int16_vector.size() & 0xFFFF)));
	cur += IASS_SERIALIZED_LEN_INT16;
	for (unsigned int i=0; i < int16_vector.size(); i++) {
		iass_serialize_int16(dest + cur, int16_vector[i]);
		cur += IASS_SERIALIZED_LEN_INT16;
	}

	/* int32_vector */
	iass_serialize_uint16 (dest + cur, ((uint16_t) (int32_vector.size() & 0xFFFF)));
	cur += IASS_SERIALIZED_LEN_INT16;
	for (unsigned int i=0; i < int32_vector.size(); i++) {
		iass_serialize_int32(dest + cur, int32_vector[i]);
		cur += IASS_SERIALIZED_LEN_INT32;
	}

	/* float_vector */
	iass_serialize_uint16 (dest + cur, ((uint16_t) (float_vector.size() & 0xFFFF)));
	cur += IASS_SERIALIZED_LEN_INT16;
	for (unsigned int i=0; i < float_vector.size(); i++) {
		iass_serialize_float(dest + cur, float_vector[i]);
		cur += IASS_SERIALIZED_LEN_FLOAT;
	}

	/* double_vector */
	iass_serialize_uint16 (dest + cur, ((uint16_t) (double_vector.size() & 0xFFFF)));
	cur += IASS_SERIALIZED_LEN_INT16;
	for (unsigned int i=0; i < double_vector.size(); i++) {
		iass_serialize_double(dest + cur, double_vector[i]);
		cur += IASS_SERIALIZED_LEN_DOUBLE;
	}

	/* cstrings */
	iass_serialize_uint16 (dest + cur, ((uint16_t) (cstrings_vector.size() & 0xFFFF)));
	cur += IASS_SERIALIZED_LEN_INT16;
	for (unsigned int i=0; i < cstrings_vector.size(); i++) {
		iass_serialize_uint16(dest + cur, ((uint16_t) (cstrings_vector[i]->len & 0xFFFF)));
		cur += IASS_SERIALIZED_LEN_INT16;
	}
	for (unsigned int i=0; i < cstrings_vector.size(); i++) {
		memcpy(dest + cur, cstrings_vector[i]->buf, cstrings_vector[i]->len);
		cur += cstrings_vector[i]->len;
	}
	return cur;
}


unsigned int iass_ipcblock::serialize_and_store(char* dest) {
	unsigned int serialized_len;
	serialized_len = serialize(dest);
	if (serialized_len) {
		if (_serialized) delete _serialized;
		_serialized = new iass_cstring (dest, serialized_len);    
		dirty = false;  
	}
	return serialized_len;
}


unsigned int iass_ipcblock::unserialize(const char* src, unsigned int len) {
	unsigned int cur = 0;
	uint16_t num_elements = 0;
	assert(src);
	assert(len);
	/* avoid unserializing to a non-empty ipc_block */
	assert(IPC_BLOCK_IS_EMPTY);

	/* load int8_vector */
	num_elements = iass_unserialize_uint16 (src + cur);
	cur += IASS_SERIALIZED_LEN_INT16;
	for (unsigned int i = 0; i < num_elements; i++ ) {
		push_int8(iass_unserialize_int8 (src + cur));
		cur += IASS_SERIALIZED_LEN_INT8;
	}

	/* load int16_vector */
	num_elements = iass_unserialize_uint16 (src + cur);
	cur += IASS_SERIALIZED_LEN_INT16;
	for (unsigned int i = 0; i < num_elements; i++ ) {
		push_int16(iass_unserialize_int16 (src + cur));
		cur += IASS_SERIALIZED_LEN_INT16;
	}

	/* load int32_vector */
	num_elements = iass_unserialize_uint16 (src + cur);
	cur += IASS_SERIALIZED_LEN_INT16;
	for (unsigned int i = 0; i < num_elements; i++ ) {
		push_int32(iass_unserialize_int32 (src + cur));
		cur += IASS_SERIALIZED_LEN_INT32;
	}

	/* load floats */
	num_elements = iass_unserialize_uint16 (src + cur);
	cur += IASS_SERIALIZED_LEN_INT16;
	for (unsigned int i = 0; i < num_elements; i++ ) {
		push_float(iass_unserialize_float (src + cur));
		cur += IASS_SERIALIZED_LEN_FLOAT;
	}

	/* load doubles */
	num_elements = iass_unserialize_uint16 (src + cur);
	cur += IASS_SERIALIZED_LEN_INT16;
	for (unsigned int i = 0; i < num_elements; i++ ) {
		push_double(iass_unserialize_double (src + cur));
		cur += IASS_SERIALIZED_LEN_DOUBLE;
	}

	/* load cstrings */
	num_elements = iass_unserialize_uint16 (src + cur);
	cur += IASS_SERIALIZED_LEN_INT16;
	unsigned int cstrings_cur = cur + num_elements * IASS_SERIALIZED_LEN_INT16;
	for (unsigned int i = 0; i < num_elements; i++ ) {
		uint16_t cstring_len = iass_unserialize_uint16 (src + cur);
		cur += IASS_SERIALIZED_LEN_INT16;
		iass_cstring* cstring = new iass_cstring (src + cstrings_cur, cstring_len);
		push_cstring(cstring);
		cstrings_cur += cstring_len;
	}
	return cur;
}


unsigned int iass_ipcblock::unserialize_and_store (const char* src, unsigned int len) {
	unsigned int serialized_len;
	serialized_len = unserialize(src, len);
	if (serialized_len) {
		/* unserialize(src, len) checks that this ipc_block is empty,
		 * therefore _serialized is NULL when we get here */
		_serialized = new iass_cstring(src, serialized_len);
		dirty = false;
	}
	return serialized_len;
}


iass_cstring* iass_ipcblock::serialized(void) {
	if(dirty) {
		if (_serialized) {
			delete _serialized;
			_serialized = NULL;
		}
		dirty = false;
	}
	return _serialized;
}


void iass_ipcblock::clear(void) {
	int8_vector.clear();
	int16_vector.clear();
	int32_vector.clear();
	float_vector.clear();
	double_vector.clear();
	for (unsigned int i=0; i < cstrings_vector.size(); i ++) delete cstrings_vector[i];
	cstrings_vector.clear();
	if (_serialized) {
		delete _serialized;
		_serialized = NULL;
	}
	dirty = false;
}



#ifdef debug_iass_ipcblock
void iass_ipcblock::print(void) {
	std::cout << " - num int8_vector " << int8_vector.size() << "\n  ";
	for (unsigned int i=0; i< int8_vector.size(); i++)
		std::cout << i << ":" << (unsigned int) int8_vector[i] << ", ";

	std::cout << "\n - num int16_vector " << int16_vector.size() << "\n  ";
	for (unsigned int i=0; i< int16_vector.size(); i++)
		std::cout << i << ":" << int16_vector[i] << ", ";

	std::cout << "\n - num int32_vector " << int32_vector.size() << "\n  ";
	for (unsigned int i=0; i< int32_vector.size(); i++)
		std::cout << i << ":" << int32_vector[i] << ", ";

	std::cout << "\n - num float_vector " << float_vector.size() << "\n  ";
	for (unsigned int i=0; i< float_vector.size(); i++)
		std::cout << i << ":" << float_vector[i] << ", ";

	std::cout << "\n - num double_vector " << double_vector.size() << "\n  ";
	for (unsigned int i=0; i< double_vector.size(); i++)
		std::cout << i << ":" << double_vector[i] << ", ";

	std::cout << "\n - num memchunk_vector " << cstrings_vector.size() << "\n  ";
	for (unsigned int i=0; i< cstrings_vector.size(); i++) {
		std::cout << "BEGIN -";
		for (unsigned int j=0; j< cstrings_vector[i]->len; j++) 
			std::cout << j << ":" << (unsigned int) *(((char*)cstrings_vector[i]->buf)+j) << ", ";
		std::cout << "-END\n";

		std::cout << "BEGIN -";
		for (unsigned int j=0; j< cstrings_vector[i]->len; j++) 
			std::cout << j << ":" << (char ) *(((char*)cstrings_vector[i]->buf)+j) << ", ";
		std::cout << "-END\n";
	}
	std::cout << "\n";
}
#endif

