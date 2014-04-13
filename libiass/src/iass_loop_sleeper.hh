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

#ifndef iass_loop_sleeper_hh
#define iass_loop_sleeper_hh

#include <sys/time.h>
#include <iostream>

#include <iostream>
#include <sys/time.h>


/* inline unsigned int iass_utils_time_passed(timeval& old, timeval& current); */

class iass_loop_sleeper
{
	/* members */
private:
	unsigned int frequency;
	unsigned int sleep_amount;
	timeval last_time;
	timeval current_time;
	unsigned int ticks;

/* functions */
public:
	iass_loop_sleeper(unsigned int freq) {
		if ( freq <= 1 ) {
			std::cout << "warning iass_loop_sleeper(unsigned int freq), freq is " << freq << ", setting to 2\n";
			freq=2;
		} else if ( freq > 1000 ) {
			std::cout << "warning iass_loop_sleeper(unsigned int freq), freq is " << freq << ", it's very difficult to reliably follow frequencies higher then 1-2KHz\n";
		}

		/* get current time day */
		gettimeofday(&last_time, NULL);

		/* usleep(1000000) sleeps for 1 sec and therefore: */
		sleep_amount = (unsigned int) (1000000/freq);
		ticks = 0;
		frequency = freq;
	}


	void sleep(void) {
		ticks++;
		/* get current time */
		gettimeofday(&current_time, NULL);
    
		/* check if one second is passed since when we grabbed last_time value */
		if (current_time.tv_sec > last_time.tv_sec) {
			if (current_time.tv_sec==(last_time.tv_sec+1)) {
				sleep_amount = ((sleep_amount * ticks) / frequency);   
				// avoid sleep_amount becoming 0 or blocking to too low values
				if (sleep_amount==0) sleep_amount = 50; /* 20 KHz */
			}
			else std::cout << "warning iass_loop_sleeper::sleep(void), not able to follow frequency(" << frequency <<"Hz)\n";

			ticks = 0;
		}

		// make new value the last value for the next call 
		last_time.tv_sec = current_time.tv_sec;
		last_time.tv_usec = current_time.tv_usec;

		/* finally sleep */
		usleep(sleep_amount);
	}

};


/*inline unsigned int iass_utils_time_passed(timeval& old, timeval& current) {
  return ( (1000000 * (current.tv_sec - old.tv_sec) + current.tv_usec) - old.tv_usec);
  }*/


#endif /*iass_loop_sleeper_hh*/

