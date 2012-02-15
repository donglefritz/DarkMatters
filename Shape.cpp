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

void Shape::desc(void) {
	float cx = getCenter().x;
	float cy = getCenter().y;
	float cz = getCenter().z;
	std::stringstream ss;
	ss << "SHAPE: entName: " << mEntity->getName() << " nodeName: " << mSceneNode->getName() << " center: " << cx <<","<< cy <<","<< cz;
	Utils::log(ss.str());
}