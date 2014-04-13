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

#ifndef iass_configs_hh
#define iass_configs_hh

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <map>
#include <vector>
#include <ace/Thread_Mutex.h>
#include "iass_configs_defaults.hh"
#include "iass_configs_entry.hh"
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>


class iass_configs
{
/* functions */
public:
	/* get singletone instance */
	static iass_configs* instance();

	void save_to_file(char* path);
	void load_from_file(char* path);

	int set_parameter(std::string path, int _value);
	int set_parameter(std::string path, unsigned int _value);
	int set_parameter(std::string path, bool _value);
	int set_parameter(std::string path, double _value);
	int set_parameter(std::string path, std::string _value);

	int get_parameter(std::string path, int* _value);
	int get_parameter(std::string path, unsigned int* _value);
	int get_parameter(std::string path, bool* _value);
	int get_parameter(std::string path, double* _value);
	int get_parameter(std::string path, std::string* _value);


private:
	iass_configs();
 
	void store_parameter(iass_configs_entry* p);
	void write_configs_file();


/* members */
private:
	ACE_Thread_Mutex access_mutex;
	std::vector<iass_configs_entry*> entries_vector;
	std::map<std::string, unsigned int> entries_map;
	std::map<std::string, unsigned int>::iterator entries_map_iter;

	/* singletone stuff */
	static iass_configs* st_instance;		// singletone instance
	static ACE_Thread_Mutex st_mutex;		// singletone obj creation mutex
};

#endif /*iass_configs_hh*/

