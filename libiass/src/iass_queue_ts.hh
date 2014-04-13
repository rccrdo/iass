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

#ifndef iass_queue_ts_hh
#define iass_queue_ts_hh

#include <string.h>
#include <iostream>
#include "iass_libdebug.hh"
#include <ace/Thread_Mutex.h>

/* ! must be divisible per 4 
   ! larger malloc blocks are no advantage at all (riccardo)*/
#define QUEUE_ARRAY_BLOCK_SIZE 4096

/* - WATERMARKS THRESHOLDS -
 * ! these threshold give the best results for me (riccardo)
 * ! values of QUEUE_THRESHOLD_FRACT_DEN higher than 8 crash with sisgsev for hundred millions entris; why ?
 to much memory requeste for rialloc ?
*/
#define QUEUE_HI_THRESHOLD_FRACT_NUM 7
#define QUEUE_LO_THRESHOLD_FRACT_NUM 1
#define QUEUE_THRESHOLD_FRACT_DEN 8

//#define QUEUE_REALLOC_DEBUG

template <class T>
class iass_queue_ts {

/* members */
private:
	T* array;
	unsigned int queue_size;
	unsigned int array_len;
	unsigned int array_left_cur;
	unsigned int array_right_cur;
	unsigned int array_hi_th;
	unsigned int array_low_th;
	ACE_Thread_Mutex mutex;


/* functions */
public:
	iass_queue_ts() {
		/* malloc first QUEUE_ARRAY_BLOCK_SIZE chunk */
		array = (T*) malloc(QUEUE_ARRAY_BLOCK_SIZE * sizeof(T));

		queue_size = 0;
		array_len = QUEUE_ARRAY_BLOCK_SIZE;
		array_left_cur = 0;
		array_right_cur = 0;
		array_hi_th = (array_len / QUEUE_THRESHOLD_FRACT_DEN) * QUEUE_HI_THRESHOLD_FRACT_NUM;
		array_low_th = (array_len / QUEUE_THRESHOLD_FRACT_DEN) * QUEUE_LO_THRESHOLD_FRACT_NUM;
	}


	~iass_queue_ts() {
		mutex.acquire();
		if (queue_size) std::cout << "warning ~iass_queue_ts(), still " << queue_size << " entries in queue\n";

		/* free memory */
		free(array);
		mutex.release();
	}


	inline unsigned int size(void) {
		mutex.acquire();
		return queue_size;
		mutex.release();
	}


	void push (const T* buf, unsigned int len) {
		assert(buf);
		assert(len);    

		mutex.acquire();
		unsigned int new_extents = array_right_cur + len;
		if (new_extents > array_hi_th ) {

#ifdef QUEUE_REALLOC_DEBUG
			std::cout << " push realloc :" << array_len << " -> ";
#endif

			/* find array_len so that after realloc we don't fall again over array_hi_th */
			array_len = ((new_extents * QUEUE_THRESHOLD_FRACT_DEN)/ QUEUE_HI_THRESHOLD_FRACT_NUM);

			/* find the smallest multiple of QUEUE_ARRAY_BLOCK_SIZE bigger than array_len */
			array_len = (((array_len) / QUEUE_ARRAY_BLOCK_SIZE) + 1) * QUEUE_ARRAY_BLOCK_SIZE;

#ifdef QUEUE_REALLOC_DEBUG
			std::cout << array_len <<"\n";
			std::cout << "   while, array_left_cur = " << array_left_cur << ", array_right_cur = " << array_right_cur <<"\n";
			std::cout << "          array_low_th = " << array_low_th << ", array_hi_th = " << array_hi_th <<"\n";
#endif

			array = (T*) realloc(array, array_len * sizeof(T));

			/* update thresholds */
			array_hi_th = ((array_len / QUEUE_THRESHOLD_FRACT_DEN) * QUEUE_HI_THRESHOLD_FRACT_NUM);
			array_low_th = ((array_len / QUEUE_THRESHOLD_FRACT_DEN) * QUEUE_LO_THRESHOLD_FRACT_NUM);
		}

		/* copy (*buf) contents for len bytes int the tail of the queue */
		memcpy(array + array_right_cur, buf, len * sizeof(T));
		queue_size += len;
		array_right_cur = new_extents;

		mutex.release();
	}


	void pop(unsigned int len) {
//    assert(len);
#ifdef debug_iass_queue_ts
		if(!len) {
			std::cout << "warning iass_queue_ts::pop(unsigned int len), len is zero\n";
		}
#endif

		mutex.acquire();

		if ( len <= queue_size ) {
			array_left_cur += len;
			queue_size -= len;

			if ((queue_size < array_low_th) and 
			    (array_left_cur > array_low_th) and 
			    (array_left_cur > QUEUE_ARRAY_BLOCK_SIZE) ) {

				memmove(array, array + array_left_cur, queue_size * sizeof(T));

				/* update cursors */
				array_right_cur -= array_left_cur;
				array_left_cur = 0;

#ifdef QUEUE_REALLOC_DEBUG
				std::cout << " pop realloc :" << array_len << " -> ";
#endif

				array_len = (queue_size*2); /* realloc a at least half-empty queue */

				/* find the smallest multiple of QUEUE_ARRAY_BLOCK_SIZE bigger than array_len */
				array_len = (((array_len) / QUEUE_ARRAY_BLOCK_SIZE) + 1) * QUEUE_ARRAY_BLOCK_SIZE;

#ifdef QUEUE_REALLOC_DEBUG
				std::cout << (array_len) << "\n";
				std::cout << "   while, array_left_cur = " << array_left_cur << ", array_right_cur = " << array_right_cur <<"\n";
				std::cout << "          array_low_th = " << array_low_th << ", array_hi_th = " << array_hi_th <<"\n";
#endif
         
				array = (T*) realloc(array, array_len * sizeof(T));

				/* update thresholds */
				array_hi_th = ((array_len / QUEUE_THRESHOLD_FRACT_DEN) * QUEUE_HI_THRESHOLD_FRACT_NUM);
				array_low_th = ((array_len / QUEUE_THRESHOLD_FRACT_DEN) * QUEUE_LO_THRESHOLD_FRACT_NUM);
			}

			mutex.release();
			return;
		}
		else if (queue_size!=0) {
			queue_size = 0;
			array_len = QUEUE_ARRAY_BLOCK_SIZE;
			array_left_cur = 0;
			array_right_cur = 0;
			array_hi_th = (array_len / QUEUE_THRESHOLD_FRACT_DEN) * QUEUE_HI_THRESHOLD_FRACT_NUM;
			array_low_th = (array_len / QUEUE_THRESHOLD_FRACT_DEN) * QUEUE_LO_THRESHOLD_FRACT_NUM;

			array = (T*) realloc(array, array_len * sizeof(T));

			mutex.release();
			return;
		}

		/* array_right_cur is pointing to next push position, if array_right_cur
		 *  and array_left_cur are equal than array_left_cur is an invalid position to pop from,
		 *  better said: queue size is 0 */
		mutex.release();
		return;
	}


	unsigned int top(T* buf, unsigned int len) {
		assert(buf);

		mutex.acquire();

		if (queue_size < len) len = queue_size;
		memcpy(buf, array + array_left_cur, len * sizeof(T));
		mutex.release();
		return len;
	}
};


#endif /*iass_queue_ts_hh*/
