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

#ifndef iass_sharedfile_data_hh
#define iass_sharedfile_data_hh

#include <iostream>
#include "iass_cstring.hh"


/* known file types */
enum iass_file_type {
	file_t_unknown,
	file_t_basictype
};


class iass_sharedfile_data
{
/* functions */
public:
	iass_sharedfile_data(const char* _abs_path, unsigned int _abs_path_len,
			     const char* _rel_path, unsigned int _rel_path_len,
			     iass_file_type _file_t);
	~iass_sharedfile_data();

	iass_cstring* abs_path(void);
	iass_cstring* rel_path(void);
	iass_file_type type(void);


/* members */
private:
	/* absolute path to file
	 * i.e /opt/iass_shared/model-xyz.xml
	 * where /opt/iass_shared is the folder we are using as shared folder */
	iass_cstring* abs;


	/* relative path to file
	 * contains the path to the file omitting the part of the path to the shared folder
	 * i.e in the previous example rel would contain: model-xyz.xml */
	iass_cstring* rel;

	/* file type */
	iass_file_type file_t;
};

#endif /*iass_sharedfile_data_hh*/


