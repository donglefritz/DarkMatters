
/*


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


*/