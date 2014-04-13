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

#ifndef iass_ipctunnel_hh
#define iass_ipctunnel_hh

#include <iostream>
#include "string.h"
#include "iass_libdebug.hh"
#include "iass_queue.hh"
#include "iass_loop_sleeper.hh"
#include "iass_packet.hh"
#include "idls/iass_tunnel_idlS.h"
#include "iass_ipcblock.hh"
#include "iass_queue_ts.hh"
#include <ace/Thread_Mutex.h>
#include <ace/Thread_Manager.h>

#define IPC_TUNNEL_MAINLOOP_FREQUENCY 100 /* herz [Hz] */
#define IPC_TUNNEL_BUFFERS_LEN 1000000
#define IPC_TUNNEL_INCOMING_QUEUE_MAX_LEN 10*IPC_TUNNEL_BUFFERS_LEN

struct iass_ipctunnel_data {
	/* to acquire/release when accessing any of the followings but ipc_data_queue*/
	ACE_Thread_Mutex mutex;		

	bool is_running;
	bool keep_running;
	iass_queue<char> incoming_queue;
	iass_queue_ts<iass_ipcblock*> ipc_data_queue;
};


class iass_ipctunnel : public POA_iass_tunnel_idl {
/* functions */
public:
	iass_ipctunnel();

	/* pop iass_ipcblock structures created from incoming streams 
	 * ! thread safe call */
	unsigned int pull(iass_ipcblock** buf, unsigned int buf_len);

	void run_separate_thread(void);
	bool is_running(void);
	void stop(void);

private:
	static void* internal_run_separate_thread(void* args);

/* CORBA rpc methods */
public:
	void push(const iass_corba_raw_stream& stream) throw();

/* members */
private:
	iass_ipctunnel_data tunnel_data;
};

#endif /*iass_ipctunnel_hh*/

