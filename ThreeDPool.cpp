/*
-----------------------------------------------------------------------------
Filename:    TutorialApplication.cpp
-----------------------------------------------------------------------------

This source file is part of the
   ___                 __    __ _ _    _
  /___\__ _ _ __ ___  / / /\ \ (_) | _(_)
 //  // _` | '__/ _ \ \ \/  \/ / | |/ / |
/ \_// (_| | | |  __/  \  /\  /| |   <| |
\___/ \__, |_|  \___|   \/  \/ |_|_|\_\_|
      |___/
Tutorial Framework (for Ogre 1.9)
http://www.ogre3d.org/wiki/
-----------------------------------------------------------------------------
*/

#include <OGRE/OgreMeshManager.h>

#include "ThreeDPool.h"

Ogre::Vector3 cameraOffset;
Stick* cueStickObject;
btRigidBody* cueStick;
Ball* cueBallObject;
btRigidBody* cueBall;
Room* room;

Ogre::Vector3 preFreeLookCameraPosition;
Ogre::Vector3 preFreeLookCameraDirection;

const float CUE_STICK_MAX = 150.0f, CUE_STICK_MIN = 50.0f, STICK_POWER_MULT = 0.05f;

std::vector<Ball*> balls;

int remainingBalls;

//---------------------------------------------------------------------------
ThreeDPool::ThreeDPool(void) :
    mMoveSpeed(750),
    hitBall(false),
    LMBDown(false),
    cueStickDelta(0),
    cueStickTotal(CUE_STICK_MIN),
    adjustingStick(false),
    adjustingCamera(false),
    cursorDisplaying(false),
    soundOn(true),
    strokes(0),
    cameraCounter(0),
    typeMap(),
    pocketMap()
{
}
//---------------------------------------------------------------------------
ThreeDPool::~ThreeDPool(void)
{
    delete pCamera;
}

//---------------------------------------------------------------------------
void ThreeDPool::createScene(void)
{
    //-------------basic setup stuff-----------------//
    mSceneMgr->setAmbientLight(Ogre::ColourValue(0.5, 0.5, 0.5));
    mSceneMgr->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_ADDITIVE);

    physicsEngine = new Simulator();
    physicsEngine->initObjects();

    // makeGround();

    cueBallObject = new Ball(mSceneMgr, physicsEngine, 0, 0, 240, "cueBall", typeMap, pocketMap, "Example/White", true);
    cueBall = cueBallObject->getRigidBody();

    cueStickObject = new Stick(mSceneMgr, physicsEngine, 0, 0, 240 + CUE_STICK_MIN, "cueStick", CUE_STICK_MAX, CUE_STICK_MIN, STICK_POWER_MULT, cueBall, typeMap);
    cueStick = cueStickObject->getRigidBody();
    
    cameraOffset = Ogre::Vector3(mCamera->getPosition()-cueStickObject->getPosition());
    btVector3 btPos = cueStick->getCenterOfMassPosition();
    Ogre::Vector3 cueStickPos(float(btPos.x()),float(btPos.y()), float(btPos.z()));
    mCamera->lookAt(cueStickPos);
    mCamera->setPosition(cueStickPos + cameraOffset);

    room = new Room(mSceneMgr, physicsEngine);
    
    addPockets();

    //----Set up CEGUI----//
    
    mRenderer = &CEGUI::OgreRenderer::bootstrapSystem();
    CEGUI::ImageManager::setImagesetDefaultResourceGroup("Imagesets");
    CEGUI::Font::setDefaultResourceGroup("Fonts");
    CEGUI::Scheme::setDefaultResourceGroup("Schemes");
    CEGUI::WidgetLookManager::setDefaultResourceGroup("LookNFeel");
    CEGUI::WindowManager::setDefaultResourceGroup("Layouts");
    
    CEGUI::SchemeManager::getSingleton().createFromFile("TaharezLook.scheme");
    CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().setDefaultImage("TaharezLook/MouseArrow");
    
    // This will set up a default sheet to show the cursor
    //    CEGUI::WindowManager &wmgr = CEGUI::WindowManager::getSingleton();
    //    CEGUI::Window *sheet = wmgr.createWindow("DefaultWindow", "CEGUIDemo/Sheet");
    //    CEGUI::System::getSingleton().getDefaultGUIContext().setRootWindow(sheet);
    
    // This will show a demo GUI window
    //    CEGUI::Window *guiRoot = CEGUI::WindowManager::getSingleton().loadLayoutFromFile("TextDemo.layout"); 
    //    CEGUI::System::getSingleton().getDefaultGUIContext().setRootWindow(guiRoot);
    
    CEGUI::WindowManager &wmgr = CEGUI::WindowManager::getSingleton();
    CEGUI::Window *sheet = wmgr.createWindow("DefaultWindow", "ThreeDPool/Sheet");
    CEGUI::GUIContext& context = CEGUI::System::getSingleton().getDefaultGUIContext();
    context.setRootWindow(sheet);
    
    CEGUI::Window *quit = wmgr.createWindow("TaharezLook/Button", "QuitButton");
    quit->setText("Quit");    
        
    // In UDim, only set one of the two params, the other should be 0
    quit->setSize(CEGUI::USize(CEGUI::UDim(0.15, 0), CEGUI::UDim(0.05, 0)));
    quit->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&ThreeDPool::quit, this));
    
    sheet->addChild(quit);
    
    CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().hide();
    quit->hide();
    
    // Stroke counter
    CEGUI::Window *strokesWin = wmgr.createWindow("TaharezLook/StaticText", "StrokeCount");
    std::stringstream ss;
    ss << "Strokes: " << strokes;
    strokesWin->setText(ss.str());
    strokesWin->setSize(CEGUI::USize(CEGUI::UDim(0.1, 0), CEGUI::UDim(0.05, 0)));
    strokesWin->setPosition(CEGUI::UVector2(CEGUI::UDim(0.85, 0), CEGUI::UDim(0.9, 0)));
    
    sheet->addChild(strokesWin);
        
    //----------MAKE MORE BALLS AS DESIRED-----------//
//    for (int i = 0; i < 100; ++i) {
//        std::stringstream ss;
//        ss << "b" << i;
//        std::string bname = ss.str();
//        std::cout << bname << std::endl;
//        balls.push_back(new Ball(mSceneMgr, physicsEngine, i % 10, i % 50 , i, bname, typeMap, pocketMap));
//    }
    balls.push_back(new Ball(mSceneMgr, physicsEngine, 200, 200, 400, "ball1", typeMap, pocketMap, "Example/Red"));
    
    remainingBalls = balls.size();
    
    // Remaining Ball Counter
    CEGUI::Window *remainingBallWin = wmgr.createWindow("TaharezLook/StaticText", "RemainingBalls");
    std::stringstream ss2;
    ss2 << "Remaining: " << remainingBalls;
    remainingBallWin->setText(ss2.str());
    remainingBallWin->setSize(CEGUI::USize(CEGUI::UDim(0.15, 0), CEGUI::UDim(0.05, 0)));
    remainingBallWin->setPosition(CEGUI::UVector2(CEGUI::UDim(0.85, 0), CEGUI::UDim(0.1, 0)));
    sheet->addChild(remainingBallWin);
    
    CEGUI::Window *youWin = wmgr.createWindow("TaharezLook/StaticText", "YouWin");
    youWin->setText("You Win!");
    youWin->setSize(CEGUI::USize(CEGUI::UDim(0.1, 0), CEGUI::UDim(0.05, 0)));
    youWin->setPosition(CEGUI::UVector2(CEGUI::UDim(0.45, 0), CEGUI::UDim(0.45, 0)));

    sheet->addChild(youWin);
    
    youWin->hide();
    //--------------------//
    
    // 0. 0. -200

    
    //....etc.

    SDL_Init(SDL_INIT_VIDEO);
    int audio_rate = 22050;
    Uint16 audio_format = AUDIO_S16SYS;
    int audio_channels = 2;
    int audio_buffers = 4096; 
    
    Mix_OpenAudio(audio_rate, audio_format, audio_channels, audio_buffers);

    Mix_AllocateChannels(64);
    ball_ball = Mix_LoadWAV("cueball_hit_other.wav");
    stick_ball = Mix_LoadWAV("cue_strike_ball.wav");
    pocket = Mix_LoadWAV("pool_ball_into_pocket.wav");
}

void ThreeDPool::addPockets() {
    // 12 Pockets
    Pocket* p1 = new Pocket(mSceneMgr, physicsEngine, 
            -240, -240, 480,
            "p1", typeMap);
    Pocket* p2 = new Pocket(mSceneMgr, physicsEngine, 
            -240, -240, 0,
            "p2", typeMap);
    Pocket* p3 = new Pocket(mSceneMgr, physicsEngine, 
            -240, -240, -480,
            "p3", typeMap);
    Pocket* p4 = new Pocket(mSceneMgr, physicsEngine, 
            -240, 240, 480,
            "p4", typeMap);
    Pocket* p5 = new Pocket(mSceneMgr, physicsEngine, 
            -240, 240, 0,
            "p5", typeMap);
    Pocket* p6 = new Pocket(mSceneMgr, physicsEngine, 
            -240, 240, -480,
            "p6", typeMap);
    Pocket* p7 = new Pocket(mSceneMgr, physicsEngine, 
            240, -240, 480,
            "p7", typeMap);
    Pocket* p8 = new Pocket(mSceneMgr, physicsEngine, 
            240, -240, 0,
            "p8", typeMap);
    Pocket* p9 = new Pocket(mSceneMgr, physicsEngine, 
            240, -240, -480,
            "p9", typeMap);
    Pocket* p10 = new Pocket(mSceneMgr, physicsEngine, 
            240, 240, 480,
            "p10", typeMap);
    Pocket* p11 = new Pocket(mSceneMgr, physicsEngine, 
            240, 240, 0,
            "p11", typeMap);
    Pocket* p12 = new Pocket(mSceneMgr, physicsEngine, 
            240, 240, -480,
            "p12", typeMap);
    
}

void ThreeDPool::incrementStrokeCount() {    
    CEGUI::GUIContext& context = CEGUI::System::getSingleton().getDefaultGUIContext();
    CEGUI::Window* sheet = context.getRootWindow();
    CEGUI::Window* strokesWin = sheet->getChild("StrokeCount");
    std::stringstream ss;
    
    ++strokes;
    ss << "Strokes: " << strokes;
    strokesWin->setText(ss.str());
}

void ThreeDPool::decrementRemainingBallCount() {
    CEGUI::GUIContext& context = CEGUI::System::getSingleton().getDefaultGUIContext();
    CEGUI::Window* sheet = context.getRootWindow();
    CEGUI::Window* remainingBallWin = sheet->getChild("RemainingBalls");
    std::stringstream ss;
    
    Mix_PlayChannel(-1, pocket, 0);
    
    --remainingBalls;
    ss << "Remaining: " << remainingBalls;
    remainingBallWin->setText(ss.str());
    
    if (remainingBalls < 1) {
        CEGUI::Window* youWin = sheet->getChild("YouWin");
        youWin->show();
    }
}

void ThreeDPool::displayQuitCursor () {
    CEGUI::GUIContext& context = CEGUI::System::getSingleton().getDefaultGUIContext();
    CEGUI::Window* sheet = context.getRootWindow();
    CEGUI::Window* quit = sheet->getChild("QuitButton");
    
    context.getMouseCursor().show();
    quit->show();
}

void ThreeDPool::hideQuitCursor () {
    CEGUI::GUIContext& context = CEGUI::System::getSingleton().getDefaultGUIContext();
    CEGUI::Window* sheet = context.getRootWindow();
    CEGUI::Window* quit = sheet->getChild("QuitButton");
    
    context.getMouseCursor().hide();
    quit->hide();
}

void ThreeDPool::createFrameListener() {
    Ogre::LogManager::getSingletonPtr()->logMessage("*** Initializing OIS ***");
    OIS::ParamList pl;
    size_t windowHnd = 0;
    std::ostringstream windowHndStr;
 
    mWindow->getCustomAttribute("WINDOW", &windowHnd);
    windowHndStr << windowHnd;
    pl.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));
 
    mInputManager = OIS::InputManager::createInputSystem( pl );
 
    mKeyboard = static_cast<OIS::Keyboard*>(mInputManager->createInputObject( OIS::OISKeyboard, true ));
    mMouse = static_cast<OIS::Mouse*>(mInputManager->createInputObject( OIS::OISMouse, true ));
 
    mMouse->setEventCallback(this);
    mKeyboard->setEventCallback(this);
 
    //Set initial mouse clipping size
    windowResized(mWindow);
 
    //Register as a Window listener
    Ogre::WindowEventUtilities::addWindowEventListener(mWindow, this);
 
    mRoot->addFrameListener(this);
}

bool ThreeDPool::keyPressed(const OIS::KeyEvent& arg) {
    // CEGUI Injection
    CEGUI::GUIContext& context = CEGUI::System::getSingleton().getDefaultGUIContext();
    context.injectKeyDown((CEGUI::Key::Scan)arg.key);
    context.injectChar((CEGUI::Key::Scan)arg.text);
    return true;
}

bool ThreeDPool::keyReleased(const OIS::KeyEvent &arg) {
    // CEGUI Injection
    CEGUI::System::getSingleton().getDefaultGUIContext().injectKeyUp((CEGUI::Key::Scan)arg.key);
    
    if (!BaseApplication::keyReleased(arg))
        return false;
    
    switch(arg.key) {
        case OIS::KC_T :
            if (!adjustingCamera){
                preFreeLookCameraPosition = mCamera->getPosition();
                preFreeLookCameraDirection = mCamera->getDirection();
            } else {
                mCamera->setPosition(preFreeLookCameraPosition);
                mCamera->setDirection(preFreeLookCameraDirection);
            }
            adjustingCamera = !adjustingCamera;
            break;
        case OIS::KC_ESCAPE :
            if (!cursorDisplaying)
                displayQuitCursor();
            else 
                hideQuitCursor();
            cursorDisplaying = !cursorDisplaying;
            break;
        case OIS::KC_Y:
            soundOn = !soundOn;
            break;
    }
    return true;
}

bool ThreeDPool::mouseMoved(const OIS::MouseEvent &me) {
    // CEGUI Injection
    CEGUI::GUIContext& context = CEGUI::System::getSingleton().getDefaultGUIContext();
    context.injectMouseMove(me.state.X.rel, me.state.Y.rel);
    // Scroll wheel.
    if (me.state.Z.rel)
        context.injectMouseWheelChange(me.state.Z.rel / 120.0f);

    if (!BaseApplication::mouseMoved(me))
        return false;
    
    if (!adjustingCamera) {
        if(me.state.buttonDown(OIS::MB_Left))
        {
            cueStickDelta = me.state.Y.rel * 0.05;
        }
        else{
            cueStickRotationX = 0.01 * me.state.X.rel;
            cueStickRotationY = 0.01 * me.state.Y.rel;
        }
    } else {
       mCamera->yaw(Ogre::Degree(-0.13 * me.state.X.rel));
       mCamera->pitch(Ogre::Degree(-0.13 * me.state.Y.rel));
    }
}

CEGUI::MouseButton convertButton(OIS::MouseButtonID buttonID)
{
    switch (buttonID)
    {
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

bool ThreeDPool::mouseReleased(const OIS::MouseEvent &me, OIS::MouseButtonID id)
{
    // CEGUI Injection
    CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseButtonUp(convertButton(id));
    
    // if(!BaseApplication::mouseReleased(me, id))
    //     return false;
    if(fabs(cueStickTotal) > 0.1)
        if(id==OIS::MB_Left)
            hitBall = true;
    return true;
}

bool ThreeDPool::mousePressed(const OIS::MouseEvent &me, OIS::MouseButtonID id)
{
    // CEGUI Injection
    CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseButtonDown(convertButton(id));
    
    if(!BaseApplication::mousePressed(me, id))
        return false;

    using namespace std;
    switch(id)
    {
        case OIS::MB_Left:
            cout << "Left" << endl;
            LMBDown = true;
            break;
        case OIS::MB_Right:
            break;
        case OIS::MB_Middle:
            break;
        default:
            break;
    }
    return true;
}


bool ThreeDPool::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
    if(mWindow->isClosed())
        return false;

    if(mShutDown)
        return false;

    // Need to capture/update each device
    mKeyboard->capture();
    mMouse->capture();

    //Need to inject timestamps to CEGUI System.
    CEGUI::System::getSingleton().injectTimePulse(evt.timeSinceLastFrame);
    
    gameLoop(evt);
    physicsLoop();

    return true;
}

void ThreeDPool::gameLoop(const Ogre::FrameEvent& evt)
{
    if(adjustingStick) {
        if(fabs(cueBall->getLinearVelocity().length())>0.01f){
            cueStick->setLinearVelocity(btVector3(0, 0, 0));
        }
        bool done = cueStickObject->readjustStickToCueball(adjustingStick);
        if(done) cameraFollowStick();
        // adjustingCamera = true;
    } else if(adjustingCamera){
        using namespace Ogre;
        Vector3 camDirVec = Vector3::ZERO;
        Real thisMove = mKeyboard->isKeyDown(OIS::KC_LSHIFT) ? 
            mMoveSpeed : mMoveSpeed / 2;
    
        if (mKeyboard->isKeyDown(OIS::KC_W))
            camDirVec += (mCamera->getDirection() * thisMove);

        if (mKeyboard->isKeyDown(OIS::KC_S))
            camDirVec -= (mCamera->getDirection() * thisMove);

        if (mKeyboard->isKeyDown(OIS::KC_A))
            camDirVec -= (mCamera->getDirection().crossProduct(mCamera->getUp()) * thisMove);

        if (mKeyboard->isKeyDown(OIS::KC_D))
            camDirVec += (mCamera->getDirection().crossProduct(mCamera->getUp()) * thisMove);
        
        mCamera->move(camDirVec * evt.timeSinceLastFrame);
    } else if(hitBall) {
        cueStickObject->releaseStick(adjustingStick, hitBall, cueStickTotal, cueStickDelta);
        incrementStrokeCount();
    }
    else {
        cueStickObject->rotateToMouseInput(cueStickRotationX, cueStickRotationY);
        pCamera->moveCameraToStick(cueStickObject);
        cueStickObject->chargeStick(adjustingStick, cueStickTotal, cueStickDelta, LMBDown);
    }
}

void ThreeDPool::physicsLoop()
{
    if (physicsEngine == NULL)
        return;

    physicsEngine->getDynamicsWorld()->stepSimulation(1.0f/60.0f); //suppose you have 60 frames per second
    int length = physicsEngine->getDynamicsWorld()->getCollisionObjectArray().size();
    for (int i = 0; i< length; i++) {
        btCollisionObject* obj = physicsEngine->getDynamicsWorld()->getCollisionObjectArray()[i];
        btRigidBody* body = btRigidBody::upcast(obj);
        if (body && body->getMotionState()){
            btTransform trans;
            body->getMotionState()->getWorldTransform(trans);
            void *userPointer = body->getUserPointer();
            if (userPointer) {
                btQuaternion orientation = trans.getRotation();
                Ogre::SceneNode *sceneNode = static_cast<Ogre::SceneNode*>(userPointer);
                
                sceneNode->setOrientation(Ogre::Quaternion(orientation.getW(), orientation.getX(), orientation.getY(), orientation.getZ()));                
                sceneNode->setPosition(Ogre::Vector3(trans.getOrigin().getX(), trans.getOrigin().getY(), trans.getOrigin().getZ()));
            }
        }
    }

   
    int numManifolds = physicsEngine->getDynamicsWorld()->getDispatcher()->getNumManifolds();
    for (int i = 0; i < numManifolds; i++)
    {
        btPersistentManifold* contactManifold =  physicsEngine->getDynamicsWorld()->getDispatcher()->getManifoldByIndexInternal(i);
        const btCollisionObject* obA = contactManifold->getBody0();
        const btCollisionObject* obB = contactManifold->getBody1();

        objType obAType = typeMap[(size_t) (obA->getUserPointer())];
        objType obBType = typeMap[(size_t) (obB->getUserPointer())];
 
        if (soundOn) {
            if((obAType == stickType && obBType == cueBallType) || (obBType == stickType && obAType == cueBallType))
                Mix_PlayChannel(-1, stick_ball, 0);
            else if(obAType == ballType && obBType == ballType)
                Mix_PlayChannel(-1, ball_ball, 0);
            else if((obAType == ballType && obBType == cueBallType) || (obBType == ballType && obAType == cueBallType))
                Mix_PlayChannel(-1, ball_ball, 0);
        }
        
        if((obAType == pocketType && obBType == ballType) || (obBType == pocketType && obAType == ballType)) {
            void* usr = obAType == ballType ? obA->getUserPointer() : obB->getUserPointer();
            Ogre::SceneNode* node = static_cast<Ogre::SceneNode*>(usr);
                        
            Ball* ball = pocketMap[node];
            
            ball->removeFromWorld();
            
            decrementRemainingBallCount();
        }
        
            
        if((obAType == pocketType && obBType == cueBallType) || (obBType == pocketType && obAType == cueBallType)) {
            void* usr = obAType == cueBallType ? obA->getUserPointer() : obB->getUserPointer();
            Ogre::SceneNode* node = static_cast<Ogre::SceneNode*>(usr);

            Ball* cueBall = pocketMap[node];
            
            incrementStrokeCount();
            
            cueBall->resetCueBall();
        }
    }
}

bool ThreeDPool::quit (const CEGUI::EventArgs& e) {
    mShutDown = true;
    return true;
}

// void ThreeDPool::makeGround(void)
// {
//     //---------------make a ground plane------------------------//
//     Ogre::Plane plane(Ogre::Vector3::UNIT_Y, 0);//blueprint
//     Ogre::MeshManager::getSingleton().createPlane(
//         "plane1",
//         Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
//         Ogre::Plane(Ogre::Vector3::UNIT_Y, 0),
//         1500, 1500, 20, 20,
//         true,
//         1, 5, 5,
//         Ogre::Vector3::UNIT_Z);
//     Ogre::Entity* plane1 = mSceneMgr->createEntity("plane1");
//     Ogre::SceneNode* node1 = mSceneMgr->getRootSceneNode()->createChildSceneNode();
//     node1->attachObject(plane1);
//     node1->setPosition(0, 0, 0);
//     plane1->setCastShadows(false); //obviously we don't want the ground to cast shadows
//     plane1->setMaterialName("Examples/Rockwall");//give the ground a material
    
//     btTransform groundTransform;
//     groundTransform.setIdentity();
//     groundTransform.setOrigin(btVector3(0, 10, 0));
    
//     btScalar groundMass(0.);
//     btVector3 localGroundInertia(0, 0, 0);
    
//     btCollisionShape* groundShape = new btBoxShape(btVector3(btScalar(1500.), btScalar(20.), btScalar(1500.)));
//     btDefaultMotionState* groundMotionState = new btDefaultMotionState(groundTransform);
    
//     groundShape->calculateLocalInertia(groundMass, localGroundInertia);
    
//     btRigidBody::btRigidBodyConstructionInfo groundRBInfo(groundMass, groundMotionState, groundShape, localGroundInertia);
//     btRigidBody* groundBody = new btRigidBody(groundRBInfo);
//     groundBody->setRestitution(1.0);
//     groundBody->setFriction(btScalar(1.0));
//     groundBody->setRollingFriction(btScalar(1.0));
//     physicsEngine->getDynamicsWorld()->addRigidBody(groundBody);
// }


//--------Camera Section-------//
void ThreeDPool::createCamera(void){
    pCamera = new PlayerCamera(mSceneMgr->createCamera("PlayerCam"));
    mCamera = pCamera->getCamera();
    mCamera->setPosition(Ogre::Vector3(200, 200, 200));
    mCamera->setNearClipDistance(1);

    // mCamera->lookAt(Ogre::Vector3(50, 300, 300));
}

// void ThreeDPool::startCameraAdjust(){
//     cameraCounter = 30;
//     btVector3 btPos = cueStick->getCenterOfMassPosition();
//     Ogre::Vector3 cueStickPos(float(btPos.x()),float(btPos.y()), float(btPos.z()));

//     newLookAt = cueStickPos;
//     newCamPos = cueStickPos + cameraOffset;
// }

// void ThreeDPool::cameraAdjust(){
//     assert(cameraCounter >= 0);
//     if(cameraCounter==0){
//         adjustingCamera = false;
//         return;
//     }
//     --cameraCounter;

//     mCamera->lookAt()

//     btVector3 btPos = cueStick->getCenterOfMassPosition();
//     Ogre::Vector3 cueStickPos(float(btPos.x()),float(btPos.y()), float(btPos.z()));
//     mCamera->lookAt(cueStickPos);
//     mCamera->setPosition(cueStickPos + cameraOffset);

// }

void ThreeDPool::cameraFollowStick(void)
{
    btVector3 btPos = cueStick->getCenterOfMassPosition();
    Ogre::Vector3 cueStickPos(float(btPos.x()),float(btPos.y()), float(btPos.z()));
    mCamera->setPosition(cueStickPos + cameraOffset);
    mCamera->lookAt(cueStickPos);
}

//---------------------------------------------------------------------------

//#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
//#define WIN32_LEAN_AND_MEAN
//#include "windows.h"
//#endif

#ifdef __cplusplus
extern "C" {
#endif

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
    INT WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT)
#else
    int main(int argc, char *argv[])
#endif
    {
        // Create application object
        ThreeDPool app;

        try {
            app.go();
        } catch(Ogre::Exception& e)  {
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
            MessageBox(NULL, e.getFullDescription().c_str(), "An exception has occurred!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
#else
            std::cerr << "An exception has occurred: " <<
                e.getFullDescription().c_str() << std::endl;
#endif
        }

        return 0;
    }

#ifdef __cplusplus
}
#endif

//---------------------------------------------------------------------------
