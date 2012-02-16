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


	int size   = 6;
	int depth  = 8;
	int count  = 0;
	int offset = -(size*depth); 

	Ogre::Entity* entity;
	Ogre::SceneNode* node;
	std::stringstream name;

	for (int xx=0; xx<depth; ++xx) {
		for (int zz=0; zz<depth; ++zz) {
			int x = size * xx;
			int z = size * zz;
			name.str("");
			name << "entity" << count;
			Ogre::String entityName = name.str();
			entity = mSceneMgr->createEntity(name.str(), "TwoBySixBlock.mesh");
			name.str("");
			name << "node" << count;
			Ogre::String nodeName = name.str();
			node   = mSceneMgr->getRootSceneNode()->createChildSceneNode(name.str());
			node->attachObject(entity);
			node->translate(x+offset, 0, z+offset);
			if (xx%2==0) {
				if (count%2==0) {
					entity->setMaterialName("BlackBorder");
				} else {
					entity->setMaterialName("Skull");
				}
			} else {
				if (count%2==0) {
					entity->setMaterialName("Skull");
				} else {
					entity->setMaterialName("BlackBorder");
				}
			}
			count++;
		}
	}



	/*
	Ogre::Entity* entity;
	Ogre::SceneNode* node;
	std::stringstream name;
	for (int i=0; i<NUM_SHAPES; ++i) {
		name.str("");
		name << "entity" << i;
		Ogre::String entityName = name.str();
		entity = mSceneMgr->createEntity(name.str(), "TwoBySixBlock.mesh");
		name.str("");
		name << "node" << i;
		Ogre::String nodeName = name.str();
		node   = mSceneMgr->getRootSceneNode()->createChildSceneNode(name.str());
		node->attachObject(entity);
		node->translate((float), 0, 0);
		if (i%2==0) {
			entity->setMaterialName("BlackBorder");
		} else {
			entity->setMaterialName("Skull");
		}
	}

	*/

	/*
	//PolyVox::Region region(PolyVox::Vector3DInt32(-255,0,0), PolyVox::Vector3DInt32(8,8,8));
	//PolyVox::Region region(PolyVox::Vector3DInt32(-1,0,0), PolyVox::Vector3DInt32(8,8,8));
	PolyVox::Region region(PolyVox::Vector3DInt32(0,0,0), PolyVox::Vector3DInt32(8,8,8));
	PolyVox::SurfaceMesh<PolyVox::PositionMaterialNormal> mesh;
	PolyVox::CubicSurfaceExtractorWithNormals<PolyVox::LargeVolume,PolyVox::Material8> surfaceExtractor(mWorldData, region, &mesh);
	surfaceExtractor.execute();


	Ogre::ManualObject* mo = mSceneMgr->createManualObject("mo");
	Ogre::Vector3 offset   = Ogre::Vector3::ZERO;
	polyVoxMeshToOgreObject(&mesh, mo);
	Ogre::MeshPtr ogreMesh = mo->convertToMesh("worldMesh");
	Ogre::Entity* entity   = mSceneMgr->createEntity("worldEntity","worldMesh");
	Ogre::SceneNode* node  = mSceneMgr->getRootSceneNode()->createChildSceneNode("worldNode");
	node->attachObject(entity);
	entity->setMaterialName("BlackBorder");

	// save mesh to file:
    Ogre::MeshSerializer meshSerializer;
    meshSerializer.exportMesh(ogreMesh.getPointer(), "TwoBySixBlock.mesh");
	*/
}

bool World::frameRenderingQueued(const Ogre::FrameEvent& evt) {
	if (!BasicWindow::frameRenderingQueued(evt)) { return false; }
	/*
	static Ogre::Vector3 lastCamPos(Ogre::Vector3::ZERO);

	int shapeLength       = 7;
	float maxDist         = shapeLength * 0.51;
	Ogre::Vector3 camPos  = mCamera->getPosition();
	Ogre::Vector3 camDiff = camPos - lastCamPos;
	float absDiffX        = abs(camDiff.x);
	float absDiffZ        = abs(camDiff.z);

	if (lastCamPos == camPos || (absDiffX < maxDist && absDiffZ < maxDist) { return true; }

	*/
 


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
