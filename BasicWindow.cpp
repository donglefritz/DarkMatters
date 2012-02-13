#include "BasicWindow.h"

// TEST:

void load(const PolyVox::ConstVolumeProxy<PolyVox::Material8>& volume, const PolyVox::Region& reg) {
	std::stringstream ss;
	ss << "VOLDATA: loading region: " << reg.getLowerCorner() << " -> " << reg.getUpperCorner();
	Utils::log(ss.str());
	Perlin perlin(2,2,1,234);
	for(int x=reg.getLowerCorner().getX(); x<=reg.getUpperCorner().getX(); x++)	{
		for(int y=reg.getLowerCorner().getY(); y<=reg.getUpperCorner().getY(); y++)	{
			float perlinVal = perlin.Get(x / static_cast<float>(255-1), y / static_cast<float>(255-1));
			perlinVal += 1.0f;
			perlinVal *= 0.5f;
			perlinVal *= 255;
			for(int z = reg.getLowerCorner().getZ(); z <= reg.getUpperCorner().getZ(); z++) {
				PolyVox::Material8 voxel;
				if(z < perlinVal) {
					const int xpos = 50;
					const int zpos = 100;
					if((x-xpos)*(x-xpos) + (z-zpos)*(z-zpos) < 200) {
						// tunnel
						voxel.setMaterial(0);
					} else {
						// solid
						voxel.setMaterial(15);
					}
				} else {
					voxel.setMaterial(0);
				}
				volume.setVoxelAt(x, y, z, voxel);
			}
		}
	}
}

void unload(const PolyVox::ConstVolumeProxy<PolyVox::Material8>& volume, const PolyVox::Region& reg) {
	std::stringstream ss;
	ss << "VOLDATA: unloading region: " << reg.getLowerCorner() << " -> " << reg.getUpperCorner();
	Utils::log(ss.str());
}


// CONSTRUCTOR / DESTRUCTOR:

BasicWindow::BasicWindow(void) :
	mRoot(0),
	mPluginsFileName(Ogre::StringUtil::BLANK),
	mResourcesFileName(Ogre::StringUtil::BLANK),
	mShutDown(false),
	mSendMouseToGUI(true),
	mSendKeyboardToGUI(false),
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

	Utils::log("*** Initialising OIS ***");
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
	//mSceneMgr->setAmbientLight(Ogre::ColourValue(0,0,0,0));

	Ogre::Light* cameraLight = mSceneMgr->createLight("cameraLight");
	mCameraNode->attachObject(cameraLight);
	cameraLight->setType(Ogre::Light::LT_SPOTLIGHT);
	cameraLight->setDiffuseColour(Ogre::ColourValue::Red);
	cameraLight->setSpecularColour(Ogre::ColourValue::Blue);
	cameraLight->setSpotlightRange(Ogre::Degree(35), Ogre::Degree(50));

	Utils::addAxesLines(mSceneMgr, 50);



	// POLYVOX:

	// create large volume:
	PolyVox::LargeVolume<PolyVox::Material8> volData(&load, &unload, 256);
	volData.setMaxNumberOfBlocksInMemory(4096);
	volData.setMaxNumberOfUncompressedBlocks(64);


	// testing the volume:
	/*
	ss << "DEBUG: Memory usage: " << (volData.calculateSizeInBytes()/1024.0/1024.0) << "MB" << std::endl;
	ss << "DEBUG: Compression ratio: 1 to " << (1.0/(volData.calculateCompressionRatio())) << std::endl;

	// prefetch:
	PolyVox::Region reg(PolyVox::Vector3DInt32(-255,0,0), PolyVox::Vector3DInt32(255,255,255));
	ss << "DEBUG: Prefetching region: " << reg.getLowerCorner() << " -> " << reg.getUpperCorner() << std::endl;
	volData.prefetch(reg);
	ss << "DEBUG: Memory usage: " << (volData.calculateSizeInBytes()/1024.0/1024.0) << "MB" << std::endl;
	ss << "DEBUG: Compression ratio: 1 to " << (1.0/(volData.calculateCompressionRatio())) << std::endl;

	// flush region:
	PolyVox::Region reg2(PolyVox::Vector3DInt32(0,0,0), PolyVox::Vector3DInt32(255,255,255));
	ss << "DEBUG: Flushing region: " << reg2.getLowerCorner() << " -> " << reg2.getUpperCorner() << std::endl;
	volData.flush(reg2);
	ss << "DEBUG: Memory usage: " << (volData.calculateSizeInBytes()/1024.0/1024.0) << "MB" << std::endl;
	ss << "DEBUG: Compression ratio: 1 to " << (1.0/(volData.calculateCompressionRatio())) << std::endl;

	// flush entire volume:
	ss << "DEBUG: Flushing entire volume" << std::endl;
	volData.flushAll();
	ss << "DEBUG: Memory usage: " << (volData.calculateSizeInBytes()/1024.0/1024.0) << "MB" << std::endl;
	ss << "DEBUG: Compression ratio: 1 to " << (1.0/(volData.calculateCompressionRatio())) << std::endl;
	*/

	// extract the surface

	PolyVox::SurfaceMesh<PolyVox::PositionMaterialNormal> mesh;

	// WTF POLYVOX???????????????????????????????
	// WORKS: PolyVox::Region reg(PolyVox::Vector3DInt32(-255,0,0), PolyVox::Vector3DInt32(255,255,255));
	// PUKES: PolyVox::Region reg(PolyVox::Vector3DInt32(1,1,1), PolyVox::Vector3DInt32(31,31,31));

	PolyVox::Region reg(PolyVox::Vector3DInt32(-255,0,0), PolyVox::Vector3DInt32(255,255,255));
	PolyVox::CubicSurfaceExtractorWithNormals<PolyVox::LargeVolume,PolyVox::Material8> surfaceExtractor(&volData, reg, &mesh);
	surfaceExtractor.execute();

	std::stringstream ss;
	ss << "DEBUG: # vertices in extracted mesh: " << mesh.getNoOfVertices();
	Utils::log(ss.str());

	// OGRE:
	Utils::polyVoxMeshToOgreObject(mSceneMgr, &mesh)->convertToMesh("volDataMesh");
	Ogre::Entity* entity  = mSceneMgr->createEntity("volDataMesh");
	Ogre::SceneNode* node = mSceneMgr->getRootSceneNode()->createChildSceneNode();
	node->attachObject(entity);
	entity->setMaterialName("Leaf");
	node->scale(10, 10, 10);
	// save mesh to file:
    //Ogre::MeshSerializer meshSerializer;
    //meshSerializer.exportMesh(largeMoMesh.getPointer(), "largePerlinNoise.mesh");


	// OLD TESTS:

	/*
	// create simple volume:
	PolyVox::SimpleVolume<PolyVox::MaterialDensityPair44> volData(PolyVox::Region(PolyVox::Vector3DInt32(0,0,0), PolyVox::Vector3DInt32(63, 63, 63)));
	Utils::createSphereInPolyVoxVolume(volData, 30);
	PolyVox::SurfaceMesh<PolyVox::PositionMaterialNormal> polyVoxMesh;
	PolyVox::CubicSurfaceExtractorWithNormals<PolyVox::SimpleVolume,PolyVox::MaterialDensityPair44> surfaceExtractor(&volData, volData.getEnclosingRegion(), &polyVoxMesh);
	surfaceExtractor.execute();

	// add simple volume to scene:
	Ogre::ManualObject* mo = Utils::polyVoxMeshToOgreObject(mSceneMgr, &polyVoxMesh);
	mo->convertToMesh("moMesh");
	Ogre::Entity* entity  = mSceneMgr->createEntity("moMesh");
	Ogre::SceneNode* node = mSceneMgr->getRootSceneNode()->createChildSceneNode("testNode");
	node->translate(1, 200, 1);
	node->attachObject(entity);
	entity->setMaterialName("RotatingCloud");

	// create large volume:
	PolyVox::Vector3DInt32 size(128, 8, 128);
	PolyVox::Vector3DInt32 begin(0, 0, 0);
	PolyVox::Vector3DInt32 end(128, 8, 128);
	PolyVox::Region viewable(begin, end);
	PolyVox::LargeVolume<PolyVox::Material8> largeVolData(PolyVox::Region(begin, size));
	PolyVox::SurfaceMesh<PolyVox::PositionMaterialNormal> largePolyMesh;
	
	uint32_t cubedMaxInMemory = 255;
	largeVolData.setMaxNumberOfBlocksInMemory(cubedMaxInMemory*cubedMaxInMemory*cubedMaxInMemory);

	int maxHeight = 8;
	Utils::randomlyFillRegionOfPolyVoxVolume(largeVolData, begin, end, maxHeight);
	PolyVox::CubicSurfaceExtractorWithNormals<PolyVox::LargeVolume,PolyVox::Material8> largeExtractor(&largeVolData, viewable, &largePolyMesh);
	largeExtractor.execute();

	// add viewable chunk of large volume to scene:
	Ogre::ManualObject* largeMo = Utils::polyVoxMeshToOgreObject(mSceneMgr, &largePolyMesh);
	Ogre::MeshPtr largeMoMesh  = largeMo->convertToMesh("largeMoMesh");
	Ogre::Entity* largeEntity  = mSceneMgr->createEntity("largeMoMesh");
	Ogre::SceneNode* largeNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("largeNode");
	largeNode->attachObject(largeEntity);
	largeEntity->setMaterialName("Leaf");
	largeNode->scale(10, 1, 10);
	*/

}

// CALLBACKS:





bool BasicWindow::frameRenderingQueued(const Ogre::FrameEvent& evt) {
	if (mWindow->isClosed() || mShutDown) { return false; }
	
	mKeyboard->capture();
	mMouse->capture();
		
	mCameraMan->frameRenderingQueued(evt);

	CEGUI::System::getSingleton().injectTimePulse(evt.timeSinceLastFrame);

	// get camera position
	Ogre::Light* cameraLight = mSceneMgr->getLight("cameraLight");
	cameraLight->setPosition(mCamera->getPosition());
	cameraLight->setDirection(mCamera->getDirection());

	

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
		CEGUI::System::getSingleton().injectMouseButtonDown(Utils::convertMouseButton(id));
		if (Utils::isMouseOverGUI()) {
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
		CEGUI::System::getSingleton().injectMouseButtonUp(Utils::convertMouseButton(id));
	}
	return true;
}

bool BasicWindow::quit(const CEGUI::EventArgs& evt) {
	mShutDown = true;
	return true;
}
