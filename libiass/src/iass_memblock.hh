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

#ifndef iass_memblock_hh
#define iass_memblock_hh

#include <string.h>
#include <iostream>
#include "iass_libdebug.hh"

/** \class iass_memblock
 *  \brief Template class representing an array of obj of type T
 *
 *  Helper methods make it very easy to work with it.
 *  This class will work only with ptr to types or basic type (char, int, double...)
 */
template <class T>
class iass_memblock
{
/* functions */
public:
	/**
	 * Builds a iass_memblock copying contents from a given memory chunk
	 * @param _buf ptr to the begin of the buffer to copy
	 * @param _len number of elements to copy (the number of bytes would by (sizeof(T)*len))
	 */
	iass_memblock(const T* _buf, unsigned int _len) {
		buf = NULL;
		assert(_buf);
		assert(_len);
		reserved = _len;
		len = _len;
		buf = (char*) malloc(reserved * sizeof(T));
		assign(_buf, _len);
	}


	/**
	 * Builds a iass_memblock simply malloc'ing the given amount of bytes
	 * @param r amount of elements to be reserved for the internal buffer (the number of bytes is (sizeof(T)*r))
	 */
	iass_memblock(unsigned int r) {
		buf = NULL;
		len = 0;
		assert(r);
		reserved = r;
		buf = (T*) malloc(reserved * sizeof(T));
	}


	/**
	 * Builds a iass_memblock like in iass_memblock(unsigned int r), but also memsets malloc'ed memory to the value of c
	 * @param r amount of elements to be reserved for the internal buffer (the number of bytes is (sizeof(T)*r))
	 * @param c value to use for memsetting the malloc'ed memory
	 */
	iass_memblock(unsigned int r, unsigned char c) {
		iass_memblock((unsigned int) r);
		memset (buf, c, reserved*sizeof(T));
	}


	/**
	 * destructor which takes care of freeing the memory pointed by mem
	 */
	~iass_memblock() {
		if (buf) free (buf);
	}


	/**
	 * assigns(copies) the memory chunk pointed by _buf for len elements to the internal buffer
	 * @param _buf ptr to the memory chunk to copy
	 * @param len number of elements to copy (the number of bytes is (sizeof(T)*r))
	 *
	 * Implementation takes care of the memory reallocation when necessary
	 */
	inline void assign(const T* _buf, unsigned int len) {
		assert(_buf);
		assert(len);
		/* check if realloc is necessary */
		if (len > reserved) reserve(len);
		memcpy(buf, _buf, len*sizeof(T));
		len = len;
	}


	/**
	 * reallocs the internal buffer to the given number of bytes
	 * @param r number of bytes to reserve
	 *
	 * When the len length of the internal buffer is bigger of the one to reserve, the len member is set
	 * to the value reserved
	 */
	inline void reserve (unsigned int r) {
		assert(r);
		reserved = r;
		buf = (T*) realloc(buf, reserved*sizeof(T));
		if (len > reserved) len = reserved;
	}


	/**
	 * Sets the whole reserved memory chunk to the given value (byte per byte)
	 * @param c value to use for memset
	 *
	 * The member len is resetted to 0
	 */
	void set (unsigned char c) {
		len = 0;
		memset (buf, c, reserved*sizeof(T));
	}
 
/* members */
public:
	T* buf;
	unsigned int len;
	unsigned int reserved;
};

#endif /*iass_memblock_hh_*/
