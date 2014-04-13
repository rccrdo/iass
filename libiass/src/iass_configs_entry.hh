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

#ifndef iass_configs_entry_H_
#define iass_configs_entry_H_

#include <string>
#include <iostream>
#include <vector>


/*  xml tree path separator
    - in the xml tree that iass_configs uses, an string on the left of CONFIGS_ENTRY_PATH_SEP is the name of
    an anchestor node for every element at the right of of the element at right CONFIGS_ENTRY_PATH_SEP */
#define CONFIGS_ENTRY_PATH_SEP "::"


/* valid entry types */
enum iass_configs_entry_type {
	INT,
	UINT,
	BOOLEAN,
	DOUBLE,
	STRING
};


class iass_configs_entry
{
/* functions */
public:
	iass_configs_entry(std::string _path, std::string _desc);
	virtual ~iass_configs_entry() {};

	std::string get_path(void);
	std::string get_description(void);

	unsigned int get_num_path_elements(void);
	std::string get_path_element(unsigned int i);

	iass_configs_entry_type get_type(void);

/* members */
protected:
	std::string path;
	std::string description;
	iass_configs_entry_type entry_type;

private:
	unsigned int num_elements;
	std::vector<std::string> elements;

};



/***** int entry *****/
class iass_configs_int_entry : public iass_configs_entry
{
/* functions */
public:
	iass_configs_int_entry(std::string _path, std::string _desc, int _min, int _max, int _value);
	~iass_configs_int_entry() {};

	int get_value(void);
	void set_value(int _value);

/* members */
private:
	int min, max, value;
};



/***** uint entry *****/
class iass_configs_uint_entry : public iass_configs_entry
{
/* functions */
public:
	iass_configs_uint_entry(std::string _path, std::string _desc, unsigned int _min, unsigned int _max, unsigned int _value);
	~iass_configs_uint_entry() {};

	unsigned int get_value(void);
	void set_value(unsigned int _value);

/* members */
private:
	unsigned int min, max, value;
};



/***** boolean entry *****/
class iass_configs_boolean_entry : public iass_configs_entry
{
/* functions */
public:
	iass_configs_boolean_entry(std::string _path, std::string _desc, bool _value);
	~iass_configs_boolean_entry() {};

	bool get_value(void);
	void set_value(bool _value);

/* members */
private:
	bool value;
};



/***** double entry *****/
class iass_configs_double_entry : public iass_configs_entry
{
/* functions */
public:
	iass_configs_double_entry(std::string _path, std::string _desc, double _min, double _max, double _value);
	~iass_configs_double_entry() {};

	double get_value(void);
	void set_value(double _value);

/* members */
private:
	double min, max, value;
};




/***** string entry *****/
class iass_configs_string_entry : public iass_configs_entry
{
/* functions */
public:
	iass_configs_string_entry(std::string _path, std::string _desc, std::string _value);
	~iass_configs_string_entry() {};

	std::string get_value(void);
	void set_value(std::string _value);

/* members */
private:
	std::string value;
};


#endif /*iass_configs_entry_HH_*/

