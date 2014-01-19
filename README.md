charcam
=======
About:
	This project is a basic character and camera movement system built on OGRE.

Licensing and Credits:
	The OGRE license is in: ogre_license.txt
	Much of this project relies and builds upon the OGRE tutorial framework.
	See for more information:
		http://www.ogre3d.org/tikiwiki/

	I do not take credit for any files except:
	CharCam.h
	CharCam.cpp

	All other files should be assumed to have come from the OGRE tutorial framework.
	It should also be known that these files may also be altered, and not assumed to be the originals from OGRE.

	The following files have been taken from either OGRE's distribution or Tutorial framework, and may have been altered:
	./src:
		BaseApplication.cpp
		BaseApplication.h

	./src/dist/bin:
		plugins.cfg
		resources.cfg

	./src/dist/media/materials/programs:
		Example_Basic.cg
		Grass.cg
		hdr.cg

	./src/dist/media/materials/scripts:
		Examples.material

	./src/dist/media/materials/textures:
		clouds.jpg
		grass_1024.jpg
		r2skin.jpg

	./src/dist/media/models:
		robot.mesh
		robot.skeleton

Building:
	This project has only been built on linux, building on windows has not been attempted.
	Dependencies: OGRE-1.9, OIS, libboost
	To build simply run the command 'make' in the root project directory.
	All executable files should be available in the bin folder.

How to use(keybindings):
	Key		Function
	-----	-----------------------
	W		Move character forward
	A		Move character left
	S		Move character backward
	D		Move character right
	I		Zoom camera in
	O		Zoom camera out
	UP		Move camera up
	DOWN	Move camera down
	LEFT	Move camera left
	RIGHT	Move camera right
