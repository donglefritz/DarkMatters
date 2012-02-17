#ifndef World_h
#define World_h

#include "basicwindow.h"
#include "Perlin.h"
#include "Shape.h"
#include "PolyVoxCore/MaterialDensityPair.h"
#include "PolyVoxCore/CubicSurfaceExtractorWithNormals.h"
#include "PolyVoxCore/SurfaceMesh.h"
#include "PolyVoxCore/SimpleVolume.h"
#include <PolyVoxCore/LargeVolume.h>
#include <PolyVoxCore/Material.h>


class World : public BasicWindow {
public:
	World(void);
	~World(void);

protected:
	uint16_t mChunkSize;
	PolyVox::LargeVolume<PolyVox::Material8>* mWorldData;

	virtual void createScene(void);
	virtual bool frameRenderingQueued(const Ogre::FrameEvent& evt);
	virtual void polyVoxMeshToOgreObject(PolyVox::SurfaceMesh<PolyVox::PositionMaterialNormal>* mesh,Ogre::ManualObject* mo);

	// These only work if static and I'm not exactly sure why (has to do with context);
	static void loadRegion(const PolyVox::ConstVolumeProxy<PolyVox::Material8>& volData, const PolyVox::Region& region);
	static void unloadRegion(const PolyVox::ConstVolumeProxy<PolyVox::Material8>& volData, const PolyVox::Region& region);

	// intermediate ogre tutorial #1:
	virtual bool nextLocation(void);
	Ogre::Real                mDistance;
	Ogre::Vector3             mDirection;
	Ogre::Vector3             mDestination;
	Ogre::AnimationState*     mAnimationState;
	Ogre::Entity*             mEntity;
	Ogre::SceneNode*          mNode;
	std::deque<Ogre::Vector3> mWalkList;
	Ogre::Real                mWalkSpeed;

};


#endif // World_h
