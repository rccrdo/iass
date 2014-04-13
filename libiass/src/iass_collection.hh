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

#ifndef iass_collection_hh
#define iass_collection_hh

#include <stdlib.h>
#include <iostream>
#include "iass_priority_queue.hh"

/* ! must be divisible per 4 
   ! larger malloc blocks are no advantage at all (riccardo)*/
#define COLLECTION_ARRAY_BLOCK_SIZE 4096

/* - WATERMARKS THRESHOLDS -
 * ! these threshold give the best results for me (riccardo)
 * ! values of COLLECTION_THRESHOLD_FRACT_DEN higher than 8 crash with sisgsev for hundred millions entries; why ?
 *    to much memory requeste for rialloc ? */
#define COLLECTION_HI_THRESHOLD_FRACT_NUM 7
#define COLLECTION_LO_THRESHOLD_FRACT_NUM 1
#define COLLECTION_THRESHOLD_FRACT_DEN 8

//#define COLLECTION_REALLOC_DEBUG

template <class T>
class iass_collection
{
/* members */
private:
	unsigned int collection_size;
	unsigned int array_len;
	unsigned int array_ext_cur;
	unsigned int array_hi_th;
	unsigned int array_low_th;
	T* array;
	iass_priority_queue<unsigned int> deleted_pq;


/* functions */
private:
	inline void update_thresholds(void) {
		array_hi_th = ((array_len / COLLECTION_THRESHOLD_FRACT_DEN) * COLLECTION_HI_THRESHOLD_FRACT_NUM);
		array_low_th = ((array_len / COLLECTION_THRESHOLD_FRACT_DEN) * COLLECTION_LO_THRESHOLD_FRACT_NUM);
	}

public:
	iass_collection() {
		/* malloc first COLLECTION_ARRAY_BLOCK_SIZE chunk of slots */
		array = (T*) malloc(COLLECTION_ARRAY_BLOCK_SIZE * sizeof(T));
		memset(array, 0, COLLECTION_ARRAY_BLOCK_SIZE * sizeof(T));

		array_len = COLLECTION_ARRAY_BLOCK_SIZE;
		collection_size = 0;
		array_ext_cur = 0;
		update_thresholds();
	}


	~iass_collection() {
		if (collection_size) 
			std::cout << "warning ~iass_collection(), still " << collection_size << " entries in this collection\n";
		/* free memory */
		deleted_pq.clear();
		free (array);
	}


	unsigned int extent(void) {
		return array_ext_cur;
	}


	T operator[](unsigned int& pos) {
		if (pos >= array_len) {
			std::cout << "warning iass_collection::operator[](unsigned int& pos), pos("
				  << pos << ") out of ranges (0," << (array_len-1) << ")\n";
			return NULL;
		}
		return array[pos];
	}


	unsigned int insert(T obj) {
		if (!obj) {
			std::cout << "warning iass_collection::insert(T* obj), obj is a NULL ptr\n";
			/* returning zero is a 'mistake', but actually nobody should push a NULL ptr here */
			return 0;
		}

		/* search a free place at the begin of the array in the queue */
		unsigned int insert_pos;
		if (deleted_pq.size()) {
			deleted_pq.remove_min(insert_pos);
			if ( !(insert_pos < array_ext_cur) ) {
				/* we didn't find a valid value for the current collection extents;
				 * as this is a priority queue, next values will just be higher: we can clear the queue */
				deleted_pq.clear();
				insert_pos = array_ext_cur;
				array_ext_cur ++;
			}
		}
		else { 
			insert_pos = array_ext_cur;
			array_ext_cur ++;
		}

		/* insert obj at insert_position */
		array[insert_pos] = obj;
		collection_size++;

		/* enlarge the array if necessary */
		if (array_ext_cur > array_hi_th ) {
#ifdef COLLECTION_REALLOC_DEBUG
			std::cout << " collection insert realloc :" << array_len << " -> ";
#endif
			/* find array_len so that after realloc we don't fall again in the array_hi_th zone */
			array_len = ((array_ext_cur * COLLECTION_THRESHOLD_FRACT_DEN)/ COLLECTION_HI_THRESHOLD_FRACT_NUM);

			/* find the smallest multiple of COLLECTION_ARRAY_BLOCK_SIZE bigger than array_len */
			array_len = (((array_len) / COLLECTION_ARRAY_BLOCK_SIZE) + 1) * COLLECTION_ARRAY_BLOCK_SIZE;
#ifdef COLLECTION_REALLOC_DEBUG
			std::cout << array_len <<"\n";
			std::cout << "   while, array_ext = " << array_ext_cur << "\n";
			std::cout << "          array_low_th = " << array_low_th << ", array_hi_th = " << array_hi_th <<"\n";
#endif
			/* realloc array */
			array = (T*) realloc(array, array_len * sizeof(T));

			/* update thresholds */
			update_thresholds();
		}
		return (array_ext_cur - 1);
	}


	T remove(unsigned int& pos) {
		if (pos>=array_ext_cur) {
			std::cout << "warning iass_collection::remove(unsigned int& pos), pos("
				  << pos  << "), out of valid ranges (0," << (array_len - 1) << "\n";
			return NULL;
		}

		T obj = array[pos];
		array[pos] = NULL;
		collection_size--;
		deleted_pq.insert(pos);

		/* shrink array when in the last positions are stored only NULL ptrs */
		if (pos == (array_ext_cur - 1) ) {
			for (unsigned int j = (array_ext_cur -1); j>=0 ; j-- ) {
				if (array[j] == NULL) array_ext_cur --;
				else break;
			}
		}

		/* try to shrink array using the low watermark rule */
		if ( array_ext_cur < array_low_th ) {
			/* realloc a at least half-empty queue */
			unsigned int new_len = collection_size*2;
			if (new_len < array_ext_cur) new_len = array_ext_cur;

			/* find the smallest multiple of COLLECTION_ARRAY_BLOCK_SIZE bigger than new_len  */
			new_len = (((new_len) / COLLECTION_ARRAY_BLOCK_SIZE) + 1) * COLLECTION_ARRAY_BLOCK_SIZE;

#ifdef COLLECTION_REALLOC_DEBUG
			std::cout << " collection remove realloc (size " << collection_size << ") :" << array_len << " -> " << new_len << "\n";
			std::cout << "   while, array_ext_cur = " << array_ext_cur << "\n";
			std::cout << "          array_low_th = " << array_low_th << ", array_hi_th = " << array_hi_th <<"\n";
#endif
			array_len = new_len;
			array = (T*) realloc(array, array_len * sizeof(T) );

			/* update thresholds */
			update_thresholds();
		}
		return obj;
	}


};

#endif /*iass_collection_hh*/

