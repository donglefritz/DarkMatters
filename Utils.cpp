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

void Utils::createSphereInPolyVoxVolume(PolyVox::SimpleVolume<PolyVox::MaterialDensityPair44>& volData, float radius) {
        PolyVox::Vector3DFloat center(volData.getWidth() / 2.0f, volData.getHeight() / 2.0f, volData.getDepth()/2.0f);

        for (int z = 0; z < volData.getWidth(); z++) {
                for (int y = 0; y < volData.getHeight(); y++) {
                        for (int x = 0; x < volData.getDepth(); x++) {
								PolyVox::Vector3DFloat position((float)x, (float)y, (float)z);
                                float distance = (float)((position-center).length());

                                if (distance <= radius) {
                                        // 8 bit max is 15
                                        uint8_t density  = 15;
										uint8_t material = 15;

                                        PolyVox::MaterialDensityPair44 voxel = volData.getVoxelAt(x,y,z);

                                        voxel.setDensity(density);
										voxel.setMaterial(material);

                                        volData.setVoxelAt(x, y, z, voxel);
                                }
                        }
                }
        }
}

void Utils::addAxesLines(Ogre::SceneManager* mSceneMgr, float length) {	
	Ogre::ManualObject* mo = mSceneMgr->createManualObject("axesLines");
	mo->begin("BaseWhiteNoLighting", Ogre::RenderOperation::OT_LINE_LIST);
	mo->position(-length,0,0);				   //<- 0
	mo->colour(Ogre::ColourValue::Red);
	mo->position(length,0,0);                  //<- 1
	mo->colour(Ogre::ColourValue::Red);
	mo->position(0,-length,0);                 //<- 2
	mo->colour(Ogre::ColourValue::Green);
	mo->position(0,length,0);                  //<- 3
	mo->colour(Ogre::ColourValue::Green);
	mo->position(0,0,-length);                 //<- 4
	mo->colour(Ogre::ColourValue::Blue);   
	mo->position(0,0,length);                  //<- 5
	mo->colour(Ogre::ColourValue::Blue);
	mo->index(0);
	mo->index(1);
	mo->index(2);
	mo->index(3);
	mo->index(4);
	mo->index(5);
	mo->end();
	mSceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject(mo);
}

CEGUI::MouseButton Utils::convertMouseButton(OIS::MouseButtonID id) {
	switch(id) {
	case OIS::MB_Left:
		return CEGUI::LeftButton;
	case OIS::MB_Right:
		return CEGUI::RightButton;
	case OIS::MB_Middle:
		return CEGUI::MiddleButton;
	default:
		return CEGUI::LeftButton;
	}
}

bool Utils::isMouseOverGUI(void) {
	CEGUI::Window* win  = CEGUI::System::getSingleton().getWindowContainingMouse();
	CEGUI::Window* root = CEGUI::WindowManager::getSingleton().getWindow("root");
	if (win == NULL || win == root)  {
		return false;
	} else {
		return true;
	}
}

void Utils::getMeshInformation(Ogre::MeshPtr mesh,
                        size_t &vertex_count,
                        Ogre::Vector3* &vertices,
                        size_t &index_count,
                        unsigned long* &indices,
                        const Ogre::Vector3 &position,
                        const Ogre::Quaternion &orient,
                        const Ogre::Vector3 &scale) {

    bool added_shared = false;
    size_t current_offset = 0;
    size_t shared_offset = 0;
    size_t next_offset = 0;
    size_t index_offset = 0;
 
    vertex_count = index_count = 0;
 
    // Calculate how many vertices and indices we're going to need
    for ( unsigned short i = 0; i < mesh->getNumSubMeshes(); ++i)
    {
        Ogre::SubMesh* submesh = mesh->getSubMesh(i);
        // We only need to add the shared vertices once
        if(submesh->useSharedVertices)
        {
            if( !added_shared )
            {
                vertex_count += mesh->sharedVertexData->vertexCount;
                added_shared = true;
            }
        }
        else
        {
            vertex_count += submesh->vertexData->vertexCount;
        }
        // Add the indices
        index_count += submesh->indexData->indexCount;
    }
 
    // Allocate space for the vertices and indices
    vertices = new Ogre::Vector3[vertex_count];
    indices = new unsigned long[index_count];
 
    added_shared = false;
 
    // Run through the submeshes again, adding the data into the arrays
    for (unsigned short i = 0; i < mesh->getNumSubMeshes(); ++i)
    {
        Ogre::SubMesh* submesh = mesh->getSubMesh(i);
 
        Ogre::VertexData* vertex_data = submesh->useSharedVertices ? mesh->sharedVertexData : submesh->vertexData;
 
        if ((!submesh->useSharedVertices) || (submesh->useSharedVertices && !added_shared))
        {
            if(submesh->useSharedVertices)
            {
                added_shared = true;
                shared_offset = current_offset;
            }
 
            const Ogre::VertexElement* posElem =
                vertex_data->vertexDeclaration->findElementBySemantic(Ogre::VES_POSITION);
 
            Ogre::HardwareVertexBufferSharedPtr vbuf =
                vertex_data->vertexBufferBinding->getBuffer(posElem->getSource());
 
            unsigned char* vertex =
                static_cast<unsigned char*>(vbuf->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));
 
            // There is _no_ baseVertexPointerToElement() which takes an Ogre::Real or a double
            //  as second argument. So make it float, to avoid trouble when Ogre::Real will
            //  be comiled/typedefed as double:
            //Ogre::Real* pReal;
            float* pReal;
 
            for( size_t j = 0; j < vertex_data->vertexCount; ++j, vertex += vbuf->getVertexSize())
            {
                posElem->baseVertexPointerToElement(vertex, &pReal);
                Ogre::Vector3 pt(pReal[0], pReal[1], pReal[2]);
                vertices[current_offset + j] = (orient * (pt * scale)) + position;
            }
 
            vbuf->unlock();
            next_offset += vertex_data->vertexCount;
        }
 
        Ogre::IndexData* index_data = submesh->indexData;
        size_t numTris = index_data->indexCount / 3;
        Ogre::HardwareIndexBufferSharedPtr ibuf = index_data->indexBuffer;
 
        bool use32bitindexes = (ibuf->getType() == Ogre::HardwareIndexBuffer::IT_32BIT);
 
        unsigned long* pLong = static_cast<unsigned long*>(ibuf->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));
        unsigned short* pShort = reinterpret_cast<unsigned short*>(pLong);
 
        size_t offset = (submesh->useSharedVertices)? shared_offset : current_offset;
 
        if ( use32bitindexes )
        {
            for ( size_t k = 0; k < numTris*3; ++k)
            {
                indices[index_offset++] = pLong[k] + static_cast<unsigned long>(offset);
            }
        }
        else
        {
            for ( size_t k = 0; k < numTris*3; ++k)
            {
                indices[index_offset++] = static_cast<unsigned long>(pShort[k]) +
                                          static_cast<unsigned long>(offset);
            }
        }
 
        ibuf->unlock();
        current_offset = next_offset;
    }
}


void Utils::log(Ogre::String msg) {
	Ogre::LogManager::getSingletonPtr()->logMessage(msg);
}
