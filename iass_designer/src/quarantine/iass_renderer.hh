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

#ifndef iass_renderer_hh
#define iass_renderer_hh

#include <iostream>
#include "iass_ts_renderer_queue.hh"
#include <OGRE/Ogre.h>


class iass_renderer
{
/* functions */
public:
	iass_renderer(iass_ts_renderer_queue* incoming, iass_ts_renderer_queue* outcoming);

	int run(void);

/* members */
private:
	bool shutdown;
	bool keep_running;

	iass_ts_renderer_queue* iqueue;
	iass_ts_renderer_queue* oqueue;

	/* Ogre stuff */
	Ogre::Root *ogre_root;		  		// Ogre'scene root node
	Ogre::SceneManager *ogre_scenemngr;		// Ogre's scene manager
	Ogre::RenderWindow* ogre_renderwin;  		// Ogre's render window
	Ogre::Camera *ogre_def_camera;			// this scene dafault cam
	Ogre::Viewport* ogre_def_viewport;	// Ogre viewports
};

#endif /*iass_renderer_hh*/

