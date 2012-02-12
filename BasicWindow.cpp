#include "BasicWindow.h"

// STATIC HELPERS:

CEGUI::MouseButton convertMouseButton(OIS::MouseButtonID id) {
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

void drawColorfulCube(Ogre::ManualObject* mo) {
	float pos = 100.0f;
	float neg = -pos;
	float lo  = 0.0f;
	float hi  = 1.0f; 

	mo->begin("BaseWhiteNoLighting", Ogre::RenderOperation::OT_TRIANGLE_LIST);

	mo->position(neg, pos, neg);	//<- 0
	mo->colour(lo, hi, lo, hi);

	mo->position(pos, pos, neg);	//<- 1
	mo->colour(hi, hi, lo, hi);

	mo->position(pos, neg, neg);	//<- 2
	mo->colour(hi, lo, lo, hi);

	mo->position(neg, neg, neg);	//<- 3
	mo->colour(lo, lo, lo, hi);

	mo->position(neg, pos, pos);	//<- 4
	mo->colour(lo, hi, hi, hi);

	mo->position(pos, pos, pos);	//<- 5
	mo->colour(hi, hi, hi, hi);

	mo->position(pos, neg, pos);	//<- 6
	mo->colour(hi, lo, hi, hi);

	mo->position(neg, neg, pos);	//<- 7
	mo->colour(lo, lo, hi, hi);

	// back:
	mo->triangle(0,1,2);
	mo->triangle(2,3,0);
	// front:
	mo->triangle(4,6,5);
	mo->triangle(6,4,7);
	// top:
	mo->triangle(0,4,5);
	mo->triangle(5,1,0);
	// bottom:
	mo->triangle(2,6,7);
	mo->triangle(7,3,2);
	// left:
	mo->triangle(0,7,4);
	mo->triangle(7,0,3);
	// right:
	mo->triangle(1,5,6);
	mo->triangle(6,2,1);

	mo->end();
}

void drawCube(Ogre::ManualObject* mo, float size, Ogre::ColourValue& color) {
	{
		float pos = size;
		float neg = -pos;

		mo->begin("BaseWhiteNoLighting", Ogre::RenderOperation::OT_TRIANGLE_LIST);

		mo->position(neg, pos, neg);	//<- 0
		mo->colour(color);
		mo->textureCoord(1,0);

		mo->position(pos, pos, neg);	//<- 1
		mo->colour(color);
		mo->textureCoord(0,0);

		mo->position(pos, neg, neg);	//<- 2
		mo->colour(color);
		mo->textureCoord(1,1);

		mo->position(neg, neg, neg);	//<- 3
		mo->colour(color);
		mo->textureCoord(1,0);

		mo->position(neg, pos, pos);	//<- 4
		mo->colour(color);
		mo->textureCoord(0,0);

		mo->position(pos, pos, pos);	//<- 5
		mo->colour(color);
		mo->textureCoord(0,1);

		mo->position(pos, neg, pos);	//<- 6
		mo->colour(color);
		mo->textureCoord(1,1);

		mo->position(neg, neg, pos);	//<- 7
		mo->colour(color);
		mo->textureCoord(1,0);

		// back:
		mo->triangle(0,1,2);
		mo->triangle(2,3,0);
		// front:
		mo->triangle(4,6,5);
		mo->triangle(6,4,7);
		// top:
		mo->triangle(0,4,5);
		mo->triangle(5,1,0);
		// bottom:
		mo->triangle(2,6,7);
		mo->triangle(7,3,2);
		// left:
		mo->triangle(0,7,4);
		mo->triangle(7,0,3);
		// right:
		mo->triangle(1,5,6);
		mo->triangle(6,2,1);

		mo->end();
	}
	{
		mo->begin("BaseWhiteNoLighting", Ogre::RenderOperation::OT_LINE_LIST);
		float len = 300.0f;
		
		mo->position(-len,0,0);					//<- 0
		mo->colour(Ogre::ColourValue::Red);

		mo->position(len,0,0);                  //<- 1
		mo->colour(Ogre::ColourValue::Red);

		mo->position(0,-len,0);                    //<- 2
		mo->colour(Ogre::ColourValue::Green);

		mo->position(0,len,0);                  //<- 3
		mo->colour(Ogre::ColourValue::Green);

		mo->position(0,0,-len);                    //<- 4
		mo->colour(Ogre::ColourValue::Blue);   

		mo->position(0,0,len);                  //<- 5
		mo->colour(Ogre::ColourValue::Blue);

		mo->index(0);
		mo->index(1);
		mo->index(2);
		mo->index(3);
		mo->index(4);
		mo->index(5);
		
		mo->end();
	}

}

void addAxesLines(Ogre::ManualObject* mo, float length) {
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
}

void addQuadTriangleIndexes(Ogre::ManualObject* mo) {
	mo->index(0);
	mo->index(1);
	mo->index(2);
	mo->index(2);
	mo->index(3);
	mo->index(0);
}

void addQuadBack(Ogre::ManualObject* mo, float size, Ogre::ColourValue& color) {
	float pos = size;
	float neg = -pos;
	// back (const z = neg):
	mo->begin("BaseWhiteNoLighting",Ogre::RenderOperation::OT_TRIANGLE_LIST);
	mo->position(pos, 0, neg);   //<- 0
	mo->textureCoord(1,0);
	mo->colour(color);
	mo->position(pos, neg, neg); //<- 1
	mo->textureCoord(1,1);
	mo->colour(color);
	mo->position(0, neg, neg);   //<- 2
	mo->textureCoord(0,1);
	mo->colour(color);
	mo->position(0, 0, neg);     //<- 3
	mo->textureCoord(0,0); 
	mo->colour(color);
	addQuadTriangleIndexes(mo);
	mo->end();
}

void addQuadTop(Ogre::ManualObject* mo, float size, Ogre::ColourValue& color) {
	float pos = size;
	float neg = -pos;
	// top (const y = 0):
	mo->begin("BaseWhiteNoLighting", Ogre::RenderOperation::OT_TRIANGLE_LIST);
	mo->position(0, 0, neg);   //<- 0
	mo->textureCoord(0,0);
	mo->colour(color);
	mo->position(0,0,0);       //<- 1
	mo->textureCoord(0,1);
	mo->colour(color);
	mo->position(pos,0,0);     //<- 2
	mo->textureCoord(1,1);
	mo->colour(color);
	mo->position(pos,0,neg);   //<- 3
	mo->textureCoord(1,0);
	mo->colour(color);
	addQuadTriangleIndexes(mo);
	mo->end();
}

void addQuadBottom(Ogre::ManualObject* mo, float size, Ogre::ColourValue& color) {
	float pos = size;
	float neg = -pos;
	// bottom (const y = neg):
	mo->begin("BaseWhiteNoLighting", Ogre::RenderOperation::OT_TRIANGLE_LIST);
	mo->position(pos,neg,neg);   //<- 0
	mo->textureCoord(1,0);
	mo->colour(color);
	mo->position(pos,neg,0);     //<- 1
	mo->textureCoord(1,1);
	mo->colour(color);
	mo->position(0,neg,0);       //<- 2
	mo->textureCoord(0,1);
	mo->colour(color);
	mo->position(0, neg, neg);   //<- 3
	mo->textureCoord(0,0);
	mo->colour(color);
	addQuadTriangleIndexes(mo);
	mo->end();
}

void addQuadLeft(Ogre::ManualObject* mo, float size, Ogre::ColourValue& color) {
	float pos = size;
	float neg = -pos;
	// left (const x = 0):
	mo->begin("BaseWhiteNoLighting", Ogre::RenderOperation::OT_TRIANGLE_LIST);
	mo->position(0, 0, neg);   //<- 0
	mo->textureCoord(0,0);
	mo->colour(color);
	mo->position(0,neg,neg);   //<- 1
	mo->textureCoord(0,1);
	mo->colour(color);
	mo->position(0,neg,0);     //<- 2
	mo->textureCoord(1,1);
	mo->colour(color);
	mo->position(0, 0, 0);     //<- 3
	mo->textureCoord(1,0);
	mo->colour(color);
	addQuadTriangleIndexes(mo);
	mo->end();
}

void addQuadRight(Ogre::ManualObject* mo, float size, Ogre::ColourValue& color) {
	float pos = size;
	float neg = -pos;
	// right (const x = pos):
	mo->begin("BaseWhiteNoLighting", Ogre::RenderOperation::OT_TRIANGLE_LIST);
	mo->position(pos, 0, 0);     //<- 0
	mo->textureCoord(1,0);
	mo->colour(color);
	mo->position(pos,neg,0);     //<- 1
	mo->textureCoord(1,1);
	mo->colour(color);
	mo->position(pos,neg,neg);   //<- 2
	mo->textureCoord(0,1);
	mo->colour(color);
	mo->position(pos, 0, neg);   //<- 3
	mo->textureCoord(0,0);
	mo->colour(color);
	addQuadTriangleIndexes(mo);
	mo->end();
}

void addCube(Ogre::ManualObject* mo, float size, Ogre::ColourValue& color) {

	float pos = size;
	float neg = -pos;

	// front (const z = 0):
	mo->begin("BaseWhiteNoLighting",Ogre::RenderOperation::OT_TRIANGLE_LIST);
	mo->position(0, 0, 0);     //<- 0
	mo->textureCoord(0,0); 
	mo->colour(color);
	mo->position(0, neg, 0);   //<- 1
	mo->textureCoord(0,1);
	mo->colour(color);
	mo->position(pos, neg, 0); //<- 2
	mo->textureCoord(1,1);
	mo->colour(color);
	mo->position(pos, 0, 0);   //<- 3
	mo->textureCoord(1,0);
	mo->colour(color);
	addQuadTriangleIndexes(mo);
	mo->end();

	// back (const z = neg):
	mo->begin("BaseWhiteNoLighting",Ogre::RenderOperation::OT_TRIANGLE_LIST);
	mo->position(pos, 0, neg);   //<- 0
	mo->textureCoord(1,0);
	mo->colour(color);
	mo->position(pos, neg, neg); //<- 1
	mo->textureCoord(1,1);
	mo->colour(color);
	mo->position(0, neg, neg);   //<- 2
	mo->textureCoord(0,1);
	mo->colour(color);
	mo->position(0, 0, neg);     //<- 3
	mo->textureCoord(0,0); 
	mo->colour(color);
	addQuadTriangleIndexes(mo);
	mo->end();

	// top (const y = 0):
	mo->begin("BaseWhiteNoLighting", Ogre::RenderOperation::OT_TRIANGLE_LIST);
	mo->position(0, 0, neg);   //<- 0
	mo->textureCoord(0,0);
	mo->colour(color);
	mo->position(0,0,0);       //<- 1
	mo->textureCoord(0,1);
	mo->colour(color);
	mo->position(pos,0,0);     //<- 2
	mo->textureCoord(1,1);
	mo->colour(color);
	mo->position(pos,0,neg);   //<- 3
	mo->textureCoord(1,0);
	mo->colour(color);
	addQuadTriangleIndexes(mo);
	mo->end();

	// bottom (const y = neg):
	mo->begin("BaseWhiteNoLighting", Ogre::RenderOperation::OT_TRIANGLE_LIST);
	mo->position(pos,neg,neg);   //<- 0
	mo->textureCoord(1,0);
	mo->colour(color);
	mo->position(pos,neg,0);     //<- 1
	mo->textureCoord(1,1);
	mo->colour(color);
	mo->position(0,neg,0);       //<- 2
	mo->textureCoord(0,1);
	mo->colour(color);
	mo->position(0, neg, neg);   //<- 3
	mo->textureCoord(0,0);
	mo->colour(color);
	addQuadTriangleIndexes(mo);
	mo->end();

	// left (const x = 0):
	mo->begin("BaseWhiteNoLighting", Ogre::RenderOperation::OT_TRIANGLE_LIST);
	mo->position(0, 0, neg);   //<- 0
	mo->textureCoord(0,0);
	mo->colour(color);
	mo->position(0,neg,neg);   //<- 1
	mo->textureCoord(0,1);
	mo->colour(color);
	mo->position(0,neg,0);     //<- 2
	mo->textureCoord(1,1);
	mo->colour(color);
	mo->position(0, 0, 0);     //<- 3
	mo->textureCoord(1,0);
	mo->colour(color);
	addQuadTriangleIndexes(mo);
	mo->end();

	// right (const x = pos):
	mo->begin("BaseWhiteNoLighting", Ogre::RenderOperation::OT_TRIANGLE_LIST);
	mo->position(pos, 0, 0);     //<- 0
	mo->textureCoord(1,0);
	mo->colour(color);
	mo->position(pos,neg,0);     //<- 1
	mo->textureCoord(1,1);
	mo->colour(color);
	mo->position(pos,neg,neg);   //<- 2
	mo->textureCoord(0,1);
	mo->colour(color);
	mo->position(pos, 0, neg);   //<- 3
	mo->textureCoord(0,0);
	mo->colour(color);
	addQuadTriangleIndexes(mo);
	mo->end();
}

void getMeshInformation(Ogre::MeshPtr mesh,
                        size_t &vertex_count,
                        Ogre::Vector3* &vertices,
                        size_t &index_count,
                        unsigned long* &indices,
                        const Ogre::Vector3 &position,
                        const Ogre::Quaternion &orient,
                        const Ogre::Vector3 &scale) {

    /* USAGE EXAMPLE:
	 * Ogre::MeshPtr mesh = mSceneMgr->createEntity(Ogre::SceneManager::PT_CUBE)->getMesh();
	 * std::size_t vertexCount, indexCount;
	 * unsigned long* indices;
	 * Ogre::Vector3* vertices;
	 * Ogre::Vector3 position, scale;
	 * Ogre::Quaternion orientation;
	 * getMeshInformation(mesh, vertexCount, vertices, indexCount, indices, position, orientation, scale);
	 */

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



void addQuadTopAt(Ogre::ManualObject* mo, Ogre::Vector3& startPosition, float size, Ogre::ColourValue& color) {
	Ogre::Vector3 loc;
	// 0:
	loc = startPosition;
	loc.z -= size;
	mo->position(loc);
	mo->textureCoord(0,0);
	mo->colour(color);
	// 1:
	loc = startPosition;
	mo->position(loc);
	mo->textureCoord(0,1);
	mo->colour(color);
	// 2:
	loc = startPosition;
	loc.x += size;
	mo->position(loc);
	mo->textureCoord(1,1);
	mo->colour(color);
	// 3:
	loc = startPosition;
	loc.x += size;
	loc.z -= size;
	mo->position(loc);
	mo->textureCoord(1,0);
	mo->colour(color);
}

void addQuadBottomAt(Ogre::ManualObject* mo, Ogre::Vector3& startPosition, float size, Ogre::ColourValue& color) {
	Ogre::Vector3 loc;
	// 0:
	loc = startPosition;
	loc.x += size;
	loc.y -= size;
	loc.z -= size;
	mo->position(loc);
	mo->textureCoord(1,0);
	mo->colour(color);
	// 1:
	loc = startPosition;
	loc.x += size;
	loc.y -= size;
	mo->position(loc);
	mo->textureCoord(1,1);
	mo->colour(color);
	// 2:
	loc = startPosition;
	loc.y -= size;
	mo->position(loc);
	mo->textureCoord(0,1);
	mo->colour(color);
	// 3:
	loc = startPosition;
	loc.y -= size;
	loc.z -= size;
	mo->position(loc);
	mo->textureCoord(0,0);
	mo->colour(color);
}

void addQuadFrontAt(Ogre::ManualObject* mo, Ogre::Vector3& startPosition, float size, Ogre::ColourValue& color) {
	Ogre::Vector3 loc;
	// 0:
	loc = startPosition;
	mo->position(loc);
	mo->textureCoord(0,0); 
	mo->colour(color);
	// 1:
	loc = startPosition;
	loc.y -= size;
	mo->position(loc);
	mo->textureCoord(0,1);
	mo->colour(color);
	// 2:
	loc = startPosition;
	loc.x += size;
	loc.y -= size;
	mo->position(loc);
	mo->textureCoord(1,1);
	mo->colour(color);
	// 3:
	loc = startPosition;
	loc.x += size;
	mo->position(loc);
	mo->textureCoord(1,0);
	mo->colour(color);
}

void addQuadBackAt(Ogre::ManualObject* mo, Ogre::Vector3& startPosition, float size, Ogre::ColourValue& color) {
	Ogre::Vector3 loc;
	// 0:
	loc = startPosition;
	loc.x += size;
	loc.z -= size;
	mo->position(loc);
	mo->textureCoord(1,0);
	mo->colour(color);
	// 1:
	loc = startPosition;
	loc.x += size;
	loc.y -= size;
	loc.z -= size;
	mo->position(loc);
	mo->textureCoord(1,1);
	mo->colour(color);
	// 2:
	loc = startPosition;
	loc.y -= size;
	loc.z -= size;
	mo->position(loc);
	mo->textureCoord(0,1);
	mo->colour(color);
	// 3:
	loc = startPosition;
	loc.z -= size;
	mo->position(loc);
	mo->textureCoord(0,0); 
	mo->colour(color);
}

void addQuadLeftAt(Ogre::ManualObject* mo, Ogre::Vector3& startPosition, float size, Ogre::ColourValue& color) {
	Ogre::Vector3 loc;
	// 0:
	loc = startPosition;
	loc.z -= size;
	mo->position(loc);
	mo->textureCoord(0,0);
	mo->colour(color);
	// 1:
	loc = startPosition;
	loc.y -= size;
	loc.z -= size;
	mo->position(loc);
	mo->textureCoord(0,1);
	mo->colour(color);
	// 2:
	loc = startPosition;
	loc.y -= size;
	mo->position(loc);
	mo->textureCoord(1,1);
	mo->colour(color);
	// 3:
	loc = startPosition;
	mo->position(loc);
	mo->textureCoord(1,0);
	mo->colour(color);
}

void addQuadRightAt(Ogre::ManualObject* mo, Ogre::Vector3& startPosition, float size, Ogre::ColourValue& color) {
	Ogre::Vector3 loc;
	// 0:
	loc = startPosition;
	loc.x += size;
	mo->position(loc);
	mo->textureCoord(1,0);
	mo->colour(color);
	// 1:
	loc = startPosition;
	loc.x += size;
	loc.y -= size;
	mo->position(loc);
	mo->textureCoord(1,1);
	mo->colour(color);
	// 2:
	loc = startPosition;
	loc.x += size;
	loc.y -= size;
	loc.z -= size;
	mo->position(loc);
	mo->textureCoord(0,1);
	mo->colour(color);
	// 3:
	loc = startPosition;
	loc.x += size;
	loc.z -= size;
	mo->position(loc);
	mo->textureCoord(0,0);
	mo->colour(color);
}

void addCubeAt(Ogre::ManualObject* mo, Ogre::Vector3& startPosition, float size, Ogre::ColourValue& color, int& index) {
	Ogre::Vector3 loc = startPosition;
	loc.y += size;
	loc.z += size;

	addQuadTopAt(mo, loc, size, color);
	mo->quad(index, index+1, index+2, index+3);
	index += 4;

	addQuadBottomAt(mo, loc, size, color);
	mo->quad(index, index+1, index+2, index+3);
	index += 4;

	addQuadLeftAt(mo, loc, size, color);
	mo->quad(index, index+1, index+2, index+3);
	index += 4;

	addQuadRightAt(mo, loc, size, color);
	mo->quad(index, index+1, index+2, index+3);
	index += 4;

	addQuadBackAt(mo, loc, size, color);
	mo->quad(index, index+1, index+2, index+3);
	index += 4;

	addQuadFrontAt(mo, loc, size, color);
	mo->quad(index, index+1, index+2, index+3);
	index += 4;
}

void randomlyFillVolumeData32(int volData[][32][32], int excludedMax) {
	Utils utils;
	int depth = 32;
	for (int x=0; x<depth; ++x) {
		for (int y=0; y<depth; ++y) {
			for (int z=0; z<depth; ++z) {
				volData[x][y][z] = utils.randomInt(6);
			}
		}
	}
}

void addSpheretoVolumeData32(int volData[][32][32], int radius) {
	Utils utils;
	int depth  = 32;
	int center = depth/2;
	for (int x=0; x<depth; ++x) {
		for (int y=0; y<depth; ++y) {
			for (int z=0; z<depth; ++z) {
				float distance = utils.distanceBetween(center,center,center, x, y, z);
				if (distance < radius) {
					volData[x][y][z] = utils.randomInt(5)+1;
				} else {
					volData[x][y][z] = 0;
				}
			}
		}
	}
}

void addTopHalfOfSphereToVolumeData32(int volData[][32][32], int radius) {
	Utils utils;
	int depth  = 32;
	int center = depth/2;
	for (int x=0; x<depth; ++x) {
		for (int y=0; y<depth; ++y) {
			for (int z=0; z<depth; ++z) {
				float distance = utils.distanceBetween(center,0,center, x, y, z);
				if (distance < radius/2) {
					volData[x][y][z] = utils.randomInt(5)+1;
				} else {
					volData[x][y][z] = 0;
				}
			}
		}
	}
}

void addBottomHalfOfSphereToVolumeData32(int volData[][32][32], int radius) {
	Utils utils;
	int depth  = 32;
	int center = depth/2;
	for (int x=0; x<depth; ++x) {
		for (int y=0; y<depth; ++y) {
			for (int z=0; z<depth; ++z) {
				float distance = utils.distanceBetween(center,depth,center, x, y, z);
				if (distance < radius/2) {
					volData[x][y][z] = utils.randomInt(5)+1;
				} else {
					volData[x][y][z] = 0;
				}
			}
		}
	}
}

void drawVolumeDataToManualObject(Ogre::ManualObject* mo, int volData[][32][32], Ogre::Vector3& startingLocation) {
	int depth  = 32;
	int index  = 0;
	float size = 10.0f;
	Ogre::Vector3 location;

	// TODO: move these to a Colors:: namespace somewhere:
	Ogre::ColourValue red(1.0f,0,0,1.0f);
	Ogre::ColourValue green(0,1.0f,0,1.0f);
	Ogre::ColourValue blue(0,0,1.0f,1.0f);
	Ogre::ColourValue gray(0.7f,0.7f,0.7f,1.0f);
	Ogre::ColourValue other(0.3f, 0.2f, 0.6f, 1.0f);

	for (int x=0; x<depth; ++x) {
		for (int y=0; y<depth; ++y) {
			for (int z=0; z<depth; ++z) {
				if (volData[x][y][z] != 0) {
					location   = startingLocation;
					location.x += x * size;
					location.y += y * size;
					location.z += z * size;
					switch (volData[x][y][z]) {
					case 1:
						addCubeAt(mo, location, size, red, index);
						break;
					case 2:
						addCubeAt(mo, location, size, blue, index);
						break;
					case 3:
						addCubeAt(mo, location, size, green, index);
						break;
					case 4:
						addCubeAt(mo, location, size, gray, index);
						break;
					case 5:
						addCubeAt(mo, location, size, other, index);
						break;
					default:
						break;
					}
				}
			}
		}
	}
}

bool isMouseOverGUI() {
	CEGUI::Window* win  = CEGUI::System::getSingleton().getWindowContainingMouse();
	CEGUI::Window* root = CEGUI::WindowManager::getSingleton().getWindow("root");
	if (win == NULL || win == root)  {
		return false;
	} else {
		return true;
	}
}

// POLYVOX TESTS:

void createSphereInPolyVoxVolume(PolyVox::SimpleVolume<PolyVox::MaterialDensityPair44>& volData, float fRadius) {
        //This vector hold the position of the center of the volume
        PolyVox::Vector3DFloat v3dVolCenter(volData.getWidth() / 2.0f, volData.getHeight() / 2.0f, volData.getDepth()/2.0f);

        //This three-level for loop iterates over every voxel in the volume
        for (int z = 0; z < volData.getWidth(); z++) {
                for (int y = 0; y < volData.getHeight(); y++) {
                        for (int x = 0; x < volData.getDepth(); x++) {
                                //Store our current position as a vector...
								PolyVox::Vector3DFloat v3dCurrentPos((float)x, (float)y, (float)z);
                                //And compute how far the current position is from the center of the volume
                                float distToCenter = (float)((v3dCurrentPos - v3dVolCenter).length());

                                //If the current voxel is less than 'radius' units from the center then we make it solid.
                                if(distToCenter <= fRadius) {
                                        // 8 bit max is 15
                                        uint8_t density  = 15;
										uint8_t material = 15;

                                        PolyVox::MaterialDensityPair44 voxel = volData.getVoxelAt(x,y,z);

                                        voxel.setDensity(density);
										voxel.setMaterial(material);

                                        //Wrte the voxel value into the volume
                                        volData.setVoxelAt(x, y, z, voxel);
                                }
                        }
                }
        }
}

// CONSTRUCTOR / DESTRUCTOR:

BasicWindow::BasicWindow(void) :
	mRoot(0),
	mPluginsFileName(Ogre::StringUtil::BLANK),
	mResourcesFileName(Ogre::StringUtil::BLANK),
	mShutDown(false),
	mSendMouseToGUI(true),
	mSendKeyboardToGUI(false),
	mUtils(),
	mCamera(0),
	mCameraMan(0),
	mKeyboard(0),
	mMouse(0),
	mInputManager(0) {
#ifdef _DEBUG
	mPluginsFileName   = "plugins_d.cfg";
	mResourcesFileName = "resources_d.cfg"; 
#else
	mPluginsFileName   = "plugins.cfg";
	mResourcesFileName = "resources.cfg"; 
#endif
}

BasicWindow::~BasicWindow(void) {
	Ogre::WindowEventUtilities::removeWindowEventListener(mWindow, this);
	windowClosed(mWindow);
	delete mRoot; mRoot=0;
	delete mCameraMan; mCameraMan=0;
}

// INITIALISATION:

bool BasicWindow::go(void) {

	// 1. define the root object:
	
	mRoot = new Ogre::Root(mPluginsFileName);

	// 2. define the resources that ogre will use:

	Ogre::ConfigFile configFile;
	configFile.load(mResourcesFileName);
	Ogre::ConfigFile::SectionIterator sectionsIter = configFile.getSectionIterator();
	Ogre::ConfigFile::SettingsMultiMap::iterator settingsIter;
	Ogre::ConfigFile::SettingsMultiMap* settings;
	Ogre::String secName, typeName, archName;

	while(sectionsIter.hasMoreElements()) {
		secName  = sectionsIter.peekNextKey();
		settings = sectionsIter.getNext();
		for (settingsIter=settings->begin(); settingsIter!=settings->end(); ++settingsIter) {
			typeName = settingsIter->first;
			archName = settingsIter->second;
			Ogre::ResourceGroupManager::getSingleton().addResourceLocation(archName, typeName, secName);
		}
	}

	// 3. choose and setup the render system:

	if (!(mRoot->restoreConfig() || mRoot->showConfigDialog())) {
		return false;
	}

	// 4. create the render window:

	mWindow = mRoot->initialise(true, "Testing");

	// 5. initialise the required resources:

	Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);
	Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

	// 6. setup scene:

	mSceneMgr = mRoot->createSceneManager("DefaultSceneManager");
	
	mCamera   = mSceneMgr->createCamera("mCamera");
	mCamera->setNearClipDistance(1);
	//mCamera->setFarClipDistance(10000);

	mCameraMan = new OgreBites::SdkCameraMan(mCamera);

	Ogre::Viewport* vp = mWindow->addViewport(mCamera);
	//vp->setBackgroundColour(Ogre::ColourValue(1, 1, 1));
	mCamera->setAspectRatio(Ogre::Real(vp->getActualWidth()) / Ogre::Real(vp->getActualHeight()));

	mCameraNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("cameraNode");
	mCameraNode->attachObject(mCamera);

	createGUI();
	createScene();

	// 7. setup 3rd party libraries and plugins:

	log("*** Initialising OIS ***");
	OIS::ParamList paramList;
	size_t windowHandle = 0;
	std::ostringstream windowHandleString;
	mWindow->getCustomAttribute("WINDOW", &windowHandle);
	windowHandleString << windowHandle;
	paramList.insert(std::make_pair(std::string("WINDOW"), windowHandleString.str()));

	mInputManager = OIS::InputManager::createInputSystem(paramList);
	mKeyboard     = static_cast<OIS::Keyboard*>(mInputManager->createInputObject(OIS::OISKeyboard, true));
	mMouse        = static_cast<OIS::Mouse*>(mInputManager->createInputObject(OIS::OISMouse, true));

	windowResized(mWindow);  //<- sets mouse clipping size

	// 8. add listeners and callbacks:

	Ogre::WindowEventUtilities::addWindowEventListener(mWindow, this);
	mRoot->addFrameListener(this);
    mMouse->setEventCallback(this);
    mKeyboard->setEventCallback(this);

	// 9. start the render loop

	mRoot->startRendering();

	return true;
}

void BasicWindow::createGUI(void) {
	mRenderer = &CEGUI::OgreRenderer::bootstrapSystem();
	
	CEGUI::Imageset::setDefaultResourceGroup("Imagesets");
	CEGUI::Font::setDefaultResourceGroup("Fonts");
	CEGUI::Scheme::setDefaultResourceGroup("Schemes");
	CEGUI::WidgetLookManager::setDefaultResourceGroup("LookNFeel");
	CEGUI::WindowManager::setDefaultResourceGroup("Layouts");
	
	CEGUI::SchemeManager::getSingleton().create("TaharezLook.scheme");
	CEGUI::System::getSingleton().setDefaultMouseCursor("TaharezLook", "MouseArrow");

	CEGUI::WindowManager* winMgr = CEGUI::WindowManager::getSingletonPtr();
	CEGUI::System* guiSystem     = CEGUI::System::getSingletonPtr();
	CEGUI::Window* rootWindow    = winMgr->createWindow("DefaultWindow", "root");
	guiSystem->setGUISheet(rootWindow);

	/* not using the gui quite yet:
	try {
		rootWindow->addChildWindow(CEGUI::WindowManager::getSingleton().loadWindowLayout("Console.layout"));
		rootWindow->addChildWindow(CEGUI::WindowManager::getSingleton().loadWindowLayout("RightPanel.layout"));
	} catch (CEGUI::Exception& e) {
		OGRE_EXCEPT(Ogre::Exception::ERR_INTERNAL_ERROR, std::string(e.getMessage().c_str()), "Error parsing gui layout");
	}
	*/

	/*
	// "Quit" button:
	CEGUI::WindowManager& winMgr = CEGUI::WindowManager::getSingleton();
	CEGUI::Window* sheet         = winMgr.createWindow("DefaultWindow", "CEGUIDemo/Sheet");
	CEGUI::Window* quit          = winMgr.createWindow("TaharezLook/Button", "CEGUIDemo/QuitButton");
	quit->setText("Quit");
	quit->setSize(CEGUI::UVector2(CEGUI::UDim(0.15f, 0), CEGUI::UDim(0.05f, 0)));
	quit->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&BasicWindow::quit, this));
	sheet->addChildWindow(quit);
	CEGUI::System::getSingleton().setGUISheet(sheet);
	*/
}

void BasicWindow::createScene(void) {
	mSceneMgr->setAmbientLight(Ogre::ColourValue(1.0f,1.0f,1.0f));

	Ogre::ManualObject* axesLines = mSceneMgr->createManualObject("axesLines");
	mSceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject(axesLines);
	addAxesLines(axesLines, 200);

	// PolyVox test:

	//PolyVox::LargeVolume<PolyVox::MaterialDensityPair44> largeVolume(PolyVox::Region(PolyVox::Vector3DInt32(0,0,0), PolyVox::Vector3DInt32(256, 256, 256)));
	
	// simple volume:
	PolyVox::SimpleVolume<PolyVox::MaterialDensityPair44> volData(PolyVox::Region(PolyVox::Vector3DInt32(0,0,0), PolyVox::Vector3DInt32(63, 63, 63)));
	createSphereInPolyVoxVolume(volData, 30);
	PolyVox::SurfaceMesh<PolyVox::PositionMaterialNormal> polyVoxMesh;
	PolyVox::CubicSurfaceExtractorWithNormals<PolyVox::SimpleVolume,PolyVox::MaterialDensityPair44> surfaceExtractor(&volData, volData.getEnclosingRegion(), &polyVoxMesh);
	surfaceExtractor.execute();

	// add to scene:
	Ogre::ManualObject* mo = Utils::polyVoxMeshToOgreObject(mSceneMgr, &polyVoxMesh);
	mo->convertToMesh("moMesh");
	Ogre::Entity* entity  = mSceneMgr->createEntity("moMesh");
	Ogre::SceneNode* node = mSceneMgr->getRootSceneNode()->createChildSceneNode("testNode");
	node->attachObject(entity);
	entity->setMaterialName("RotatingCloud");


	
}



// UTILITIES:

void BasicWindow::log(Ogre::String msg) {
	Ogre::LogManager::getSingletonPtr()->logMessage(msg);
}

// CALLBACKS:

bool BasicWindow::frameRenderingQueued(const Ogre::FrameEvent& evt) {
	if (mWindow->isClosed() || mShutDown) { return false; }
	
	mKeyboard->capture();
	mMouse->capture();
		
	mCameraMan->frameRenderingQueued(evt);

	CEGUI::System::getSingleton().injectTimePulse(evt.timeSinceLastFrame);

	return true;
}

void BasicWindow::windowResized(Ogre::RenderWindow* rw) {
	unsigned int width, height, depth;
	int left, top;
	rw->getMetrics(width, height, depth, left, top);
	const OIS::MouseState &ms = mMouse->getMouseState();
	ms.width  = width;
	ms.height = height;
}

void BasicWindow::windowClosed(Ogre::RenderWindow* rw) {
	if (rw == mWindow) {
		if (mInputManager) {
			mInputManager->destroyInputObject(mMouse);
			mInputManager->destroyInputObject(mKeyboard);
			OIS::InputManager::destroyInputSystem(mInputManager);
			mInputManager = 0;
		}
	}
}

bool BasicWindow::keyPressed(const OIS::KeyEvent& evt) {
	switch(evt.key) {
	case OIS::KC_ESCAPE:
		mShutDown = true;
		break;
	case OIS::KC_SYSRQ:
		mWindow->writeContentsToTimestampedFile("screenshot", ".jpg");
		break;
	// TODO: catch and send slash commands to GUI console
	// TODO: handle Enter key (submit current action to gui, ex: slash commands)
	default:
		if (mSendKeyboardToGUI) {
			CEGUI::System::getSingleton().injectKeyDown(evt.key);
			CEGUI::System::getSingleton().injectChar(evt.text);
		} else {
			mCameraMan->injectKeyDown(evt);
		}
		break;
	}
	return true;
}

bool BasicWindow::keyReleased(const OIS::KeyEvent& evt) {
	if (mSendKeyboardToGUI) {
		CEGUI::System::getSingleton().injectKeyUp(evt.key);
	} else {
		mCameraMan->injectKeyUp(evt);
	}
	return true;
}

bool BasicWindow::mouseMoved(const OIS::MouseEvent& evt) {
	if (mSendMouseToGUI) {
		CEGUI::System& gui = CEGUI::System::getSingleton();
		gui.injectMouseMove((float)evt.state.X.rel, (float)evt.state.Y.rel);
		if (evt.state.Z.rel) { gui.injectMouseWheelChange(evt.state.Z.rel / 120.0f); }
	} else {
		mCameraMan->injectMouseMove(evt);
	}
	return true;
}

bool BasicWindow::mousePressed(const OIS::MouseEvent& evt, OIS::MouseButtonID id) {
	if (id == OIS::MB_Right) {
		mSendMouseToGUI = false;
	} else {
		CEGUI::System::getSingleton().injectMouseButtonDown(convertMouseButton(id));
		if (isMouseOverGUI()) {
			mSendKeyboardToGUI = true;
		} else {
			mSendKeyboardToGUI = false;
		}
	}
	return true;
}

bool BasicWindow::mouseReleased(const OIS::MouseEvent& evt, OIS::MouseButtonID id) {
	if (id == OIS::MB_Right) {
		mSendMouseToGUI = true;
	} else {
		CEGUI::System::getSingleton().injectMouseButtonUp(convertMouseButton(id));
	}
	return true;
}

bool BasicWindow::quit(const CEGUI::EventArgs& evt) {
	mShutDown = true;
	return true;
}
