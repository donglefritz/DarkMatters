#ifndef World_h
#define World_h

#include "basicwindow.h"
#include "Perlin.h"
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
	// Ogre:
	virtual void createScene(void);
	virtual bool frameRenderingQueued(const Ogre::FrameEvent& evt);

	// PolyVox:
	uint16_t mWorldDepth;
	PolyVox::LargeVolume<PolyVox::Material8>* mWorldData;
	
	// These must be static to retain callback context (or something like that :)
	static void loadRegion(const PolyVox::ConstVolumeProxy<PolyVox::Material8>& volData, const PolyVox::Region& region);
	static void unloadRegion(const PolyVox::ConstVolumeProxy<PolyVox::Material8>& volData, const PolyVox::Region& region);


};


#endif // World_h
