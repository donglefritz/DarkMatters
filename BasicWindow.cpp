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
	mSceneMgr->setAmbientLight(Ogre::ColourValue(1.0f,1.0f,1.0f));

	Utils::addAxesLines(mSceneMgr, 200);

	//PolyVox::LargeVolume<PolyVox::MaterialDensityPair44> largeVolume(PolyVox::Region(PolyVox::Vector3DInt32(0,0,0), PolyVox::Vector3DInt32(256, 256, 256)));
	
	// simple volume:
	PolyVox::SimpleVolume<PolyVox::MaterialDensityPair44> volData(PolyVox::Region(PolyVox::Vector3DInt32(0,0,0), PolyVox::Vector3DInt32(63, 63, 63)));
	Utils::createSphereInPolyVoxVolume(volData, 30);
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
