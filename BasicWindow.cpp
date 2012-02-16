#include "BasicWindow.h"

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
	/*
	// center marker:
	addAxesLines(50);

	// lights:
	mSceneMgr->setAmbientLight(Ogre::ColourValue(1.0f,1.0f,1.0f));
	Ogre::Light* cameraLight = mSceneMgr->createLight("cameraLight");
	mCameraNode->attachObject(cameraLight);
	cameraLight->setType(Ogre::Light::LT_SPOTLIGHT);
	cameraLight->setDiffuseColour(Ogre::ColourValue::Red);
	cameraLight->setSpecularColour(Ogre::ColourValue::Blue);
	cameraLight->setSpotlightRange(Ogre::Degree(35), Ogre::Degree(50));

	// PolyVox:
	PolyVox::Region exampleRegion(PolyVox::Vector3DInt32(-255,0,0), PolyVox::Vector3DInt32(32,32,32));
	PolyVox::LargeVolume<PolyVox::Material8> volData(&Utils::loadRegion, &Utils::unloadRegion, 64);
	volData.setMaxNumberOfBlocksInMemory(4096);
	volData.setMaxNumberOfUncompressedBlocks(32);

	PolyVox::Region region(PolyVox::Vector3DInt32(-255,0,0), PolyVox::Vector3DInt32(28,28,28));

	
	PolyVox::SurfaceMesh<PolyVox::PositionMaterialNormal> mesh;
	PolyVox::CubicSurfaceExtractorWithNormals<PolyVox::LargeVolume,PolyVox::Material8> surfaceExtractor(&volData, region, &mesh);
	surfaceExtractor.execute();

	// OGRE:
	Ogre::ManualObject* mo = mSceneMgr->createManualObject();
	Utils::polyVoxMeshToOgreObject(&mesh, mo);
	mo->convertToMesh("volDataMesh");
	Ogre::Entity* entity   = mSceneMgr->createEntity("volDataMesh");
	Ogre::SceneNode* node  = mSceneMgr->getRootSceneNode()->createChildSceneNode();
	node->attachObject(entity);
	entity->setMaterialName("BlackBorder");
	//node->scale(10, 10, 10);
	*/


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

	//std::stringstream ss;
	//ss << "DEBUG: # vertices in extracted mesh: " << mesh.getNoOfVertices();
	//Utils::log(ss.str());	

	// WORKS: PolyVox::Region reg(PolyVox::Vector3DInt32(-255,0,0), PolyVox::Vector3DInt32(255,255,255));
	// PUKES: PolyVox::Region reg(PolyVox::Vector3DInt32(1,1,1), PolyVox::Vector3DInt32(31,31,31));






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
	Ogre::ManualObject* mo = mSceneMgr->createManualObject();
	Utils::polyVoxMeshToOgreObject(&polyVoxMesh, mo);
	mo->convertToMesh("moMesh");
	Ogre::Entity* entity  = mSceneMgr->createEntity("moMesh");
	Ogre::SceneNode* node = mSceneMgr->getRootSceneNode()->createChildSceneNode("testNode");
	node->translate(1, 200, 1);
	node->attachObject(entity);
	entity->setMaterialName("BlackBorder");
	*/
	
	/*
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

	float moveAmount = 5;

	switch(evt.key) {
	case OIS::KC_ESCAPE:
		mShutDown = true;
		break;
	case OIS::KC_SYSRQ:
		mWindow->writeContentsToTimestampedFile("screenshot", ".jpg");
		break;
	case OIS::KC_HOME:
		mCamera->setPosition(Ogre::Vector3(0,0,5));
		break;
	case OIS::KC_NUMPAD8:	//<- in
		mCamera->setPosition(mCamera->getPosition()+Ogre::Vector3(0,0,-moveAmount));
		break;
	case OIS::KC_NUMPAD2:	//<- out
		mCamera->setPosition(mCamera->getPosition()+Ogre::Vector3(0,0,moveAmount));
		break;
	case OIS::KC_NUMPAD4:	//<- left
		mCamera->setPosition(mCamera->getPosition()+Ogre::Vector3(-moveAmount,0,0));
		break;
	case OIS::KC_NUMPAD6:	//<- right
		mCamera->setPosition(mCamera->getPosition()+Ogre::Vector3(moveAmount,0,0));
		break;
	case OIS::KC_PGUP:		//<- up
		mCamera->setPosition(mCamera->getPosition()+Ogre::Vector3(0,moveAmount,0));
		break;
	case OIS::KC_PGDOWN:	//<- down
		mCamera->setPosition(mCamera->getPosition()+Ogre::Vector3(0,-moveAmount,0));
		break;
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

// HELPERS:

CEGUI::MouseButton BasicWindow::convertMouseButton(OIS::MouseButtonID id) {
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

bool BasicWindow::isMouseOverGUI(void) {
	CEGUI::Window* win  = CEGUI::System::getSingleton().getWindowContainingMouse();
	CEGUI::Window* root = CEGUI::WindowManager::getSingleton().getWindow("root");
	if (win == NULL || win == root)  {
		return false;
	} else {
		return true;
	}
}

void BasicWindow::addAxesLines(float length) {	
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
