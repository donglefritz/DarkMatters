#include "Shape.h"

Shape::Shape(void) {
	mSceneNode = NULL;
	mEntity    = NULL;
}

Shape::Shape(Ogre::SceneNode* sceneNode, Ogre::Entity* entity) {
	mSceneNode = sceneNode;
	mEntity    = entity;
}

Shape::~Shape(void) {
}

Ogre::Vector3 Shape::getCenter(void) {
	return mEntity->getWorldBoundingBox().getCenter();
}

Ogre::Vector3 Shape::getPosition(void) {
	return mSceneNode->getPosition();
}

void Shape::descToLog(void) {
	float cx = getCenter().x;
	float cy = getCenter().y;
	float cz = getCenter().z;
	float px = getPosition().x;
	float py = getPosition().y;
	float pz = getPosition().z;
	std::stringstream ss;
	ss << "SHAPE: entName: " << mEntity->getName() << " nodeName: " << mSceneNode->getName();
	ss << " center: " << cx <<","<< cy <<","<< cz;
	ss << " position: "<<px <<","<< py <<","<< pz;
	Utils::log(ss.str());
}