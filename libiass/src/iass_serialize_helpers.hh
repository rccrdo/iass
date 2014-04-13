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

#ifndef iass_serialize_helpers_hh
#define iass_serialize_helpers_hh

#include <string.h>
#include <iostream>
#include <stdint.h>
#include "iass_libdebug.hh"

#define IASS_SERIALIZED_LEN_INT8    1
#define IASS_SERIALIZED_LEN_INT16   2
#define IASS_SERIALIZED_LEN_UINT16  IASS_SERIALIZED_LEN_INT16
#define IASS_SERIALIZED_LEN_INT32   4
#define IASS_SERIALIZED_LEN_UINT32  IASS_SERIALIZED_LEN_INT32

/* float and double don't change between machine archs */
#define IASS_SERIALIZED_LEN_FLOAT   sizeof(float)
#define IASS_SERIALIZED_LEN_DOUBLE  sizeof(double)

/* serializers */ 
inline void iass_serialize_int8    (char* dest, int8_t v);
inline void iass_serialize_uint8   (char* dest, uint8_t v);

inline void iass_serialize_int16   (char* dest, int16_t v);
inline void iass_serialize_uint16  (char* dest, uint16_t v);

inline void iass_serialize_int32   (char* dest, int32_t v);
inline void iass_serialize_uint32  (char* dest, uint32_t v);

inline void iass_serialize_float   (char* dest, float v);;
inline void iass_serialize_double  (char* dest, double v);

/* unserializers */
inline int8_t iass_unserialize_int8     (const char* src);
inline uint8_t iass_unserialize_uint8   (const char* src);

inline int16_t iass_unserialize_int16   (const char* src);
inline uint16_t iass_unserialize_uint16 (const char* src);

inline int32_t iass_unserialize_int32   (const char* src);
inline uint32_t iass_unserialize_uint32 (const char* src);

inline float iass_unserialize_float     (const char* src);
inline double iass_unserialize_double   (const char* src);


/* serializers */
void iass_serialize_int8 (char* dest, int8_t v) {
	assert(dest);
	*dest = v;
}


void iass_serialize_uint8 (char* dest, uint8_t v) {
	assert(dest);
	*dest = v;
}


void iass_serialize_int16 (char* dest, int16_t v) {
	assert(dest);
	memcpy(dest, &v, IASS_SERIALIZED_LEN_INT16);
}


void iass_serialize_uint32 (char* dest, uint32_t v) {
	assert(dest);
	memcpy(dest, &v, IASS_SERIALIZED_LEN_INT32);
}


void iass_serialize_uint16 (char* dest, uint16_t v) {
	assert(dest);
	memcpy(dest, &v, IASS_SERIALIZED_LEN_INT16);
}


void iass_serialize_int32 (char* dest, int32_t v) {
	assert(dest);
	memcpy(dest, &v, IASS_SERIALIZED_LEN_INT32);
}


void iass_serialize_float (char* dest, float v) {
	assert(dest);
	memcpy(dest, &v, IASS_SERIALIZED_LEN_FLOAT);
}


void iass_serialize_double (char* dest, double v) {
	assert(dest);
	memcpy(dest, &v, IASS_SERIALIZED_LEN_DOUBLE);
}





/* unserializers */
int8_t iass_unserialize_int8 (const char* src) {
	assert(src);
	return *src;
}


uint8_t iass_unserialize_uint8 (const char* src) {
	assert(src);
	return *src;
}


uint16_t iass_unserialize_uint16 (const char* src) {
	assert(src);
	uint16_t temp;
	memcpy(&temp, src, IASS_SERIALIZED_LEN_INT16);
	return temp;
}


uint32_t iass_unserialize_uint32 (const char* src) {
	assert(src);
	uint32_t temp;
	memcpy(&temp, src, IASS_SERIALIZED_LEN_INT32);
	return temp;
}


int16_t iass_unserialize_int16 (const char* src) {
	assert(src);
	int16_t temp;
	memcpy(&temp, src, IASS_SERIALIZED_LEN_INT16);
	return temp;
}


int32_t iass_unserialize_int32 (const char* src) {
	assert(src);
	int32_t temp;
	memcpy(&temp, src, IASS_SERIALIZED_LEN_INT32);
	return temp;
}


float iass_unserialize_float (const char* src) {
	assert(src);
	float temp;
	memcpy(&temp, src, IASS_SERIALIZED_LEN_FLOAT);
	return temp;
}


double iass_unserialize_double (const char* src) {
	assert(src);
	double temp;
	memcpy(&temp, src, IASS_SERIALIZED_LEN_DOUBLE);
	return temp;
}

#endif /*iass_serialize_helpers_hh*/

