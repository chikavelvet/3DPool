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

const float STICK_POWER_MAX = 150.0f, STICK_POWER_MIN = 50.0f, STICK_POWER_MULT = 5.0f;

//---------------------------------------------------------------------------
ThreeDPool::ThreeDPool(void) :
mMoveSpeed(750),
hitBall(false),
LMBDown(false),
cueStickDelta(0),
cueStickTotal(STICK_POWER_MIN),
adjustingStick(false),
adjustingCamera(false)
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

    cueBallObject = new Ball(mSceneMgr, physicsEngine, 0, 0, 0, "cueBall");
    cueBall = cueBallObject->getRigidBody();

    cueStickObject = new Stick(mSceneMgr, physicsEngine, 0, 0, 0 + STICK_POWER_MIN, "cueStick", STICK_POWER_MAX, STICK_POWER_MIN, STICK_POWER_MULT, cueBall);
    cueStick = cueStickObject->getRigidBody();
    
    cameraOffset = Ogre::Vector3(mCamera->getPosition()-cueStickObject->getPosition());
    btVector3 btPos = cueStick->getCenterOfMassPosition();
    Ogre::Vector3 cueStickPos(float(btPos.x()),float(btPos.y()), float(btPos.z()));
    mCamera->lookAt(cueStickPos);
    mCamera->setPosition(cueStickPos + cameraOffset);

    room = new Room(mSceneMgr, physicsEngine);

    //----Set up CEGUI----//
    
    mRenderer = &CEGUI::OgreRenderer::bootstrapSystem();
    CEGUI::ImageManager::setImagesetDefaultResourceGroup("Imagesets");
    CEGUI::Font::setDefaultResourceGroup("Fonts");
    CEGUI::Scheme::setDefaultResourceGroup("Schemes");
    CEGUI::WidgetLookManager::setDefaultResourceGroup("LookNFeel");
    CEGUI::WindowManager::setDefaultResourceGroup("Layouts");
    
    CEGUI::SchemeManager::getSingleton().createFromFile("TaharezLook.scheme");
    CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().setDefaultImage("TaharezLook/MouseArrow");
    
    // This will set up a sheet to show the cursor
    CEGUI::WindowManager &wmgr = CEGUI::WindowManager::getSingleton();
    CEGUI::Window *sheet = wmgr.createWindow("DefaultWindow", "CEGUIDemo/Sheet");
    CEGUI::System::getSingleton().getDefaultGUIContext().setRootWindow(sheet);
    
    //--------------------//
    
    //----------MAKE MORE BALLS AS DESIRED-----------//
    Ball* otherBall = new Ball(mSceneMgr, physicsEngine, 0, 0, -200, "otherBall1");
    //....etc.
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
                adjustingCamera = true;
            } else {
                mCamera->setPosition(preFreeLookCameraPosition);
                mCamera->setDirection(preFreeLookCameraDirection);
                adjustingCamera = false;
            }
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
    }
    else {
        cueStickObject->rotateToMouseInput(cueStickRotationX, cueStickRotationY);
        
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
                Ogre::SceneNode *sceneNode = static_cast<Ogre::SceneNode *>(userPointer);
                sceneNode->setPosition(Ogre::Vector3(trans.getOrigin().getX(), trans.getOrigin().getY(), trans.getOrigin().getZ()));
                sceneNode->setOrientation(Ogre::Quaternion(orientation.getW(), orientation.getX(), orientation.getY(), orientation.getZ()));
            }
        }
    }
}

bool ThreeDPool::quit (const CEGUI::EventArgs& e) {
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
