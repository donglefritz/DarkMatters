#ifndef AnimatedShape_h
#define AnimatedShape_h

#include "Shape.h"

class AnimatedShape : public Shape {
public:
	AnimatedShape(Ogre::SceneNode* sceneNode, Ogre::Entity* entity);
	~AnimatedShape(void);

	virtual void addLocation(Ogre::Vector3 location);
	virtual bool useNextLocation(void);
	virtual void startAnimation(Ogre::String animationName, bool shouldLoop);
	virtual void addTime(Ogre::Real timeSinceLast);
	virtual void moveTowardsNextLocation(Ogre::String movingAnim, Ogre::String idleAnim, Ogre::Real timeSinceLast);

protected:
	std::deque<Ogre::Vector3> mWalkList;
	Ogre::Real                mWalkSpeed;
	Ogre::Real                mDistance;
	Ogre::Vector3             mDirection;
	Ogre::Vector3             mDestination;
	Ogre::AnimationState*     mAnimationState;
	bool                      mMoving;

};

#endif
