#include "Utils.h"

Utils::Utils(void){
	srand((unsigned int)time(NULL));
}

Utils::~Utils(void){
}

bool Utils::randomBool(void) {
	return rand()%2==0 ? true : false;
}

float Utils::randomFloat(void) {
	return rand() / static_cast<float>(RAND_MAX);
}

int Utils::randomInt(int excludedMax) {
	return rand() % excludedMax;
}

float Utils::distanceBetween(Ogre::Vector3 p1, Ogre::Vector3 p2) {
	return distanceBetween(p1.x, p1.y, p1.z, p2.x, p2.y, p2.z);
}

float Utils::distanceBetween(int p1_x, int p1_y, int p1_z, int p2_x, int p2_y, int p2_z) {
	float x, y, z;
	x  = p2_x - p1_x;
	x *= x;
	y  = p2_y - p1_y;
	y *= y;
	z  = p2_z - p1_z;
	z *= z;
	return sqrtf(x+y+z);
}