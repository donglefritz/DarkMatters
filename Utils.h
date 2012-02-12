#ifndef UTILS_H
#define UTILS_H

#include <cstdlib>
#include <time.h>
#include <cmath>
#include <OgreVector3.h>
#include <OgreColourValue.h>



class Utils {
public:
	Utils(void);
	~Utils(void);

	static float CubeSize;

	bool  randomBool(void);
	float randomFloat(void);
	int   randomInt(int excludedMax);

	float distanceBetween(Ogre::Vector3 p1, Ogre::Vector3 p2);
	float distanceBetween(int p1_x, int p1_y, int p1_z, int p2_x, int p2_y, int p2_z);
};

#endif
