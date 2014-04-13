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

#include "iass_configs_entry.hh"


iass_configs_entry::iass_configs_entry(std::string _path, std::string _desc) {
	path = _path;
	description = _desc;

	if (description=="") {
		std::cout << "WARNING iass_configs_entry, _descr is empty\n";
	}

	if (path=="") {
		std::cout << "WARNING iass_configs_entry, path is empty\n";
		return; 
	}

	std::string::size_type index = 0;
	std::string::size_type found;
	/* find path elements separated by CONFIGS_ENTRY_PATH_SEP */
	while (index < path.size()) {
		found = path.find (CONFIGS_ENTRY_PATH_SEP, index);

		/* if no more separators were found */
		if (found == std::string::npos) {
			/* if not even one separator was found */
			if ( index==0 ) {
				elements.push_back(path.c_str());  
				return;
			}
			/* add the last element */
			elements.push_back(path.substr(index));
			return;
		}

		if ( (found <2) or (found>(path.size()-2)) ) {
			std::cout << "WARNING ::iass_configs_entry(...), path: "<< path <<" is bad <::> formatted\n";
			std::cout << "WARNING found: " << found << "\n";
		}

		elements.push_back(path.substr(index, (found-index)));
		index = found + std::string(CONFIGS_ENTRY_PATH_SEP).size();
	}

	/* if no separator was found, the whole path string is one element */
	if ( (elements.size()==0) and (path.size()>0) ) elements.push_back(path.c_str());  
}


std::string iass_configs_entry::get_path(void) {
	return path.c_str();
}

unsigned int iass_configs_entry::get_num_path_elements(void) {
	return elements.size();
}

std::string iass_configs_entry::get_path_element(unsigned int i) {
	if (i>= elements.size()) return "";
	else return elements[i].c_str();
}


std::string iass_configs_entry::get_description(void) {
	return description.c_str();
}

iass_configs_entry_type iass_configs_entry::get_type(void) {
	return entry_type;
}


/***** INT *****/
iass_configs_int_entry::iass_configs_int_entry(std::string _path, std::string _desc, int _min, int _max, int _value) :
	iass_configs_entry(_path, _desc) {
	entry_type = INT;
	min = _min;
	max = _max;
	set_value(_value);
 
}


int iass_configs_int_entry::get_value(void) {
	return value;
}


void iass_configs_int_entry::set_value(int _value) {
	if ( _value <= min ) value = min;
	else if ( _value >= max ) value = max;
	else value = _value;
}


/***** UNSIGNED INT *****/
iass_configs_uint_entry::iass_configs_uint_entry(std::string _path, std::string _desc, unsigned int _min,
						 unsigned int _max, unsigned int _value) :
	iass_configs_entry(_path, _desc) {
	entry_type = UINT;
	min = _min;
	max = _max;
	set_value(_value);
}


unsigned int iass_configs_uint_entry::get_value(void) {
	return value;
}


void iass_configs_uint_entry::set_value(unsigned int _value) {
	if ( _value <= min ) value = min;
	else if ( _value >= max ) value = max;
	else value = _value;
}


/***** BOOLEAN *****/
iass_configs_boolean_entry::iass_configs_boolean_entry(std::string _path, std::string _desc, bool _value) :
	iass_configs_entry(_path, _desc) {
	entry_type = BOOLEAN;
	set_value(_value);
}


bool iass_configs_boolean_entry::get_value(void) {
	return value;
}


void iass_configs_boolean_entry::set_value(bool _value) {
	value = _value;
}


/***** DOUBLE *****/
iass_configs_double_entry::iass_configs_double_entry(std::string _path, std::string _desc,
						     double _min, double _max, double _value) :
	iass_configs_entry(_path, _desc) {
	entry_type = DOUBLE;
	min = _min;
	max = _max;
	set_value(_value);
 
}


double iass_configs_double_entry::get_value(void) {
	return value;
}


void iass_configs_double_entry::set_value(double _value) {
	if ( _value <= min ) value = min;
	else if ( _value >= max ) value = max;
	else value = _value;
}


/***** STRING *****/
iass_configs_string_entry::iass_configs_string_entry(std::string _path, std::string _desc, std::string _value) :
	iass_configs_entry(_path, _desc) {
	entry_type = STRING;
	set_value(_value);
}


std::string iass_configs_string_entry::get_value(void) {
	return value.c_str();
}


void iass_configs_string_entry::set_value(std::string _value) {
	value = _value.c_str();
}

