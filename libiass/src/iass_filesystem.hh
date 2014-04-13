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

#ifndef iass_filesystem_hh
#define iass_filesystem_hh

#include <stddef.h>
#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <map>
#include <vector>
#include <string>
#include <iostream>
#include <sstream>
#include "iass_configs.hh"
#include "iass_entity_ref.hh"
#include <ace/Thread_Mutex.h>
#include "iass_sharedfile_data.hh"


/* max size in bytes for the buffer used to copy files
 * ! big values behave often bad here */
#define FILECOPY_BUFFER_SIZE 10000

class iass_filesystem
{
/* functions */
public:
	/* get singletone instance */
	static iass_filesystem* instance();

	/* make the file shared by copying it to the nfs shared filesystem
	 * - if no error happens, return value is 0 and, abs_shared_path will point will contain the shared path for requested file;
	 *     abs_shared_path memory must be deleted by caller
	 * - on error return value is <0 and abs_shared_path remains a NULL ptr
	 * ! iass_cstring*& abs_shared_path must be a NULL ptr
	 * ! abs_file_path must be an absolute path
	 * ! abs_shared_path when valid is an absolute path
	 */
	iass_sharedfile_data* make_shared(const char* path);

	/* check xml file to be a known type */
	iass_file_type get_file_type(const char* path);



private:
	iass_filesystem();
	int copy_file(const char* source, const char* dest);



/* members */
private:
	iass_cstring* shared_folder_path;
	ACE_Thread_Mutex access_mutex;

	std::vector<iass_sharedfile_data*> shared_files_vector;
	std::map<const char*, unsigned int> sources_map;
	std::map<const char*, unsigned int>::iterator sources_map_iter;

	/* buffer used to copy files from their origin to the shared folder */
	char filecopy_buffer[FILECOPY_BUFFER_SIZE];

	/* singletone stuff */
	static iass_filesystem* st_instance;
	static ACE_Thread_Mutex st_mutex;
};

#endif /*iass_filesystem_hh*/

