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

#include "iass_sharedfile_data.hh"


iass_sharedfile_data::iass_sharedfile_data(const char* _abs_path, unsigned int _abs_path_len,
                                           const char* _rel_path, unsigned int _rel_path_len,
                                           iass_file_type _file_t)   {
	rel = NULL;
	abs = NULL;
	file_t = file_t_unknown;

	/* check arguments */
	if (_abs_path==NULL) {
		std::cout << "warning iass_sharedfile_data(constr.), _abs_path is a NULL ptr\n";
		return;
	}
	if (_rel_path==NULL) {
		std::cout << "warning iass_sharedfile_data(constr.), _rel_path is a NULL ptr\n";
		return;
	}

	if (_abs_path_len==0) {
		std::cout << "warning iass_sharedfile_data(constr.), _abs_path_len is 0\n";
		return;
	}
	if (_rel_path_len==0) {
		std::cout << "warning iass_sharedfile_data(constr), _rel_path_len is 0\n";
		return;
	}

	abs = new iass_cstring(_abs_path, _abs_path_len);
	rel = new iass_cstring(_rel_path, _rel_path_len);

	file_t = _file_t;
}


iass_sharedfile_data::~iass_sharedfile_data() {
	if (rel) delete rel;
	if (abs) delete abs;
}


iass_cstring* iass_sharedfile_data::abs_path(void) {
	return abs;
}


iass_cstring* iass_sharedfile_data::rel_path(void) {
	return rel;
}


iass_file_type iass_sharedfile_data::type(void) {
	return file_t;
}




