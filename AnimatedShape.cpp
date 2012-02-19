#include "AnimatedShape.h"


AnimatedShape::AnimatedShape(Ogre::SceneNode* sceneNode, Ogre::Entity* entity) : Shape(sceneNode,entity), mWalkSpeed(35), mDirection(Ogre::Vector3::ZERO), mDistance(0), mMoving(false) {
}

AnimatedShape::~AnimatedShape(void) {
}

void AnimatedShape::tick(Ogre::Real timeSinceLast) {
	mAnimationState->addTime(timeSinceLast);
	if (mWalkList.empty() && Utils::randomInt(1000)==1) {
		for (int i=0; i<Utils::randomInt(5)+1; ++i) {
			addLocation(Utils::randomFlatVector3(200, 0));
		}
	} else {
		moveTowardsNextLocation(timeSinceLast);
	}
}

void AnimatedShape::addLocation(Ogre::Vector3 location) {
	mWalkList.push_back(location);
}

bool AnimatedShape::useNextLocation(void) {
	if (mWalkList.empty()) { 
		return false; 
	} else {
		mDestination = mWalkList.front();
		mWalkList.pop_front();
		return true;
	}
}

void AnimatedShape::setAnimations(Ogre::String movementAnimName, Ogre::String idleAnimName) {
	mMovementAnimation = movementAnimName;
	mIdleAnimation     = idleAnimName;
}

void AnimatedShape::moveTowardsNextLocation(Ogre::Real timeSinceLast) {
	mDirection = mDestination - mSceneNode->getPosition();
	mDistance  = mDirection.normalise();

	face(mDirection);

	if (mMoving) {
		Ogre::Real moveAmount = mWalkSpeed * timeSinceLast;
		mDistance -= moveAmount;
		if (mDistance <= 0) {
			mSceneNode->setPosition(mDestination);
			mMoving = false;
			startAnimation(mIdleAnimation, true);
		} else {
			mSceneNode->translate(mDirection * moveAmount);
		}
	} else {
		if (useNextLocation()) {
			startAnimation(mMovementAnimation, true);
			mMoving = true;
		}
	}
}

void AnimatedShape::startAnimation(Ogre::String animationName, bool shouldLoop) {
	mAnimationState = mEntity->getAnimationState(animationName);
	mAnimationState->setLoop(shouldLoop);
	mAnimationState->setEnabled(true);
}

