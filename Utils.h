#ifndef UTILS_H
#define UTILS_H

#include <cstdlib>
#include <time.h>
#include <cmath>
#include <OgreVector3.h>
#include <OgreColourValue.h>
#include <OgreSceneManager.h>
#include <OgreManualObject.h>
#include <OgreSubMesh.h>
#include <OISMouse.h>
#include <CEGUI.h>
// polyvox:
#include "PolyVoxCore/MaterialDensityPair.h"
#include "PolyVoxCore/CubicSurfaceExtractorWithNormals.h"
#include "PolyVoxCore/SurfaceMesh.h"
#include "PolyVoxCore/SimpleVolume.h"
#include <PolyVoxCore/MeshDecimator.h>
#include <PolyVoxCore/CubicSurfaceExtractor.h>
#include <PolyVoxCore/LargeVolume.h>
#include <PolyVoxCore/Material.h>

class Utils {
private:
	Utils(void);
	~Utils(void);

public:
	static bool  randomBool(void);
	static float randomFloat(void);
	static int   randomInt(int excludedMax);

	static float distanceBetween(Ogre::Vector3 p1, Ogre::Vector3 p2);
	static float distanceBetween(int p1_x, int p1_y, int p1_z, int p2_x, int p2_y, int p2_z);

	static void log(Ogre::String msg);

	static void polyVoxMeshToOgreObject(PolyVox::SurfaceMesh<PolyVox::PositionMaterialNormal>* mesh,Ogre::ManualObject* mo);
	static void createSphereInPolyVoxVolume(PolyVox::SimpleVolume<PolyVox::MaterialDensityPair44>& volData, float radius);

	static void randomlyFillRegionOfPolyVoxVolume(PolyVox::LargeVolume<PolyVox::Material8>& volData, PolyVox::Vector3DInt32& begin, PolyVox::Vector3DInt32& end, int maxHeight);

	static void fillRegion(PolyVox::LargeVolume<PolyVox::Material8>& volData, PolyVox::Region& region);

	/*
	static void extractMeshFromRegion(PolyVox::LargeVolume<PolyVox::Material8>& volData, PolyVox::SurfaceMesh<PolyVox::PositionMaterialNormal>* mesh);
	*/


    /* USAGE EXAMPLE:
	 * Ogre::MeshPtr mesh = mSceneMgr->createEntity(Ogre::SceneManager::PT_CUBE)->getMesh();
	 * std::size_t vertexCount, indexCount;
	 * unsigned long* indices;
	 * Ogre::Vector3* vertices;
	 * Ogre::Vector3 position, scale;
	 * Ogre::Quaternion orientation;
	 * getMeshInformation(mesh, vertexCount, vertices, indexCount, indices, position, orientation, scale);
	 */
	static void Utils::getMeshInformation(Ogre::MeshPtr mesh,
                        size_t &vertex_count,
                        Ogre::Vector3* &vertices,
                        size_t &index_count,
                        unsigned long* &indices,
                        const Ogre::Vector3 &position,
                        const Ogre::Quaternion &orient,
                        const Ogre::Vector3 &scale);
};

#endif
