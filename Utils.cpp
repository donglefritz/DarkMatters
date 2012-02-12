#include "Utils.h"

Utils::Utils(void){
	srand((unsigned int)time(NULL));
}

Utils::~Utils(void){
}

bool Utils::randomBool(void) {
	return rand()%2==0 ? true : false;
}

float Utils::randomFloat(void) {
	return rand() / static_cast<float>(RAND_MAX);
}

int Utils::randomInt(int excludedMax) {
	return rand() % excludedMax;
}

float Utils::distanceBetween(Ogre::Vector3 p1, Ogre::Vector3 p2) {
	return distanceBetween((int)p1.x, (int)p1.y, (int)p1.z, (int)p2.x, (int)p2.y, (int)p2.z);
}

float Utils::distanceBetween(int p1_x, int p1_y, int p1_z, int p2_x, int p2_y, int p2_z) {
	float x, y, z;
	x  = (float)(p2_x - p1_x);
	x *= x;
	y  = (float)(p2_y - p1_y);
	y *= y;
	z  = (float)(p2_z - p1_z);
	z *= z;
	return sqrtf(x+y+z);
}


Ogre::ManualObject* Utils::polyVoxMeshToOgreObject(Ogre::SceneManager* mSceneMgr, PolyVox::SurfaceMesh<PolyVox::PositionMaterialNormal>* mesh) {
      Ogre::ManualObject* mo = NULL;

	  // make sure mesh is allocated and valid first:
      if(mesh && mesh->getNoOfVertices() > 0 && mesh->getNoOfIndices() > 0) {   

         const std::vector<uint32_t>& indices                         = mesh->getIndices();
         const std::vector<PolyVox::PositionMaterialNormal>& vertices = mesh->getVertices();

         mo = mSceneMgr->createManualObject();

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

         for(unsigned int i=0; i<numVertices; ++i) {
            position = vertices[i].getPosition();
			normal   = vertices[i].getNormal();
			material = vertices[i].getMaterial();
            
			mo->position((Ogre::Real)(position.getX()), (Ogre::Real)(position.getY()), (Ogre::Real)(position.getZ()));
			mo->normal(  (Ogre::Real)(normal.getX()),   (Ogre::Real)(normal.getY()),   (Ogre::Real)(normal.getZ()));
			mo->textureCoord(texCoords[textureIndex++ % 4]);
         }

         for(unsigned int i = 0 ; i < numIndices ; i++) {
            mo->index((Ogre::uint32)(indices[i]));
         }

         mo->end();
      }

      return mo;
   }
