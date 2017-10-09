#ifndef __Room_h_
#define __Room_h_

#include "BaseApplication.h"


class Room {

private:

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



    void makePlane(Ogre::Vector3 normal, std::string name, float d1, float d2, Ogre::Vector3 up, float posx, float posy, float posz, Ogre::SceneManager* mSceneMgr, float btx, float bty, float btz,
        Simulator* physicsEngine)
    {
        Ogre::Plane p(normal, 0);
        Ogre::MeshManager::getSingleton().createPlane(name,
            Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
            p,
            d1, d2, 20, 20,
            true,
            1, 5, 5,
            up);
        Ogre::Entity* entity = mSceneMgr->createEntity(name);
        Ogre::SceneNode* node = mSceneMgr->getRootSceneNode()->createChildSceneNode();
        node->attachObject(entity);
        node->setPosition(posx, posy, posz);
        entity->setCastShadows(false);
        entity->setMaterialName("Examples/Rockwall");       
    }


    void makePlaneWithPhysics(Ogre::Vector3 normal, std::string name, float d1, float d2, Ogre::Vector3 up, float posx, float posy, float posz, Ogre::SceneManager* mSceneMgr, float btx, float bty, float btz,
        Simulator* physicsEngine)
    {
        Ogre::Plane p(normal, 0);
        Ogre::MeshManager::getSingleton().createPlane(name,
            Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
            p,
            d1, d2, 20, 20,
            true,
            1, 5, 5,
            up);
        Ogre::Entity* entity = mSceneMgr->createEntity(name);
        Ogre::SceneNode* node = mSceneMgr->getRootSceneNode()->createChildSceneNode();
        node->attachObject(entity);
        node->setPosition(posx, posy, posz);
        entity->setCastShadows(false);
        entity->setMaterialName("Examples/Rockwall");       


        btTransform groundTransform;
        groundTransform.setIdentity();
        groundTransform.setOrigin(btVector3(btx, bty, btz));
        
        btScalar groundMass(0);
        btVector3 localGroundInertia(0, 0, 0);
        
        btCollisionShape* groundShape = new btBoxShape(btVector3(btScalar(d1), btScalar(20.), btScalar(d2)));
        btDefaultMotionState* groundMotionState = new btDefaultMotionState(groundTransform);
        
        groundShape->calculateLocalInertia(groundMass, localGroundInertia);
        
        btRigidBody::btRigidBodyConstructionInfo groundRBInfo(groundMass, groundMotionState, groundShape, localGroundInertia);
        btRigidBody* groundBody = new btRigidBody(groundRBInfo);
        // groundBody->setRestitution(1.0);
        // groundBody->setFriction(btScalar(1.0));
        // groundBody->setRollingFriction(btScalar(1.0));


        physicsEngine->getCollisionShapes().push_back(groundShape);
        physicsEngine->getDynamicsWorld()->addRigidBody(groundBody);
    }

public:
    ~Room();
    Room();

    Room(Ogre::SceneManager* mSceneMgr, Simulator* physicsEngine) {
        float offset = 0.0f;

        makePlane(Ogre::Vector3::NEGATIVE_UNIT_Z, "_one_one", 426, 480, Ogre::Vector3::UNIT_Y, 0, 0, 480, mSceneMgr, 0, 0, 480 - offset, physicsEngine);
        makePlane(Ogre::Vector3::NEGATIVE_UNIT_Z, "_one_two", 480, 426, Ogre::Vector3::UNIT_Y, 0, 0, 480, mSceneMgr, 0, 0, 480 - offset, physicsEngine);
        makePlane(Ogre::Vector3::UNIT_X, "_two_one", 480, 439.5, Ogre::Vector3::UNIT_Z, -240, 0, 233.25, mSceneMgr, -240 + offset, 0, 233.25, physicsEngine);
        makePlane(Ogre::Vector3::UNIT_X, "_two_two", 480, 439.5, Ogre::Vector3::UNIT_Z, -240, 0, -233.25, mSceneMgr, -240 + offset, 0, -233.25, physicsEngine);
        makePlane(Ogre::Vector3::UNIT_X, "_two_three", 426, 960, Ogre::Vector3::UNIT_Z, -240, 0, 0, mSceneMgr, -240 + offset, 0, 0, physicsEngine);
        makePlane(Ogre::Vector3::NEGATIVE_UNIT_Y, "_three_one", 480, 439.5, Ogre::Vector3::UNIT_Z, 0, 240, 233.25, mSceneMgr, 0, 240 - offset, 233.25, physicsEngine);
        makePlane(Ogre::Vector3::NEGATIVE_UNIT_Y, "_three_two", 480, 439.5, Ogre::Vector3::UNIT_Z, 0, 240, -233.25, mSceneMgr, 0, 240 - offset, -233.25, physicsEngine);
        makePlane(Ogre::Vector3::NEGATIVE_UNIT_Y, "_three_three", 426, 960, Ogre::Vector3::UNIT_Z, 0, 240, 0, mSceneMgr, 0, 240, 0 - offset, physicsEngine);
        makePlane(Ogre::Vector3::NEGATIVE_UNIT_X, "_four_one", 480, 439.5, Ogre::Vector3::UNIT_Z, 240, 0, 233.25, mSceneMgr, 240 - offset, 0, 233.25, physicsEngine);
        makePlane(Ogre::Vector3::NEGATIVE_UNIT_X, "_four_two", 480, 439.5, Ogre::Vector3::UNIT_Z, 240, 0, -233.25, mSceneMgr, 240 - offset, 0, -233.25, physicsEngine);
        makePlane(Ogre::Vector3::NEGATIVE_UNIT_X, "_four_three", 426, 960, Ogre::Vector3::UNIT_Z, 240, 0, 0, mSceneMgr, 240 - offset, 0, 0, physicsEngine);
        makePlane(Ogre::Vector3::UNIT_Y, "_five_one", 480, 439.5, Ogre::Vector3::UNIT_Z, 0, -240, 233.25, mSceneMgr, 0, -240 + offset, 233.25, physicsEngine);
        makePlane(Ogre::Vector3::UNIT_Y, "_five_two", 480, 439.5, Ogre::Vector3::UNIT_Z, 0, -240, -233.25, mSceneMgr, 0, -240 + offset, -233.25, physicsEngine);
        makePlane(Ogre::Vector3::UNIT_Y, "_five_three", 426, 960, Ogre::Vector3::UNIT_Z, 0, -240, 0, mSceneMgr, 0, -240 + offset, 0, physicsEngine);
        makePlane(Ogre::Vector3::UNIT_Z, "_six_one", 426, 480, Ogre::Vector3::UNIT_Y, 0, 0, -480, mSceneMgr, 0, 0, -480 + offset, physicsEngine);
        makePlane(Ogre::Vector3::UNIT_Z, "_six_two", 480, 426, Ogre::Vector3::UNIT_Y, 0, 0, -480, mSceneMgr, 0, 0, -480 + offset, physicsEngine);
    }
};

#endif