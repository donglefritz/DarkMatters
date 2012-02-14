#include "World.h"


World::World(void) : BasicWindow(), mWorldDepth(32), mWorldData(0) {
	mWorldData = new PolyVox::LargeVolume<PolyVox::Material8>(&loadRegion, &unloadRegion, mWorldDepth);
	mWorldData->setMaxNumberOfBlocksInMemory(4096);
	mWorldData->setMaxNumberOfUncompressedBlocks(32);
}

World::~World(void) {
	if (mWorldData != NULL) {
		delete mWorldData;
	}
}

void World::createScene(void) {
	mSceneMgr->setAmbientLight(Ogre::ColourValue(1.0f,1.0f,1.0f));
	addAxesLines(50);

}

bool World::frameRenderingQueued(const Ogre::FrameEvent& evt) {
	if (!BasicWindow::frameRenderingQueued(evt)) { return false; }
	return true;
}

void World::loadRegion(const PolyVox::ConstVolumeProxy<PolyVox::Material8>& volData, const PolyVox::Region& region) {
	std::stringstream ss;
	ss << "Info: loadRegion() is loading region: " << region.getLowerCorner() << " -> " << region.getUpperCorner();
	Utils::log(ss.str());
	uint8_t material = 2;
	for (int x=region.getLowerCorner().getX(); x<region.getUpperCorner().getX(); x++) {
		for (int y=region.getLowerCorner().getY(); y<region.getUpperCorner().getY(); ++y) {
			for (int z=region.getLowerCorner().getZ(); z<region.getUpperCorner().getZ(); z++) {
				if (Utils::randomBool()) {
					PolyVox::Material8 voxel = volData.getVoxelAt(x,y,z);
					voxel.setMaterial(material);
					volData.setVoxelAt(x,y,z, voxel);
				}
            }
        }
    }
}

void World::unloadRegion(const PolyVox::ConstVolumeProxy<PolyVox::Material8>& volData, const PolyVox::Region& region) {
	std::stringstream ss;
	ss << "Info: unloadRegion() is unloading region: " << region.getLowerCorner() << " -> " << region.getUpperCorner();
	Utils::log(ss.str());
}
