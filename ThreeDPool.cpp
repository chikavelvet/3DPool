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

void ThreeDPool::createCamera(void){
    //make a new camera and give it a point to look at
    mCamera = mSceneMgr->createCamera("PlayerCam");
    mCamera->setPosition(Ogre::Vector3(1400, 1400, 1400));
    // mCamera->lookAt(Ogre::Vector3(50, 300, 300));
    mCamera->setNearClipDistance(1);
}


void ThreeDPool::doPhysicsForThisFrame()
{
    if(adjustingStick) {
        if(fabs(cueBall->getLinearVelocity().length())>0.f){
            // cueStick->setDamping(1, 0);    
            cueStick->setLinearVelocity(btVector3(0, 0, 0));
        }
        bool done = readjustStickToCueball();
        if(done) cameraFollowStick();
    }
    else if(hitBall) {
        releaseStick();
    }
    else {
        moveStick();
    }
    if (physicsEngine != NULL){
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
}

bool ThreeDPool::readjustStickToCueball(void){
    if(fabs(cueStick->getLinearVelocity().length())<0.5f){
        cueStick->activate(false);

    }

    bool turnIsOver = (fabs(cueStick->getLinearVelocity().length())<0.5f && fabs(cueBall->getLinearVelocity().length())<0.5f);
    if(!turnIsOver) return false;
    std::cout << turnIsOver << std::boolalpha << std::endl;

    btVector3 ballPos = cueBall->getCenterOfMassPosition();
    btTransform newTransform = cueStick->getCenterOfMassTransform();
    newTransform.setOrigin(btVector3(ballPos.getX(), ballPos.getY(), ballPos.getZ() + cueStickMin));
    cueStick->setCenterOfMassTransform(newTransform);
    adjustingStick = false;
    cueStick->activate(true);
    return true;
}

void ThreeDPool::cameraFollowStick(void)
{
    btVector3 btPos = cueStick->getCenterOfMassPosition();
    Ogre::Vector3 cueStickPos(float(btPos.x()),float(btPos.y()), float(btPos.z()));
    mCamera->lookAt(cueStickPos);
    mCamera->setPosition(cueStickPos + cameraOffset);
}

void ThreeDPool::moveStick(void){
    if(adjustingStick)
        return;

    cueStick->activate(true);
    
    if(cueStickTotal < cueStickMax && cueStickTotal > 0.0f) {
        if(cueStickTotal + cueStickDelta > cueStickMax)
            cueStickDelta = cueStickMax - cueStickTotal;
        if(cueStickTotal + cueStickDelta < cueStickMin){
            cueStickTotal = cueStickMin;
            cueStickDelta = 0.0f;
            return;
        }
        if(LMBDown) {
            cueStick->translate(btVector3(0.f, 0.f, cueStickDelta));
        }
    }

    cueStickTotal += cueStickDelta;
    cueStickDelta = 0;
}

void ThreeDPool::releaseStick(void) {
    if(cueStickTotal > 0.0f){
        cueStick->activate(true);
        cueStick->applyCentralImpulse( btVector3( 0.f, 0.f, -powerMultiplier * cueStickTotal * cueStickTotal) );
    }
    cueStickTotal = 0;
    cueStickDelta = 0;
    adjustingStick = true;
    hitBall = false;
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
    powerMultiplier = 0.02f;

    //-------------basic setup stuff-----------------//
    mSceneMgr->setAmbientLight(Ogre::ColourValue(0.5, 0.5, 0.5));
    mSceneMgr->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_ADDITIVE);

    physicsEngine = new Simulator();
    physicsEngine->initObjects();

    makeGround();

    cueBallObject = new Ball(mSceneMgr, physicsEngine, 100, 500, 500, "cueBall");
    cueBall = cueBallObject->getRigidBody();
    // makeBall(100, 500, 300, "randomBall");

    cueStickObject = new Stick(mSceneMgr, physicsEngine, 100, 500, 500 + cueStickMin, "cueStick");
    cueStick = cueStickObject->getRigidBody();
    // cueStick = makeCueStick(100, 500, 500 + cueStickMin, "cueStick");

    cameraOffset = Ogre::Vector3(mCamera->getPosition()-cueStickObject->getPosition());

    cameraFollowStick();
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

// btRigidBody* ThreeDPool::makeCueStick(btScalar x, btScalar y, btScalar z, std::string name)
// {
//     Ogre::Entity *entity = this->mSceneMgr->createEntity("cube.mesh"); 
//     Ogre::SceneNode *newNode = this->mSceneMgr->getRootSceneNode()->createChildSceneNode(name);
//     newNode->attachObject(entity);
//     newNode->setPosition(x, y, z);
//     newNode->scale(0.05, 0.05, 0.5);
     
//     //create the new shape, and tell the physics that is a sphere
//     btCollisionShape *newRigidShape = new btBoxShape(btVector3(5, 5, 23));
//     physicsEngine->getCollisionShapes().push_back(newRigidShape);
//     btTransform startTransform;
//     startTransform.setIdentity();
//     startTransform.setRotation(btQuaternion(0.0f, 0.0f, 0.0f, 1));
     
//     //set the mass of the object. a mass of "0" means that it is an immovable object
//     btScalar mass = 10;
//     btVector3 localInertia(0,0,0);
     
//     btVector3 initialPosition(x, y, z);
//     startTransform.setOrigin(initialPosition);
//     newRigidShape->calculateLocalInertia(mass, localInertia);
     
//     //actually contruvc the body and add it to the dynamics world
//     btDefaultMotionState *myMotionState = new btDefaultMotionState(startTransform); 
//     btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, newRigidShape, localInertia);
//     btRigidBody *body = new btRigidBody(rbInfo);
//     body->setUserPointer(newNode);
//     body->setRestitution(2.0);
//     body->setFriction(btScalar(1.0));
//     body->setRollingFriction(btScalar(1.0));
//     body->setDamping(0.5, 0);


//     physicsEngine->getDynamicsWorld()->addRigidBody(body);
//     physicsEngine->trackRigidBodyWithName(body, name);    
    

//     cameraOffset = Ogre::Vector3(mCamera->getPosition()-Ogre::Vector3(float(x), float(y), float(z)));

//     return body;
// }

// btRigidBody* ThreeDPool::makeBall(btScalar x, btScalar y, btScalar z, std::string name){
//         //----------------make a cube-------------------//
//         Ogre::Entity *entity = this->mSceneMgr->createEntity("sphere.mesh"); 
//         Ogre::SceneNode *newNode = this->mSceneMgr->getRootSceneNode()->createChildSceneNode(name);
//         newNode->attachObject(entity);
//         newNode->setPosition(x, y, z);
//         newNode->scale(0.1, 0.1, 0.1);
         
//         //create the new shape, and tell the physics that is a sphere
//         btCollisionShape *newRigidShape = new btSphereShape(9);
//         physicsEngine->getCollisionShapes().push_back(newRigidShape);
//         btTransform startTransform;
//         startTransform.setIdentity();
//         startTransform.setRotation(btQuaternion(0.0f, 0.0f, 0.0f, 1));
         
//         //set the mass of the object. a mass of "0" means that it is an immovable object
//         btScalar mass = 5;
//         btVector3 localInertia(0,0,0);
         
//         btVector3 initialPosition(x, y, z);
//         startTransform.setOrigin(initialPosition);
//         newRigidShape->calculateLocalInertia(mass, localInertia);
         
//         //actually contruvc the body and add it to the dynamics world
//         btDefaultMotionState *myMotionState = new btDefaultMotionState(startTransform); 
//         btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, newRigidShape, localInertia);
//         btRigidBody *body = new btRigidBody(rbInfo);
//         body->setUserPointer(newNode);
//         body->setRestitution(0.8);
//         body->setFriction(btScalar(1.0));
//         body->setRollingFriction(btScalar(1.0));
//         body->setDamping(0.1, 0);


//         physicsEngine->getDynamicsWorld()->addRigidBody(body);
//         physicsEngine->trackRigidBodyWithName(body, name);    
//         return body;
// }

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
