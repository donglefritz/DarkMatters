#include "AnimatedShape.h"


AnimatedShape::AnimatedShape(Ogre::SceneNode* sceneNode, Ogre::Entity* entity) : Shape(sceneNode,entity), mWalkSpeed(35), mDirection(Ogre::Vector3::ZERO) {
}

AnimatedShape::~AnimatedShape(void) {
}

void AnimatedShape::addLocation(Ogre::Vector3 location) {
	mWalkList.push_back(location);
}

bool AnimatedShape::useNextLocation(void) {
	if (mWalkList.empty()) { return false; }

	mDestination = mWalkList.front();
	mWalkList.pop_front();

	mDirection = mDestination - mSceneNode->getPosition();
	mDistance  = mDirection.normalise();

	Ogre::Vector3 origFacing = Ogre::Vector3::UNIT_X;
	Ogre::Vector3 nowFacing  = mSceneNode->getOrientation() * origFacing;
	// avoid divide by zero from rotate() on exactly 180 degrees:
	if ((nowFacing.dotProduct(mDirection)+1) < 0.0001f) {
		mSceneNode->yaw(Ogre::Degree(180));
	} else {
		mSceneNode->rotate(nowFacing.getRotationTo(mDirection));
	}
	return true;
}

void AnimatedShape::startAnimation(Ogre::String animationName, bool shouldLoop) {
	mAnimationState = mEntity->getAnimationState(animationName);
	mAnimationState->setLoop(shouldLoop);
	mAnimationState->setEnabled(true);
}

