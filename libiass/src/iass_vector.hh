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


#ifndef iass_vector_hh
#define iass_vector_hh

#include <stdlib.h>
#include <iostream>
#include "iass_libdebug.hh"

/* ! both must be divisible per 4 */
#define VECTOR_ARRAY_BLOCK_MIN_SIZE 32
#define VECTOR_ARRAY_BLOCK_MAX_SIZE 4096

/* - WATERMARKS THRESHOLDS -
 * ! these threshold give the best results for me (riccardo)
 * ! values of VECTOR_THRESHOLD_FRACT_DEN higher than 8 crash with sisgsev for hundred millions entries; why ?
 *    to much memory requeste for rialloc ? */
#define VECTOR_HI_THRESHOLD_FRACT_NUM 7
//#define VECTOR_LO_THRESHOLD_FRACT_NUM 1
#define VECTOR_THRESHOLD_FRACT_DEN 8

//#define VECTOR_REALLOC_DEBUG

template <class T>
class iass_vector
{
/* members */
private:
	unsigned int vector_size;
	unsigned int array_len;
	unsigned int array_hi_th;
	T* array;


/* functions */
private:
	inline void update_thresholds(void) {
		array_hi_th = ((array_len / VECTOR_THRESHOLD_FRACT_DEN) * VECTOR_HI_THRESHOLD_FRACT_NUM);
	}

public:
	iass_vector() {
		/* malloc first VECTOR_ARRAY_BLOCK_MIN_SIZE chunk of slots */
		array = (T*) malloc(VECTOR_ARRAY_BLOCK_MIN_SIZE * sizeof(T));
		memset(array, 0, VECTOR_ARRAY_BLOCK_MIN_SIZE * sizeof(T));

		array_len = VECTOR_ARRAY_BLOCK_MIN_SIZE;
		vector_size = 0;
		update_thresholds();
	}


	~iass_vector() {
		if (vector_size) 
			std::cout << "warning ~iass_vector(), still " << vector_size << " entries in this vector\n";
		/* free memory */
		free (array);
	}


	unsigned int size(void) {
		return vector_size;
	}


	T* data(void) {
		return array;
	}


	T operator[](unsigned int& pos) {
		assert(pos < array_len);
		return array[pos];
	}


	void push_back(T obj) {
		/* insert obj at insert_position */
		array[vector_size] = obj;
		vector_size++;

		/* enlarge the array if necessary */
		if (vector_size > array_hi_th ) {
#ifdef VECTOR_REALLOC_DEBUG
			std::cout << " vector insert realloc :" << array_len << " -> ";
#endif
			unsigned int new_block_size = vector_size/2;
			if (new_block_size < VECTOR_ARRAY_BLOCK_MIN_SIZE) new_block_size = VECTOR_ARRAY_BLOCK_MIN_SIZE;
			else if (new_block_size > VECTOR_ARRAY_BLOCK_MAX_SIZE) new_block_size = VECTOR_ARRAY_BLOCK_MAX_SIZE;

			/* find array_len so that after realloc we don't fall again in the array_hi_th zone */
			array_len += new_block_size;

#ifdef VECTOR_REALLOC_DEBUG
			std::cout << array_len <<"\n";
			std::cout << "   while, vector_size = " << vector_size << ", array_hi_th = " << array_hi_th <<"\n";
#endif
			/* realloc array */
			array = (T*) realloc(array, array_len * sizeof(T));

			/* update thresholds */
			update_thresholds();
		}
	}


	void clear () {
		/* malloc a VECTOR_ARRAY_BLOCK_MIN_SIZE chunk of slots */
		if (array_len > VECTOR_ARRAY_BLOCK_MIN_SIZE) {
#ifdef VECTOR_REALLOC_DEBUG
			std::cout << " clear_realloc \n";
#endif
			array = (T*) realloc(array, VECTOR_ARRAY_BLOCK_MIN_SIZE * sizeof(T));
			memset(array, 0, VECTOR_ARRAY_BLOCK_MIN_SIZE * sizeof(T));

			array_len = VECTOR_ARRAY_BLOCK_MIN_SIZE;
			update_thresholds();
		}
		vector_size = 0;
	}


};

#endif /*iass_vector_hh*/

