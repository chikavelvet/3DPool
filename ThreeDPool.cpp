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

Ogre::Vector3 cameraOffset;

Stick* cueStickObject;
btRigidBody* cueStick;
Ball* cueBallObject;
btRigidBody* cueBall;

//---------------------------------------------------------------------------
ThreeDPool::ThreeDPool(void)
{
}
//---------------------------------------------------------------------------
ThreeDPool::~ThreeDPool(void)
{
}


void ThreeDPool::buildTable(void) {
//ONE_ONE
    Ogre::Plane one_one(Ogre::Vector3::NEGATIVE_UNIT_Z, 0);
    Ogre::MeshManager::getSingleton().createPlane("_one_one",
        Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
        one_one,
        426, 480, 20, 20,
        true,
        1, 5, 5,
        Ogre::Vector3::UNIT_Y);
    Ogre::Entity* one__one = mSceneMgr->createEntity("_one_one");
    Ogre::SceneNode* sNode11 = mSceneMgr->getRootSceneNode()->createChildSceneNode();
    sNode11->attachObject(one__one);
    sNode11->setPosition(0, 0, 480);
    one__one->setCastShadows(false);
    one__one->setMaterialName("Examples/Rockwall");

//ONE_TWO
    Ogre::Plane one_two(Ogre::Vector3::NEGATIVE_UNIT_Z, 0);
    Ogre::MeshManager::getSingleton().createPlane("_one_two",
        Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
        one_two,
        480, 426, 20, 20,
        true,
        1, 5, 5,
        Ogre::Vector3::UNIT_Y);
    Ogre::Entity* one__two = mSceneMgr->createEntity("_one_two");
    Ogre::SceneNode* sNode12 = mSceneMgr->getRootSceneNode()->createChildSceneNode();
    sNode12->attachObject(one__two);
    sNode12->setPosition(0, 0, 480);
    one__two->setCastShadows(false);
    one__two->setMaterialName("Examples/Rockwall");


//TWO_ONE
    Ogre::Plane two_one(Ogre::Vector3::UNIT_X, 0);
    Ogre::MeshManager::getSingleton().createPlane("_two_one",
        Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
        two_one,
        480, 439.5, 20, 20,
        true,
        1, 5, 5,
        Ogre::Vector3::UNIT_Z);
    Ogre::Entity* two__one = mSceneMgr->createEntity("_two_one");
    Ogre::SceneNode* sNode21 = mSceneMgr->getRootSceneNode()->createChildSceneNode();
    sNode21->attachObject(two__one);
    sNode21->setPosition(-240, 0, 233.25);
    two__one->setCastShadows(false);
    two__one->setMaterialName("Examples/Rockwall");

//TWO_TWO
    Ogre::Plane two_two(Ogre::Vector3::UNIT_X, Ogre::Vector3(0,0,0));
    Ogre::MeshManager::getSingleton().createPlane("_two_two",
        Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
        two_two,
        480, 439.5, 20, 20,
        true,
        1, 5, 5,
        Ogre::Vector3::UNIT_Z);
    Ogre::Entity* two__two = mSceneMgr->createEntity("_two_two");
    Ogre::SceneNode* sNode22 = mSceneMgr->getRootSceneNode()->createChildSceneNode();
    sNode22->attachObject(two__two);
    sNode22->setPosition(-240, 0, -233.25);
    two__two->setCastShadows(false);
    two__two->setMaterialName("Examples/Rockwall");

//TWO_THREE
    Ogre::Plane two_three(Ogre::Vector3::UNIT_X, 0);
    Ogre::MeshManager::getSingleton().createPlane("_two_three",
        Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
        two_three,
        426, 960, 20, 20,
        true,
        1, 5, 5,
        Ogre::Vector3::UNIT_Z);
    Ogre::Entity* two__three = mSceneMgr->createEntity("_two_three");
    Ogre::SceneNode* sNode23 = mSceneMgr->getRootSceneNode()->createChildSceneNode();
    sNode23->attachObject(two__three);
    sNode23->setPosition(-240, 0, 0);
    two__three->setCastShadows(false);
    two__three->setMaterialName("Examples/Rockwall");


//THREE_ONE
    Ogre::Plane three_one(Ogre::Vector3::NEGATIVE_UNIT_Y, 0);
    Ogre::MeshManager::getSingleton().createPlane("_three_one",
        Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
        three_one,
        480, 439.5, 20, 20,
        true,
        1, 5, 5,
        Ogre::Vector3::UNIT_Z);
    Ogre::Entity* three__one = mSceneMgr->createEntity("_three_one");
    Ogre::SceneNode* sNode31 = mSceneMgr->getRootSceneNode()->createChildSceneNode();
    sNode31->attachObject(three__one);
    sNode31->setPosition(0, 240, 233.25);
    three__one->setCastShadows(false);
    three__one->setMaterialName("Examples/Rockwall");

//THREE_TWO
    Ogre::Plane three_two(Ogre::Vector3::NEGATIVE_UNIT_Y, 0);
    Ogre::MeshManager::getSingleton().createPlane("_three_two",
        Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
        three_two,
        480, 439.5, 20, 20,
        true,
        1, 5, 5,
        Ogre::Vector3::UNIT_Z);
    Ogre::Entity* three__two = mSceneMgr->createEntity("_three_two");
    Ogre::SceneNode* sNode32 = mSceneMgr->getRootSceneNode()->createChildSceneNode();
    sNode32->attachObject(three__two);
    sNode32->setPosition(0, 240, -233.25);
    three__two->setCastShadows(false);
    three__two->setMaterialName("Examples/Rockwall");

//THREE_THREE
    Ogre::Plane three_three(Ogre::Vector3::NEGATIVE_UNIT_Y, 0);
    Ogre::MeshManager::getSingleton().createPlane("_three_three",
        Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
        three_three,
        426, 960, 20, 20,
        true,
        1, 5, 5,
        Ogre::Vector3::UNIT_Z);
    Ogre::Entity* three__three = mSceneMgr->createEntity("_three_three");
    Ogre::SceneNode* sNode33 = mSceneMgr->getRootSceneNode()->createChildSceneNode();
    sNode33->attachObject(three__three);
    sNode33->setPosition(0, 240, 0);
    three__three->setCastShadows(false);
    three__three->setMaterialName("Examples/Rockwall");


//FOUR_ONE
    Ogre::Plane four_one(Ogre::Vector3::NEGATIVE_UNIT_X, 0);
    Ogre::MeshManager::getSingleton().createPlane("_four_one",
        Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
        four_one,
        480, 439.5, 20, 20,
        true,
        1, 5, 5,
        Ogre::Vector3::UNIT_Z);
    Ogre::Entity* four__one = mSceneMgr->createEntity("_four_one");
    Ogre::SceneNode* sNode41 = mSceneMgr->getRootSceneNode()->createChildSceneNode();
    sNode41->attachObject(four__one);
    sNode41->setPosition(240, 0, 233.25);
    four__one->setCastShadows(false);
    four__one->setMaterialName("Examples/Rockwall");

//FOUR_TWO
    Ogre::Plane four_two(Ogre::Vector3::NEGATIVE_UNIT_X, 0);
    Ogre::MeshManager::getSingleton().createPlane("_four_two",
        Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
        four_two,
        480, 439.5, 20, 20,
        true,
        1, 5, 5,
        Ogre::Vector3::UNIT_Z);
    Ogre::Entity* four__two = mSceneMgr->createEntity("_four_two");
    Ogre::SceneNode* sNode42 = mSceneMgr->getRootSceneNode()->createChildSceneNode();
    sNode42->attachObject(four__two);
    sNode42->setPosition(240, 0, -233.25);
    four__two->setCastShadows(false);
    four__two->setMaterialName("Examples/Rockwall");

//FOUR_THREE
    Ogre::Plane four_three(Ogre::Vector3::NEGATIVE_UNIT_X, 0);
    Ogre::MeshManager::getSingleton().createPlane("_four_three",
        Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
        four_three,
        426, 960, 20, 20,
        true,
        1, 5, 5,
        Ogre::Vector3::UNIT_Z);
    Ogre::Entity* four__three = mSceneMgr->createEntity("_four_three");
    Ogre::SceneNode* sNode43 = mSceneMgr->getRootSceneNode()->createChildSceneNode();
    sNode43->attachObject(four__three);
    sNode43->setPosition(240, 0, 0);
    four__three->setCastShadows(false);
    four__three->setMaterialName("Examples/Rockwall");


//FIVE_ONE
    Ogre::Plane five_one(Ogre::Vector3::UNIT_Y, 0);
    Ogre::MeshManager::getSingleton().createPlane("_five_one",
        Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
        five_one,
        480, 439.5, 20, 20,
        true,
        1, 5, 5,
        Ogre::Vector3::UNIT_Z);
    Ogre::Entity* five__one = mSceneMgr->createEntity("_five_one");
    Ogre::SceneNode* sNode51 = mSceneMgr->getRootSceneNode()->createChildSceneNode();
    sNode51->attachObject(five__one);
    sNode51->setPosition(0, -240, 233.25);
    five__one->setCastShadows(false);
    five__one->setMaterialName("Examples/Rockwall");

//FIVE_TWO
    Ogre::Plane five_two(Ogre::Vector3::UNIT_Y, 0);
    Ogre::MeshManager::getSingleton().createPlane("_five_two",
        Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
        five_two,
        480, 439.5, 20, 20,
        true, 1, 5, 5,
        Ogre::Vector3::UNIT_Z);
    Ogre::Entity* five__two = mSceneMgr->createEntity("_five_two");
    Ogre::SceneNode* sNode52 = mSceneMgr->getRootSceneNode()->createChildSceneNode();
    sNode52->attachObject(five__two);
    sNode52->setPosition(0, -240, -233.25);
    five__two->setCastShadows(false);
    five__two->setMaterialName("Examples/Rockwall");

//FIVE_THREE
    Ogre::Plane five_three(Ogre::Vector3::UNIT_Y, 0);
    Ogre::MeshManager::getSingleton().createPlane("_five_three",
        Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
        five_three,
        426, 960, 20, 20,
        true,
        1, 5, 5,
        Ogre::Vector3::UNIT_Z);
    Ogre::Entity* five__three = mSceneMgr->createEntity("_five_three");
    Ogre::SceneNode* sNode53 = mSceneMgr->getRootSceneNode()->createChildSceneNode();
    sNode53->attachObject(five__three);
    sNode53->setPosition(0, -240, 0);
    five__three->setCastShadows(false);
    five__three->setMaterialName("Examples/Rockwall");



//SIX_ONE
    Ogre::Plane six_one(Ogre::Vector3::UNIT_Z, 0);
    Ogre::MeshManager::getSingleton().createPlane("_six_one",
        Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
        six_one,
        426, 480, 20, 20,
        true,
        1, 5, 5,
        Ogre::Vector3::UNIT_Y);
    Ogre::Entity* six__one = mSceneMgr->createEntity("_six_one");
    Ogre::SceneNode* sNode61 = mSceneMgr->getRootSceneNode()->createChildSceneNode();
    sNode61->attachObject(six__one);
    sNode61->setPosition(0, 0, -480);
    six__one->setCastShadows(false);
    six__one->setMaterialName("Examples/Rockwall");

//SIX_TWO
    Ogre::Plane six_two(Ogre::Vector3::UNIT_Z, 0);
    Ogre::MeshManager::getSingleton().createPlane("_six_two",
        Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
        six_two,
        480, 426, 20, 20,
        true,
        1, 5, 5,
        Ogre::Vector3::UNIT_Y);
    Ogre::Entity* six__two = mSceneMgr->createEntity("_six_two");
    Ogre::SceneNode* sNode62 = mSceneMgr->getRootSceneNode()->createChildSceneNode();
    sNode62->attachObject(six__two);
    sNode62->setPosition(0, 0, -480);
    six__two->setCastShadows(false);
    six__two->setMaterialName("Examples/Rockwall");
}



void ThreeDPool::createCamera(void){
    //make a new camera and give it a point to look at
    mCamera = mSceneMgr->createCamera("PlayerCam");
    mCamera->setPosition(Ogre::Vector3(1400, 1400, 1400));
    // mCamera->lookAt(Ogre::Vector3(50, 300, 300));
    mCamera->setNearClipDistance(1);
}


void ThreeDPool::gameLoop()
{
    if(adjustingStick) {
        if(fabs(cueBall->getLinearVelocity().length())>0.f){
            cueStick->setLinearVelocity(btVector3(0, 0, 0));
        }
        bool done = cueStickObject->readjustStickToCueball(adjustingStick);
        if(done) cameraFollowStick();
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



bool ThreeDPool::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
    if(mWindow->isClosed())
        return false;

    if(mShutDown)
        return false;

    // Need to capture/update each device
    mKeyboard->capture();
    mMouse->capture();

    doPhysicsForThisFrame();

    return true;
}

//---------------------------------------------------------------------------
void ThreeDPool::createScene(void)
{
    //-------------basic setup stuff-----------------//
    mSceneMgr->setAmbientLight(Ogre::ColourValue(0.5, 0.5, 0.5));
    mSceneMgr->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_ADDITIVE);

    physicsEngine = new Simulator();
    physicsEngine->initObjects();

    makeGround();

    cueBallObject = new Ball(mSceneMgr, physicsEngine, 100, 500, 500, "cueBall");
    cueBall = cueBallObject->getRigidBody();

    float cueStickMax = 200.0f, cueStickMin = 50.0f, powerMultiplier = 0.02f;
    cueStickObject = new Stick(mSceneMgr, physicsEngine, 100, 500, 500 + cueStickMin, "cueStick", cueStickMax, cueStickMin, powerMultiplier, cueBall);
    cueStick = cueStickObject->getRigidBody();
    cameraOffset = Ogre::Vector3(mCamera->getPosition()-cueStickObject->getPosition());
    cameraFollowStick();


    //----------MAKE MORE BALLS AS DESIRED-----------//
    //Ball* otherBall = new Ball(mSceneMgr, physicsEngine, 100, 500, 300, "otherBall1");
    //....etc.
}

void ThreeDPool::makeGround(void)
{
    //---------------make a ground plane------------------------//
    Ogre::Plane plane(Ogre::Vector3::UNIT_Y, 0);//blueprint
    Ogre::MeshManager::getSingleton().createPlane(
        "plane1",
        Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
        Ogre::Plane(Ogre::Vector3::UNIT_Y, 0),
        1500, 1500, 20, 20,
        true,
        1, 5, 5,
        Ogre::Vector3::UNIT_Z);
    Ogre::Entity* plane1 = mSceneMgr->createEntity("plane1");
    Ogre::SceneNode* node1 = mSceneMgr->getRootSceneNode()->createChildSceneNode();
    node1->attachObject(plane1);
    node1->setPosition(0, 0, 0);
    plane1->setCastShadows(false); //obviously we don't want the ground to cast shadows
    plane1->setMaterialName("Examples/Rockwall");//give the ground a material
    
    btTransform groundTransform;
    groundTransform.setIdentity();
    groundTransform.setOrigin(btVector3(0, 10, 0));
    
    btScalar groundMass(0.);
    btVector3 localGroundInertia(0, 0, 0);
    
    btCollisionShape* groundShape = new btBoxShape(btVector3(btScalar(1500.), btScalar(20.), btScalar(1500.)));
    btDefaultMotionState* groundMotionState = new btDefaultMotionState(groundTransform);
    
    groundShape->calculateLocalInertia(groundMass, localGroundInertia);
    
    btRigidBody::btRigidBodyConstructionInfo groundRBInfo(groundMass, groundMotionState, groundShape, localGroundInertia);
    btRigidBody* groundBody = new btRigidBody(groundRBInfo);
    groundBody->setRestitution(1.0);
    groundBody->setFriction(btScalar(1.0));
    groundBody->setRollingFriction(btScalar(1.0));
    physicsEngine->getDynamicsWorld()->addRigidBody(groundBody);
}



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
