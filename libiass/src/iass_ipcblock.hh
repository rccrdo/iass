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

#ifndef iass_ipcblock_hh
#define iass_ipcblock_hh

#include <iostream>
#include <string.h>
#include <stdint.h>
#include "iass_libdebug.hh"
#include "iass_vector.hh"
#include "iass_cstring.hh"
#include "iass_serialize_helpers.hh"

#define IASS_IPC_VECTORS_MAX_ELEMENTS (unsigned int) 65000

#define IPC_BLOCK_IS_EMPTY (!(int8_vector.size() or int16_vector.size() or int32_vector.size() or \
                              float_vector.size() or double_vector.size() or cstrings_vector.size()))

class iass_ipcblock
{ 
	/* functions */
public:
	iass_ipcblock ();
	~iass_ipcblock(); 

	/* returns the length in bytes of the serialized stream for the given obj*/
	unsigned int serialized_len(void);

	/* serializes the given obj to the dest buffer
	 * ! caller must make sure dest is accessible for serialized_len() bytes
	 * returns the number of bytes used in the dest buffer */
	unsigned int serialize(char* dest);

	/* like serialize(char* dest) but also makes a copy of the serialized stream;
	 *  this is usefull when we there'is need to reuse the same obj serialized stream
	 * On success the serialized stream is stored in _serialized */
	unsigned int serialize_and_store(char* dest);

	/* unserializes the given obj described by the src buffer for a maximum of len bytes 
	 * returns true on success, false otherwise */
	unsigned int unserialize(const char* src, unsigned int len);

	/* like unserialize but also stores a copy of the serialized stream in _serialized */
	unsigned int unserialize_and_store (const char* src, unsigned int len);

	/* returns _serialized
	 * ! it can return NULL when dirty is true */
	iass_cstring* serialized(void);


	inline void push_int8 (int8_t v)  {
		assert(int8_vector.size() < IASS_IPC_VECTORS_MAX_ELEMENTS);
		dirty = true;
		int8_vector.push_back(v);   
	}

	inline void push_int16 (int16_t v) {
		assert(int16_vector.size() < IASS_IPC_VECTORS_MAX_ELEMENTS);
		dirty = true;
		int16_vector.push_back(v);  
	}

	inline void push_int32 (int32_t v) {
		assert(int32_vector.size() < IASS_IPC_VECTORS_MAX_ELEMENTS);
		dirty = true;
		int32_vector.push_back(v);
	}

	inline void push_float (float v) {
		assert(float_vector.size() < IASS_IPC_VECTORS_MAX_ELEMENTS);
		dirty = true;
		float_vector.push_back(v);
	}

	inline void push_double (double v) {
		assert(float_vector.size() < IASS_IPC_VECTORS_MAX_ELEMENTS);
		dirty = true;
		double_vector.push_back(v);
	}

	inline void push_cstring (iass_cstring* v) {
		assert(float_vector.size() < IASS_IPC_VECTORS_MAX_ELEMENTS);
		assert(v);
		dirty = true;
		cstrings_vector.push_back(v);
	}

	unsigned int int8_vector_size(void) 	{ return int8_vector.size(); }
	unsigned int int16_vector_size(void) 	{ return int16_vector.size(); }
	unsigned int int32_vector_size(void)	{ return int32_vector.size(); }
	unsigned int float_vector_size(void) 	{ return float_vector.size(); }
	unsigned int double_vector_size(void)	{ return double_vector.size(); }
	unsigned int cstrings_vector_size(void) { return cstrings_vector.size(); }

	int8_t* int8_vector_ptr(void)		{ return (int8_t*) int8_vector.data(); }
	int16_t* int16_vector_ptr(void) 	{ return (int16_t*) int16_vector.data(); }
	int32_t* int32_vector_ptr(void) 	{ return (int32_t*) int32_vector.data(); }
	float* float_vector_ptr(void) 	{ return (float*) float_vector.data(); }
	double* double_vector_ptr(void) 	{ return (double*) double_vector.data(); }
	iass_cstring** cstrings_vector_ptr(void) { return (iass_cstring**) cstrings_vector.data(); }


	/* reset the status of the ipc_block as it was newly instantieted */
	inline void clear(void);

#ifdef debug_iass_ipcblock 
	void print(void);
#endif

/* members */
public:
	/* dirty records when _serialized has a valid content or not */
	bool dirty;
	iass_cstring* _serialized;

	iass_vector<int8_t> int8_vector;
	iass_vector<int16_t> int16_vector;
	iass_vector<int32_t> int32_vector;
	iass_vector<float> float_vector;
	iass_vector<double> double_vector;
	iass_vector<iass_cstring*> cstrings_vector;
};

#endif /*iass_ipcblock_hh*/

