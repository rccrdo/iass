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

#ifndef iass_ts_renderer_queue_hh
#define iass_ts_renderer_queue_hh

#include <queue>
#include <iostream>
#include "iass_renderer_queue_data.hh"
#include <ace/Thread_Mutex.h>


class iass_ts_renderer_queue
{
/* functions */
public:

	void push(iass_renderer_queue_data* d);
	iass_renderer_queue_data* pop(void);


/* members */
private:
	std::queue<iass_renderer_queue_data*> queue;
	ACE_Thread_Mutex access_mutex;
};

#endif /*iass_ts_renderer_queue_hh*/

