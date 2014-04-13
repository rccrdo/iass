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

#ifndef iass_ipctunnel_client_hh
#define iass_ipctunnel_client_hh

#include <iostream>
#include "string.h"
#include "iass_libdebug.hh"
#include "iass_queue.hh"
#include "iass_packet.hh"
#include "iass_ipcblock.hh"
#include "iass_loop_sleeper.hh"
#include "idls/iass_tunnel_idlC.h"
#include <ace/Thread_Mutex.h>
#include <ace/Thread_Manager.h>

/** \def IPC_TUNNEL_CLIENT_MAINLOOP_FREQUENCY
 *   \brief Looping frequency for the internal thread
 */
#define IPC_TUNNEL_CLIENT_MAINLOOP_FREQUENCY 100 /* herz [Hz] */

/** \def IPC_TUNNEL_CLIENT__BUFFERS_LEN
 *   \brief Length in bytes of the buffers used internally by a iass_ipctunnel_client obj
 */
#define IPC_TUNNEL_CLIENT__BUFFERS_LEN 1000000

/** \def IPC_TUNNEL_CLIENT_INCOMING_QUEUE_MAX_LEN
 *   \brief Max acceptable length of the incoming queue before it  being partially discarded
 *
 *  This is usefull on high load cases to avoid eating all pc's resources. The downside
 *  is that a part of the information to be sended get's lost. Actually this situation
 *  shuould never be triggered
 */
#define IPC_TUNNEL_CLIENT_INCOMING_QUEUE_MAX_LEN 10000


/** class iass_ipctunnel_client
 *  \brief Helper class used push iassp_ipc_blocks to a tunnel (taking care of serializing them into packets)
 */
class iass_ipctunnel_client {
/* functions */
public:
	/**
	 *  \brief Constructor
	 *  \param dest tunnel whom to push serialized data
	 */
	iass_ipctunnel_client(iass_tunnel_idl* dest);

	/**
	 *  \brief Destructor
	 *
	 *   This method also takes care of stopping the internal thread if it's running.
	 *   Default behaviour is to release the iass_tunnel_idl obj passed to the constructor
	 */
	~iass_ipctunnel_client();

	/**
	 *  \brief Method to push valid iass_ipcblock to the outgoing working queue
	 *  \param buf Array in wich are stored ptrs to the struct to serialize
	 *  \param len Length of the array buf
	 */
	void push(iass_ipcblock** buf, unsigned int len);


	/**
	 *  \brief Helper method that spawns a new thread for working on the incoming queue
	 *
	 *  If the thread was already spawned, simply returns
	 */
	void run_separate_thread(void);

	/**
	 *  \brief Get the running state of the workqueue thread
	 */
	bool is_running(void);

	/**
	 *  \brief Stop the workqueue thread
	 */
	void stop(void);

private:
	/**
	 *  \brief Helper methos used as lacun function for the workqueue thread
	 *  \param args A ptr of type iass_ipctunnel_client* (pointing to the same obj, the the one the made the call
	 */
	static void* internal_run_separate_thread(void* args);

/* members */
private:
	/**
	 *  Mutex to acquire when processing the incoming or outgoing queue
	 */
	ACE_Thread_Mutex mutex;		

	/**
	 *  Store the state of the internal thread
	 */
	bool _is_running;

	/**
	 *  On a Stop request is set to true to say to the internal thread to exit
	 */
	bool keep_running;

	/**
	 *  Incoming queue of ptr to valid iass_ipcblock structs
	 */
	iass_queue<iass_ipcblock*> incoming_queue;

	/**
	 *  The tunnel serialized data is pushed to
	 */
	iass_tunnel_idl* tunnel;
};

#endif /*iass_ipctunnel_client_hh*/
