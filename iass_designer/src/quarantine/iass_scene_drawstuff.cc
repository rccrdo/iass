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

#include "iass_scene_drawstuff.hh"

iass_scene_drawstuff::iass_scene_drawstuff() : iass_scene((iass_scene_stats*)1) {
	std::cout << " * initing scene renderer iass_scene_drawstuff\n";
	fn.version = DS_VERSION;
	fn.start = &drawstuff_start;
	fn.step = &drawstuff_render_loop;
	fn.command = &drawstuff_command;
	fn.stop = 0;
	fn.path_to_textures = 0;	// uses default
}


void iass_scene_drawstuff::run() {
	// run loop
	dsSimulationLoop (0, NULL,400,400, &fn);
}


void iass_scene_drawstuff::drawstuff_start() {
	// adjust the starting viewpoint a bit
	float xyz[3],hpr[3];
	dsGetViewpoint (xyz,hpr);
	hpr[0] += 7;
	dsSetViewpoint (xyz,hpr);
}


void iass_scene_drawstuff::drawstuff_command (int cmd) {
	dsPrint ("received command %d (`%c')\n",cmd,cmd);
	return;
}


void iass_scene_drawstuff::drawstuff_render_loop (int pause) {
	float pos[3];
	float R[12];
	static float a = 0;
	static iass_loop_sleeper lp(80);

	if (!pause) a += 0.02f;
	if (a > (2*M_PI)) a -= (float) (2*M_PI);
	float ca = (float) cos(a);
	float sa = (float) sin(a);

	dsSetTexture (DS_WOOD);

	float b = (a > M_PI) ? (2*(a-(float)M_PI)) : a*2;
	pos[0] = -0.3f;
	pos[1] = 0;
	pos[2] = (float) (0.1f*(2*M_PI*b - b*b) + 0.65f);
	R[0] = ca; R[1] = 0; R[2] = -sa;
	R[4] = 0;  R[5] = 1; R[6] = 0;
	R[8] = sa; R[9] = 0; R[10] = ca;
	dsSetColor (1,0.8f,0.6f);
	dsDrawSphere (pos,R,0.3f);

	dsSetTexture (DS_NONE);

	pos[0] = -0.2f;
	pos[1] = 0.8f;
	pos[2] = 0.4f;
	R[0] = ca; R[1] = -sa; R[2] = 0;
	R[4] = sa; R[5] = ca;  R[6] = 0;
	R[8] = 0;  R[9] = 0;	 R[10] = 1;
	float sides[3] = {0.1f,0.4f,0.8f};
	dsSetColor (0.6f,0.6f,1);
	dsDrawBox (pos,R,sides);

	dsSetTexture (DS_WOOD);

	float r = 0.3f;		      // cylinder radius
	float d = (float)cos(a*2) * 0.4f;
	float cd = (float)cos(-d/r);
	float sd = (float)sin(-d/r);
	pos[0] = -0.2f;
	pos[1] = -1 + d;
	pos[2] = 0.3f;
	R[0] = 0;   R[1] = 0;  R[2] = -1;
	R[4] = -sd; R[5] = cd; R[6] =  0;
	R[8] =  cd; R[9] = sd; R[10] = 0;
	dsSetColor (0.4f,1,1);
	dsDrawCylinder (pos,R,0.8f,r);

	pos[0] = 0;
	pos[1] = 0;
	pos[2] = 0.2f;
	R[0] = 0; R[1] = sa; R[2] = -ca;
	R[4] = 0; R[5] = ca; R[6] = sa;
	R[8] = 1; R[9] = 0;  R[10] = 0;
	dsSetColor (1,0.9f,0.2f);
	dsDrawCappedCylinder (pos,R,0.8f,0.2f);
	lp.sleep();
}

void iass_scene_drawstuff::main_loop_jobs(void) {
	std::cout << "warning iass_scene_drawstuff::main_loop_jobs(void), doing nothing\n";
}


