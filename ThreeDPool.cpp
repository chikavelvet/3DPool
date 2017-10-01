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
    mCamera->setPosition(Ogre::Vector3(400, 400, 400));
    mCamera->lookAt(Ogre::Vector3(50, 50, 50));
    mCamera->setNearClipDistance(1);
}

//---------------------------------------------------------------------------
void ThreeDPool::createScene(void)
{
    mSceneMgr->setAmbientLight(Ogre::ColourValue(0.5, 0.5, 0.5));
    mSceneMgr->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_ADDITIVE);

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
    
    Simulator* physicsEngine = new Simulator();
    physicsEngine->initObjects();

    btTransform groundTransform;
    groundTransform.setIdentity();
    groundTransform.setOrigin(btVector3(0, -50, 0));
    
    btScalar groundMass(0.);
    btVector3 localGroundInertia(0, 0, 0);
    
    btCollisionShape* groundShape = new btBoxShape(btVector3(btScalar(50.), btScalar(50.), btScalar(50.)));
    btDefaultMotionState* groundMotionState = new btDefaultMotionState(groundTransform);
    
    groundShape->calculateLocalInertia(groundMass, localGroundInertia);
    
    btRigidBody::btRigidBodyConstructionInfo groundRBInfo(groundMass, groundMotionState, groundShape, localGroundInertia);
    btRigidBody* groundBody = new btRigidBody(groundRBInfo);
    physicsEngine->getDynamicsWorld()->addRigidBody(groundBody);





    Ogre::Entity *entity = this->mSceneMgr->createEntity("cube.mesh"); 
    Ogre::SceneNode *newNode = this->mSceneMgr->getRootSceneNode()->createChildSceneNode("physicsCube");
    newNode->attachObject(entity);
    newNode->setPosition(100, 100, 100);
     
    //create the new shape, and tell the physics that is a Box
    btCollisionShape *newRigidShape = new btBoxShape(btVector3(1.0f, 1.0f, 1.0f));
    physicsEngine->getCollisionShapes().push_back(newRigidShape);
     
    //set the initial position and transform. For this demo, we set the tranform to be none
    btTransform startTransform;
    startTransform.setIdentity();
    startTransform.setRotation(btQuaternion(1.0f, 1.0f, 1.0f, 0));
     
    //set the mass of the object. a mass of "0" means that it is an immovable object
    btScalar mass = 0.1f;
    btVector3 localInertia(0,0,0);
     
    btVector3 initialPosition(100, 100, 100);
    startTransform.setOrigin(initialPosition);
    newRigidShape->calculateLocalInertia(mass, localInertia);
     
    //actually contruvc the body and add it to the dynamics world
    btDefaultMotionState *myMotionState = new btDefaultMotionState(startTransform);
     
    btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, newRigidShape, localInertia);
    btRigidBody *body = new btRigidBody(rbInfo);
    body->setRestitution(1);
    body->setUserPointer(newNode);
     
    physicsEngine->getDynamicsWorld()->addRigidBody(body);
    physicsEngine->trackRigidBodyWithName(body, "physicsCube");

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
