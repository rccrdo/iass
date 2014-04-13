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

#ifndef iass_material_hh
#define iass_material_hh

#include <string>
#include "iass_entity_ref.hh"
#include <ace/Thread_Mutex.h>

class iass_material
{
/* functions */
public:
	iass_material(iass_entity_ref* _xml_ref);
	~iass_material();

	/* get xml_ref */
	iass_entity_ref* xml_ref(void);

	double hardness(void);
	double friction(void);
	double bounciness(void);
	double density(void);

	/* iass_materials_store methods */
	unsigned int num_instances(void);

	/* iass_material_instance methods */
	void inc_instances_count(void);
	void dec_instances_count(void);



	void hardness(double v);
	void friction(double v);
	void bounciness(double v);
	void density(double v);


/* members */
private:
	iass_entity_ref* mat_xml_ref;
	unsigned int mat_instances;

	/* used to queue iass_material_instance(s) calls */
	ACE_Thread_Mutex mat_mutex; 

	/* physical properties */
	// Determines how far Solids can interpenetrate.  This must
	// be between 0 and 1.
	double mat_hardness;

	// Simple friction constant.  This must be between 0 and 1.
	double mat_friction;

	// Bounciness (i.e. restitution) determines how elastic the
	// collisions will be between this Material and another.  In other
	// words, the more bounciness, the farther the Solids will bounce
	// when they collide (and, in real life, the less energy is lost
	// due to heat and sound).  This must be between 0 and 1.
	double mat_bounciness;

	// Density combined with the volume of a Solid's shapes determine
	// the Solid's mass.  This must be >= 0.
	double mat_density;

	/* visual properties */

};

#endif /*iass_material_hh*/

