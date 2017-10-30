#ifndef __Room_h_
#define __Room_h_

#include "GameObject.h"
#include <OgreMeshManager.h>
#include "Wall.h"
#include <vector>

class Simulator;

class Room : public GameObject {
protected:
    std::vector<Wall*> physicalWalls;
    std::vector<Wall*> visualWalls;
    
    void makePlane(Ogre::Vector3 normal, std::string name, float d1, float d2, Ogre::Vector3 up, float posx, float posy, float posz, Ogre::SceneManager* mSceneMgr, std::string color)
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
        entity->setMaterialName(color);       
    }


    void makePlaneWithPhysics(Ogre::Vector3 normal, std::string name, float d1, float d2, Ogre::Vector3 up, float posx, float posy, float posz, Ogre::SceneManager* mSceneMgr, float btx, float bty, float btz,
        Simulator* physicsEngine)
    {
        btTransform groundTransform;
        groundTransform.setIdentity();
        groundTransform.setOrigin(btVector3(btx, bty, btz));
       
        btScalar groundMass(0);
        btVector3 localGroundInertia(0, 0, 0);
        
        btCollisionShape* groundShape;

        if(normal == Ogre::Vector3::UNIT_Z || normal == Ogre::Vector3::NEGATIVE_UNIT_Z)
            groundShape = new btBoxShape(btVector3(btScalar(d1), btScalar(d2), btScalar(1.)));
        else if (normal == Ogre::Vector3::UNIT_Y || normal == Ogre::Vector3::NEGATIVE_UNIT_Y)
            groundShape = new btBoxShape(btVector3(btScalar(d1), btScalar(1.), btScalar(d2)));        
        else if (normal == Ogre::Vector3::UNIT_X || normal == Ogre::Vector3::NEGATIVE_UNIT_X)
            groundShape = new btBoxShape(btVector3(btScalar(1.), btScalar(d2), btScalar(d1)));        


        btDefaultMotionState* groundMotionState = new btDefaultMotionState(groundTransform);
        
        groundShape->calculateLocalInertia(groundMass, localGroundInertia);
        
        btRigidBody::btRigidBodyConstructionInfo groundRBInfo(groundMass, groundMotionState, groundShape, localGroundInertia);
        btRigidBody* groundBody = new btRigidBody(groundRBInfo);
        groundBody->setRestitution(1);
        groundBody->setFriction(btScalar(1.0));
        groundBody->setRollingFriction(btScalar(1.0));
        
        physicsEngine->getCollisionShapes().push_back(groundShape);
        physicsEngine->getDynamicsWorld()->addRigidBody(groundBody, COL_WALL, COL_BALL | COL_CUEBALL);
    }

public:
    Room(Ogre::SceneManager* _sceneMgr, Simulator* _simulator);
};

#endif