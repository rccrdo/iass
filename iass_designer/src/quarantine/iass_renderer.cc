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

#include "iass_renderer.hh"

iass_renderer::iass_renderer(iass_ts_renderer_queue* incoming, iass_ts_renderer_queue* outcoming) {
	/* defaults */
	iqueue = NULL;
	oqueue = NULL;

	shutdown = false;
	keep_running = true;

	/* check arguments */
	if (incoming==NULL) {
		std::cout << "warning iass_renderer(iass_ts_renderer_queue* incoming, iass_ts_renderer_queue* outcoming), incoming is a NULL ptr\n";
		return;
	}

	if (outcoming==NULL) {
		std::cout << "warning iass_renderer(iass_ts_renderer_queue* incoming, iass_ts_renderer_queue* outcoming), outcoming is a NULL ptr\n";
		return;
	}

	iqueue = incoming;
	oqueue = outcoming;

	/* start-up ogre */
	ogre_root = new Ogre::Root();
	ogre_root->showConfigDialog();
	ogre_root->initialise(true, "Render Window");

	/* ogre scene manager */
	ogre_scenemngr = ogre_root->createSceneManager(Ogre::ST_GENERIC, "Scene Manager");

	/* create a default camera */
	ogre_def_camera = ogre_scenemngr->createCamera("Default Camera");
	ogre_def_camera->setNearClipDistance(5);
	ogre_def_camera->setFarClipDistance(1000);
	ogre_def_camera->setAspectRatio(Ogre::Real(1.333333));
  
	/* create a window and a viewport */
	ogre_renderwin = ogre_root->getAutoCreatedWindow();
	ogre_def_viewport = ogre_renderwin->addViewport(ogre_def_camera);
	ogre_def_viewport->setBackgroundColour(Ogre::ColourValue(1, 1, 1));

	// Set ambient light
	ogre_scenemngr->setAmbientLight(Ogre::ColourValue(0.7, 0.7, 0.7));
	// Create a point light
	Ogre::Light* l = ogre_scenemngr->createLight("MainLight");

	// Load resource paths from config file
	Ogre::ConfigFile cf;
	cf.load("resources.cfg");

	// Go through all sections & settings in the file
	Ogre::ConfigFile::SectionIterator seci = cf.getSectionIterator();

	std::string secName, typeName, archName;
	while (seci.hasMoreElements())  {
		secName = seci.peekNextKey();
		Ogre::ConfigFile::SettingsMultiMap *settings = seci.getNext();
		Ogre::ConfigFile::SettingsMultiMap::iterator i;
		for (i = settings->begin(); i != settings->end(); ++i)  {
			typeName = i->first;
			archName = i->second;
			Ogre::ResourceGroupManager::getSingleton().addResourceLocation(archName, typeName, secName);
		}
	}
	Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

	// Accept default settings: point light, white diffuse, just set position
	// NB I could attach the light to a SceneNode if I wanted it to move
	// automatically with other objects, but I don't
	l->setPosition(20,80,50);



	Ogre::SceneNode *objectNode = ogre_scenemngr->getRootSceneNode()->createChildSceneNode();

	Ogre::Entity *ent = ogre_scenemngr->createEntity("Cube", "Cube.mesh");
	// Set material loaded from Example.material
	ent->setMaterialName("Material");

	objectNode->attachObject(ent);
	objectNode->setPosition(Ogre::Vector3(0, 0, -500));
	objectNode->setPosition(Ogre::Vector3(0, 0, -500));



	/* register a start frame listener to elaborate incoming/outcoming queue */

	/* start rendering */
}


int iass_renderer::run(void) {

	while (!shutdown) {
		while (keep_running) {
			ogre_root->renderOneFrame();
			/* timing stuff */
			usleep (1000);
		}
		usleep (500000);
	}

	return 0;
}




