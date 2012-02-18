#ifndef Shape_h
#define Shape_h

#include <OgreEntity.h>
#include <OgreSceneNode.h>
#include <OgreVector3.h>

#include "Utils.h"
#include <sstream>

class Shape {
public:
	Shape(void);
	Shape(Ogre::SceneNode* sceneNode, Ogre::Entity* entity);
	~Shape(void);

	Ogre::Entity*    mEntity;
	Ogre::SceneNode* mSceneNode;


	void descToLog(void);
	void setOriginalDirection(Ogre::Vector3 direction);
	void face(Ogre::Vector3 position);
	Ogre::Vector3 getCenter(void);
	Ogre::Vector3 getPosition(void);

protected:
	Ogre::Vector3  mOriginalDirection;
};

#endif

