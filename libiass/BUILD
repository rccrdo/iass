
 *
 * iass|intelligent autonomous systems studio
 *
 * iass component: libiass
 *  

 ================== DEPENDENCIES ================== 

 pkgs: libxml2-dev, libtao-dev, libtao-orbsvcs-dev, tao-idl
        libgl-dev, libglu-dev

 * ACE
Install libace and libace-dev pkgs
add following line to /usr/include/ace/config-lite.h
68: #define _REENTRANT

 * ODE - Open Physics Engine
install libglu.z.y.z-dev

Get last stable ODE release at:
http://sourceforge.net/projects/opende/
./configure --prefix=/usr/local --enable-double-precision --enable-release --with-trimesh=opcode

For a debug release use --enable-debug(now used) instead of --enable-release.
./configure --prefix=/usr/local --enable-double-precision --enable-debug --with-trimesh=opcode

Some ODE releases misdefine trimesh defines on installed headers. Add these lines to /usr/local/include/ode/config.h
/* Use opcode mesh support */
#define dTRIMESH_ENABLED
#define dTRIMESH_OPCODE
in ode/common.h delete lines from 108 to 113


