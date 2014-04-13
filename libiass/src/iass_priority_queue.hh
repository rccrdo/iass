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

#ifndef iass_priority_queue_hh
#define iass_priority_queue_hh

#include <iostream>
#include <stdlib.h>

/* ! must be divisible per 4 
 *  ! it's ok to have a large value as memory will be 'really allocated' only when reading or writing to it */
#define PRIORITY_QUEUE_BLOCK_SIZE 4096

/* - WATERMARKS THRESHOLDS -
 * ! these threshold give the best results for me (riccardo)
 * ! values of PRIORITY_QUEUE_THRESHOLD_FRACT_DEN higher than 8 crash with sisgsev for hundred millions entries; why ?
 to much memory requeste for rialloc ? */
#define PRIORITY_QUEUE_HI_THRESHOLD_FRACT_NUM 7
#define PRIORITY_QUEUE_LO_THRESHOLD_FRACT_NUM 1
#define PRIORITY_QUEUE_THRESHOLD_FRACT_DEN 8

//#define PRIORITY_QUEUE_REALLOC_DEBUG

template <typename T>
class iass_priority_queue
{
/* members */
private:
	T* heap;
	unsigned int heap_size;
	unsigned int array_len;
	unsigned int array_hi_th;
	unsigned int array_low_th;
  
/* functions */
public:
	iass_priority_queue () {
		heap = (T*) malloc( PRIORITY_QUEUE_BLOCK_SIZE * sizeof(T) );

		heap_size = 0;
		array_len = PRIORITY_QUEUE_BLOCK_SIZE;

		array_hi_th = (array_len / PRIORITY_QUEUE_THRESHOLD_FRACT_DEN) * PRIORITY_QUEUE_HI_THRESHOLD_FRACT_NUM;
		array_low_th = (array_len / PRIORITY_QUEUE_THRESHOLD_FRACT_DEN) * PRIORITY_QUEUE_LO_THRESHOLD_FRACT_NUM;
	};


	~iass_priority_queue () {
		if (heap_size) std::cout << "warning ~iass_priority_queue(), still " << heap_size << "keys stored in the queue\n";
  
		/* free memory */
		free(heap);
	};


	bool min(unsigned int& value) {
		if (heap_size == 0) return false;
		value = heap[1];
		return true;
	}


private:
	inline void up_heap(void) {
		unsigned int cur = heap_size;
		unsigned int parent;
		while (cur!=1) {
			parent = cur/2;
			if (heap[parent] <= heap[cur]) break;

			/* swap */
			unsigned int temp = heap[parent];
			heap[parent] = heap[cur];
			heap[cur] = temp;

			cur = parent; 
		}
	}


	inline void down_heap(void) {
		unsigned int cur = 1; /* root */
		unsigned int child;
		unsigned int dcur = 2; /* cur*2 */

		while (/*is_internal(cur)*/ (dcur ) <= heap_size ) {
			if (! (/*has_right(cur)*/ (dcur + 1) <= heap_size) ) child = dcur;
			else if ( heap[dcur] <= heap[(dcur)+1] ) child = dcur;
			else child = ((dcur)+1);

			if (heap[child] < heap[cur]) {

				/* swap */
				unsigned int temp = heap[cur];
				heap[cur] = heap[child];
				heap[child] = temp;
   
				cur = child;
				dcur = cur*2;
			}
			else break;
		}
	}


public:
	void insert(unsigned int& value) {
		heap_size++;
		heap[heap_size] = value;
		up_heap();


		if (heap_size > array_hi_th ) {
#ifdef PRIORITY_QUEUE_REALLOC_DEBUG
			std::cout << " priority queue insert realloc :" << array_len << " -> ";
#endif

			/* find array_len so that after realloc we don't fall again under the array_hi_th zone */
			array_len = ((heap_size * PRIORITY_QUEUE_THRESHOLD_FRACT_DEN)/ PRIORITY_QUEUE_HI_THRESHOLD_FRACT_NUM);

			/* find the smallest multiple of QUEUE_ARRAY_BLOCK_SIZE bigger than array_len */
			array_len = (((array_len) / PRIORITY_QUEUE_BLOCK_SIZE) + 1) * PRIORITY_QUEUE_BLOCK_SIZE;

#ifdef PRIORITY_QUEUE_REALLOC_DEBUG
			std::cout << array_len <<"\n";
			std::cout << "   while, heap_size = " << heap_size << "\n";
			std::cout << "          array_low_th = " << array_low_th << ", array_hi_th = " << array_hi_th <<"\n";
#endif

			heap = (T*) realloc(heap, array_len * sizeof(T));

			/* update thresholds */
			array_hi_th = ((array_len / PRIORITY_QUEUE_THRESHOLD_FRACT_DEN) * PRIORITY_QUEUE_HI_THRESHOLD_FRACT_NUM);
			array_low_th = ((array_len / PRIORITY_QUEUE_THRESHOLD_FRACT_DEN) * PRIORITY_QUEUE_LO_THRESHOLD_FRACT_NUM);
		}
	}
  

	bool remove_min(unsigned int& value) {
		if (heap_size == 0) return false;

		/* root -> value */
		value = heap[1];

		/* last -> root */
		heap[1] = heap[heap_size];
		heap_size--;
		down_heap();

		if ( ( heap_size < array_low_th ) and (heap_size > PRIORITY_QUEUE_BLOCK_SIZE) ) {

			array_len = ((array_len / PRIORITY_QUEUE_BLOCK_SIZE)/ 2) * PRIORITY_QUEUE_BLOCK_SIZE;
			heap = (unsigned int*) realloc(heap, array_len*sizeof(unsigned int));
			array_low_th = (array_len / 4);
			array_hi_th = (array_low_th * 3);

#ifdef PRIORITY_QUEUE_REALLOC_DEBUG
			std::cout << " priority queue remove realloc :" << array_len << " -> ";
#endif

			/* realloc a at least half-empty queue */
			array_len = (heap_size*2);

			/* find the smallest multiple of PRIORITY_QUEUE__BLOCK_SIZE bigger than array_len */
			array_len = (((array_len) / PRIORITY_QUEUE_BLOCK_SIZE) + 1) * PRIORITY_QUEUE_BLOCK_SIZE;

#ifdef PRIORITY_QUEUE_REALLOC_DEBUG
			std::cout << (array_len) << "\n";
			std::cout << "   while, heap_size = " << heap_size << "\n";
			std::cout << "          array_low_th = " << array_low_th << ", array_hi_th = " << array_hi_th <<"\n";
#endif

			heap = (T*) realloc(heap, array_len * sizeof(T) );

			/* update thresholds */
			array_hi_th = ((array_len / PRIORITY_QUEUE_THRESHOLD_FRACT_DEN) * PRIORITY_QUEUE_HI_THRESHOLD_FRACT_NUM);
			array_low_th = ((array_len / PRIORITY_QUEUE_THRESHOLD_FRACT_DEN) * PRIORITY_QUEUE_LO_THRESHOLD_FRACT_NUM);
		}
		return true;
	}


	void clear() {
		array_len = PRIORITY_QUEUE_BLOCK_SIZE;

		/* update thresholds */
		array_hi_th = ((array_len / PRIORITY_QUEUE_THRESHOLD_FRACT_DEN) * PRIORITY_QUEUE_HI_THRESHOLD_FRACT_NUM);
		array_low_th = ((array_len / PRIORITY_QUEUE_THRESHOLD_FRACT_DEN) * PRIORITY_QUEUE_LO_THRESHOLD_FRACT_NUM);

		heap_size = 0;
		heap = (unsigned int*) realloc(heap, array_len * sizeof(T));
	}


	unsigned int size() {
		return heap_size;
	}

//  void print() {
//    std::cout << "heap print, size " << heap_size << "\n"; 
//    for (unsigned int i = 1 /*position 0 isn't used, see algorithm */ ; i < heap_size; i++ ) {
//      std::cout << heap[i] << ","; 
//    }
//    std::cout << "\n\n";
//  }


};

#endif /*iass_priority_queue_hh*/

