#include "World.h"


World::World(void) : BasicWindow(), mChunkSize(32), mWorldData(0) {
	mWorldData = new PolyVox::LargeVolume<PolyVox::Material8>(&loadRegion, &unloadRegion, mChunkSize);
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

	mCamera->setPosition(90, 280, 535);
	mCamera->pitch(Ogre::Degree(-30));
	mCamera->yaw(Ogre::Degree(-15));

	// animated robot:
	mWalkSpeed = 35;
	mDirection = Ogre::Vector3::ZERO;
	Ogre::Vector3 startLoc(0, 0, 25);
	mEntity = mSceneMgr->createEntity("Robot", "robot.mesh");
	mNode   = mSceneMgr->getRootSceneNode()->createChildSceneNode("RobotNode", startLoc);
	mNode->attachObject(mEntity);

	mWalkList.push_back(Ogre::Vector3(550, 0, 50));
	mWalkList.push_back(Ogre::Vector3(-100, 0, -200));

	mAnimationState = mEntity->getAnimationState("Idle");
	mAnimationState->setLoop(true);
	mAnimationState->setEnabled(true);


}

bool World::nextLocation(void) {
	if (mWalkList.empty()) { return false; }

	mDestination  = mWalkList.front();
	mWalkList.pop_front();

	mDirection = mDestination - mNode->getPosition();
	mDistance  = mDirection.normalise();


	return true;
}


void World::polyVoxMeshToOgreObject(PolyVox::SurfaceMesh<PolyVox::PositionMaterialNormal>* mesh,Ogre::ManualObject* mo) {
	// make sure mesh is allocated and valid first:
	if (mesh == NULL || mesh->getNoOfVertices() < 1) {
		Utils::log("ERROR: World::polyVoxMeshToOgreObject() got a mesh with no data.");
		return;
	} else if (mesh->getNoOfIndices() < 1) {
		Utils::log("ERROR: World::polyVoxMeshToOgreObject() got a mesh with no indices.");
		return;
	}

	const std::vector<uint32_t>& indices                         = mesh->getIndices();
	const std::vector<PolyVox::PositionMaterialNormal>& vertices = mesh->getVertices();

	uint32_t numIndices  = mesh->getNoOfIndices();
	uint32_t numVertices = mesh->getNoOfVertices();

	mo->estimateIndexCount(numIndices);
	mo->estimateVertexCount(numVertices);

	mo->begin("BaseWhiteNoLighting", Ogre::RenderOperation::OT_TRIANGLE_LIST);

	PolyVox::Vector3DFloat position;
	PolyVox::Vector3DFloat normal;
	float material;
	int textureIndex = 0;
	const Ogre::Vector2 texCoords[4] = { Ogre::Vector2(0.0f, 1.0f), Ogre::Vector2(1.0f, 1.0f), Ogre::Vector2(0.0f, 0.0f), Ogre::Vector2(1.0f, 0.0f) };

	// vertices:
	for(unsigned int i=0; i<numVertices; ++i) {
		position = vertices[i].getPosition();
		normal   = vertices[i].getNormal();
		material = vertices[i].getMaterial();
            
		mo->position((Ogre::Real)(position.getX()), (Ogre::Real)(position.getY()), (Ogre::Real)(position.getZ()));
		mo->normal(  (Ogre::Real)(normal.getX()),   (Ogre::Real)(normal.getY()),   (Ogre::Real)(normal.getZ()));
		mo->textureCoord(texCoords[textureIndex++ % 4]);
	}

	// indices:
	for(unsigned int i = 0 ; i < numIndices ; i++) {
		mo->index((Ogre::uint32)(indices[i]));
	}
	mo->end();

}

// CALLBACKS:

bool World::frameRenderingQueued(const Ogre::FrameEvent& evt) {
	if (!BasicWindow::frameRenderingQueued(evt)) { return false; }

	// animated robot:
	if (mDirection == Ogre::Vector3::ZERO) {
		if (nextLocation()) {
			mAnimationState = mEntity->getAnimationState("Walk");
			mAnimationState->setLoop(true);
			mAnimationState->setEnabled(true);
		}
	} else {
		Ogre::Real move = mWalkSpeed * evt.timeSinceLastFrame;
		mDistance -= move;
		if (mDistance <= 0) {
			mNode->setPosition(mDestination);
			mDirection = Ogre::Vector3::ZERO;
			if (!nextLocation()) {
				mAnimationState = mEntity->getAnimationState("Idle");
				mAnimationState->setLoop(true);
				mAnimationState->setEnabled(true);
			} else {
				Ogre::Vector3 origFacing = Ogre::Vector3::UNIT_X;
				Ogre::Vector3 nowFacing  = mNode->getOrientation() * origFacing;
				// avoid divide by zero from rotate() on exactly 180 degrees:
				if ((nowFacing.dotProduct(mDirection)+1) < 0.0001f) {
					mNode->yaw(Ogre::Degree(180));
				} else {
					mNode->rotate(nowFacing.getRotationTo(mDirection));
				}
			}
		} else {
			mNode->translate(mDirection * move);
		}
	}


	mAnimationState->addTime(evt.timeSinceLastFrame);
	return true;
}

void World::loadRegion(const PolyVox::ConstVolumeProxy<PolyVox::Material8>& volData, const PolyVox::Region& region) {
	std::stringstream ss;
	ss << "Info: loadRegion() is loading region: " << region.getLowerCorner() << " -> " << region.getUpperCorner();
	Utils::log(ss.str());
	uint8_t material = 2;

	int lo_x = region.getLowerCorner().getX() +1;
	int lo_y = region.getLowerCorner().getY() +1;
	int lo_z = region.getLowerCorner().getZ() +1;
	
	int hi_x = 7;
	int hi_y = 3;
	int hi_z = 7;

	for (int x=lo_x; x<hi_x; ++x) {
		for (int y=lo_y; y<hi_y; ++y) {
			for (int z=lo_z; z<hi_z; ++z) {
				PolyVox::Material8 voxel = volData.getVoxelAt(x,y,z);
				voxel.setMaterial(material);
				volData.setVoxelAt(x,y,z, voxel);
			}
		}
	}

}

void World::unloadRegion(const PolyVox::ConstVolumeProxy<PolyVox::Material8>& volData, const PolyVox::Region& region) {
	std::stringstream ss;
	ss << "Info: unloadRegion() is unloading region: " << region.getLowerCorner() << " -> " << region.getUpperCorner();
	Utils::log(ss.str());
}
