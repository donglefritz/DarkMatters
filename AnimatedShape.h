#ifndef AnimatedShape_h
#define AnimatedShape_h

#include "Shape.h"

class AnimatedShape : public Shape {
public:
	AnimatedShape(Ogre::SceneNode* sceneNode, Ogre::Entity* entity);
	~AnimatedShape(void);

	virtual void addLocation(Ogre::Vector3 location);
	virtual void startAnimation(Ogre::String animationName, bool shouldLoop);
	virtual void tick(Ogre::Real timeSinceLast);
	virtual void moveTowardsNextLocation(Ogre::Real timeSinceLast);
	
	void setAnimations(Ogre::String movementAnimName, Ogre::String idleAnimName);

protected:
	std::deque<Ogre::Vector3> mWalkList;
	Ogre::Real                mWalkSpeed;
	Ogre::Real                mDistance;
	Ogre::Vector3             mDirection;
	Ogre::Vector3             mDestination;
	Ogre::AnimationState*     mAnimationState;
	bool                      mMoving;
	Ogre::String              mMovementAnimation;
	Ogre::String              mIdleAnimation;

	virtual bool useNextLocation(void);

};

#endif
