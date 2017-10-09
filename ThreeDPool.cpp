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
#include "Ball.h"
#include "Stick.h"
#include "Room.h"

Ogre::Vector3 cameraOffset;
Stick* cueStickObject;
btRigidBody* cueStick;
Ball* cueBallObject;
btRigidBody* cueBall;
Room* room;

//---------------------------------------------------------------------------
ThreeDPool::ThreeDPool(void)
{
}
//---------------------------------------------------------------------------
ThreeDPool::~ThreeDPool(void)
{
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

    gameLoop();
    physicsLoop();

    return true;
}

//---------------------------------------------------------------------------
void ThreeDPool::createScene(void)
{
    adjustingCamera = false;

    //-------------basic setup stuff-----------------//
    mSceneMgr->setAmbientLight(Ogre::ColourValue(0.5, 0.5, 0.5));
    mSceneMgr->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_ADDITIVE);

    physicsEngine = new Simulator();
    physicsEngine->initObjects();

    // makeGround();

    cueBallObject = new Ball(mSceneMgr, physicsEngine, 0, 0, 0, "cueBall");
    cueBall = cueBallObject->getRigidBody();

    float cueStickMax = 100.0f, cueStickMin = 40.0f, powerMultiplier = 0.2f;
    cueStickObject = new Stick(mSceneMgr, physicsEngine, 0, 0, 0 + cueStickMin, "cueStick", cueStickMax, cueStickMin, powerMultiplier, cueBall);
    cueStick = cueStickObject->getRigidBody();
    
    cameraOffset = Ogre::Vector3(mCamera->getPosition()-cueStickObject->getPosition());
    btVector3 btPos = cueStick->getCenterOfMassPosition();
    Ogre::Vector3 cueStickPos(float(btPos.x()),float(btPos.y()), float(btPos.z()));
    mCamera->lookAt(cueStickPos);
    mCamera->setPosition(cueStickPos + cameraOffset);

    room = new Room(mSceneMgr, physicsEngine);

    //----------MAKE MORE BALLS AS DESIRED-----------//
    //Ball* otherBall = new Ball(mSceneMgr, physicsEngine, 100, 500, 300, "otherBall1");
    //....etc.
}

void ThreeDPool::gameLoop()
{
    if(adjustingStick) {
        if(fabs(cueBall->getLinearVelocity().length())>0.f){
            cueStick->setLinearVelocity(btVector3(0, 0, 0));
        }
        bool done = cueStickObject->readjustStickToCueball(adjustingStick);
        if(done) cameraFollowStick();
        adjustingCamera = true;
    }
    else if(adjustingCamera){

    }
    else if(hitBall) {
        cueStickObject->releaseStick(adjustingStick, hitBall, cueStickTotal, cueStickDelta);
    }
    else {
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
    mCamera = mSceneMgr->createCamera("PlayerCam");
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
    mCamera->lookAt(cueStickPos);
    mCamera->setPosition(cueStickPos + cameraOffset);
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
