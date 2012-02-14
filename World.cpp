#include "World.h"


World::World(void) : BasicWindow(), mChunkSize(32), mWorldData(0), mViewableBoundary(8,8,8) {
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

	//PolyVox::Region region(PolyVox::Vector3DInt32(-255,0,0), PolyVox::Vector3DInt32(8,8,8));
	PolyVox::Region region(PolyVox::Vector3DInt32(-1,0,0), PolyVox::Vector3DInt32(8,8,8));
	PolyVox::SurfaceMesh<PolyVox::PositionMaterialNormal> mesh;
	PolyVox::CubicSurfaceExtractorWithNormals<PolyVox::LargeVolume,PolyVox::Material8> surfaceExtractor(mWorldData, region, &mesh);
	surfaceExtractor.execute();


	Ogre::ManualObject* mo = mSceneMgr->createManualObject("mo");
	Ogre::Vector3 offset   = Ogre::Vector3::ZERO;
	polyVoxMeshToOgreObject(&mesh, mo, &offset);
	mo->convertToMesh("worldMesh");
	Ogre::Entity* entity  = mSceneMgr->createEntity("worldEntity","worldMesh");
	Ogre::SceneNode* node = mSceneMgr->getRootSceneNode()->createChildSceneNode("worldNode");
	node->attachObject(entity);
	entity->setMaterialName("BlackBorder");
}

bool World::frameRenderingQueued(const Ogre::FrameEvent& evt) {
	if (!BasicWindow::frameRenderingQueued(evt)) { return false; }

	static int last_x = 0;

	Ogre::Vector3 pos = mCamera->getPosition();
	int requestAmount = 8;
	int crnt_x        = (int)pos.x / requestAmount;
	bool addMoreWorld = false;

	if (crnt_x > last_x) {
		last_x++;
		addMoreWorld = true;
	}

	if (addMoreWorld) {
		int start_x = 0;
		int end_x   = last_x * requestAmount;

		PolyVox::Region region(PolyVox::Vector3DInt32(start_x,0,0), PolyVox::Vector3DInt32(end_x,8,end_x));
		PolyVox::SurfaceMesh<PolyVox::PositionMaterialNormal> mesh;
		mWorldData->prefetch(region);
		PolyVox::CubicSurfaceExtractorWithNormals<PolyVox::LargeVolume,PolyVox::Material8> surfaceExtractor(mWorldData, region, &mesh);
		surfaceExtractor.execute();

		// TODO: setup dynamic ManualObject instead of destroy/create every frame.

		mSceneMgr->destroyEntity("worldEntity");
		Ogre::ManualObject* mo    = mSceneMgr->getManualObject("mo");
		mo->clear();
		polyVoxMeshToOgreObject(&mesh, mo);
		mo->convertToMesh("worldMesh");
		Ogre::Entity* worldEntity  = mSceneMgr->createEntity("worldEntity", "worldMesh");
		Ogre::SceneNode* worldNode = mSceneMgr->getSceneNode("worldNode");
		worldNode->attachObject(worldEntity);
		if (last_x % 2 == 0) {
			worldEntity->setMaterialName("BlackBorder");
		} else {
			worldEntity->setMaterialName("Skull");
		}
	}
		

	return true;
}

void World::extractRegionToOgreMesh(PolyVox::Region& region, Ogre::String meshName) {
	// TODO: implement
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

void World::loadRegion(const PolyVox::ConstVolumeProxy<PolyVox::Material8>& volData, const PolyVox::Region& region) {
	std::stringstream ss;
	ss << "Info: loadRegion() is loading region: " << region.getLowerCorner() << " -> " << region.getUpperCorner();
	Utils::log(ss.str());
	uint8_t material = 2;
	for (int x=region.getLowerCorner().getX(); x<region.getUpperCorner().getX(); x++) {
		for (int y=region.getLowerCorner().getY(); y<region.getUpperCorner().getY(); ++y) {
			for (int z=region.getLowerCorner().getZ(); z<region.getUpperCorner().getZ(); z++) {
				if (y<3) {
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
