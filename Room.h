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
    Room(Ogre::SceneManager* _sceneMgr, Simulator* _simulator) :
            physicalWalls(),
            visualWalls()
    {
        float offset = 0.0f;
        
        physicalWalls.push_back(Wall::MakePhysicalWall(
            _simulator, btVector3(            0,             0, 480 - offset), Ogre::Vector3::NEGATIVE_UNIT_Z, 426, 480  ));
        physicalWalls.push_back(Wall::MakePhysicalWall(
            _simulator, btVector3(            0,             0, 480 - offset), Ogre::Vector3::NEGATIVE_UNIT_Z, 480, 426  ));
        
        physicalWalls.push_back(Wall::MakePhysicalWall(
            _simulator, btVector3(-240 + offset,             0,       233.25), Ogre::Vector3::UNIT_X,          480, 439.5));  
        physicalWalls.push_back(Wall::MakePhysicalWall(
            _simulator, btVector3(-240 + offset,             0,      -233.25), Ogre::Vector3::UNIT_X,          480, 439.5));    
        physicalWalls.push_back(Wall::MakePhysicalWall(
            _simulator, btVector3(-240 + offset,             0,            0), Ogre::Vector3::UNIT_X,          426, 426  ));
        
        physicalWalls.push_back(Wall::MakePhysicalWall(
            _simulator, btVector3(            0,  240 - offset,       233.25), Ogre::Vector3::NEGATIVE_UNIT_Y, 480, 439.5));     
        physicalWalls.push_back(Wall::MakePhysicalWall(
            _simulator, btVector3(            0,  240 - offset,      -233.25), Ogre::Vector3::NEGATIVE_UNIT_Y, 480, 439.5));
        physicalWalls.push_back(Wall::MakePhysicalWall(
            _simulator, btVector3(            0,           240,   0 - offset), Ogre::Vector3::NEGATIVE_UNIT_Y, 426, 960  ));
        
        physicalWalls.push_back(Wall::MakePhysicalWall(
            _simulator, btVector3( 240 - offset,             0,       233.25), Ogre::Vector3::NEGATIVE_UNIT_X, 480, 439.5));     
        physicalWalls.push_back(Wall::MakePhysicalWall(
            _simulator, btVector3( 240 - offset,             0,      -233.25), Ogre::Vector3::NEGATIVE_UNIT_X, 480, 439.5));
        physicalWalls.push_back(Wall::MakePhysicalWall(
            _simulator, btVector3( 240 - offset,             0,            0), Ogre::Vector3::NEGATIVE_UNIT_X, 426, 960  ));
        
        physicalWalls.push_back(Wall::MakePhysicalWall(
            _simulator, btVector3(            0, -240 + offset,       233.25), Ogre::Vector3::UNIT_Y,          480, 439.5));     
        physicalWalls.push_back(Wall::MakePhysicalWall(
            _simulator, btVector3(            0, -240 + offset,      -233.25), Ogre::Vector3::UNIT_Y,          480, 439.5));
        physicalWalls.push_back(Wall::MakePhysicalWall(
            _simulator, btVector3(            0, -240 + offset,            0), Ogre::Vector3::UNIT_Y,          426, 960  ));
        
        physicalWalls.push_back(Wall::MakePhysicalWall(
            _simulator, btVector3(            0,             0, -480 + offset), Ogre::Vector3::UNIT_Z,          426, 480  ));
        physicalWalls.push_back(Wall::MakePhysicalWall(
            _simulator, btVector3(            0,             0, -480 - offset), Ogre::Vector3::UNIT_Z,          480, 426  ));
        
//        makePlaneWithPhysics(Ogre::Vector3::NEGATIVE_UNIT_Z, "_one_one", 426, 480, Ogre::Vector3::UNIT_Y, 0, 0, 480, _sceneMgr, 0, 0, 480 - offset, _simulator);
//        makePlaneWithPhysics(Ogre::Vector3::NEGATIVE_UNIT_Z, "_one_two", 480, 426, Ogre::Vector3::UNIT_Y, 0, 0, 479, _sceneMgr, 0, 0, 480 - offset, _simulator);

//        makePlaneWithPhysics(Ogre::Vector3::UNIT_X, "_two_one", 480, 439.5, Ogre::Vector3::UNIT_Z, -240, 0, 233.25, _sceneMgr, -240 + offset, 0, 233.25, _simulator);
//        makePlaneWithPhysics(Ogre::Vector3::UNIT_X, "_two_two", 480, 439.5, Ogre::Vector3::UNIT_Z, -240, 0, -233.25, _sceneMgr, -240 + offset, 0, -233.25, _simulator);
//        makePlaneWithPhysics(Ogre::Vector3::UNIT_X, "_two_three", 426, 960, Ogre::Vector3::UNIT_Z, -239, 0, 0, _sceneMgr,       -240 + offset, 0, 0, _simulator);

//        makePlaneWithPhysics(Ogre::Vector3::NEGATIVE_UNIT_Y, "_three_one", 480, 439.5, Ogre::Vector3::UNIT_Z, 0, 240, 233.25, _sceneMgr,  0, 240 - offset, 233.25, _simulator);
//        makePlaneWithPhysics(Ogre::Vector3::NEGATIVE_UNIT_Y, "_three_two", 480, 439.5, Ogre::Vector3::UNIT_Z, 0, 240, -233.25, _sceneMgr, 0, 240 - offset, -233.25, _simulator);
//        makePlaneWithPhysics(Ogre::Vector3::NEGATIVE_UNIT_Y, "_three_three", 426, 960, Ogre::Vector3::UNIT_Z, 0, 239, 0, _sceneMgr,       0, 240,          0 - offset, _simulator);

//        makePlaneWithPhysics(Ogre::Vector3::NEGATIVE_UNIT_X, "_four_one", 480, 439.5, Ogre::Vector3::UNIT_Z, 240, 0, 233.25, _sceneMgr,  240 - offset, 0, 233.25, _simulator);
//        makePlaneWithPhysics(Ogre::Vector3::NEGATIVE_UNIT_X, "_four_two", 480, 439.5, Ogre::Vector3::UNIT_Z, 240, 0, -233.25, _sceneMgr, 240 - offset, 0, -233.25, _simulator);
//        makePlaneWithPhysics(Ogre::Vector3::NEGATIVE_UNIT_X, "_four_three", 426, 960, Ogre::Vector3::UNIT_Z, 239, 0, 0, _sceneMgr,       240 - offset, 0, 0, _simulator);

//        makePlaneWithPhysics(Ogre::Vector3::UNIT_Y, "_five_one", 480, 439.5, Ogre::Vector3::UNIT_Z, 0, -240, 233.25, _sceneMgr,  0, -240 + offset, 233.25, _simulator);
//        makePlaneWithPhysics(Ogre::Vector3::UNIT_Y, "_five_two", 480, 439.5, Ogre::Vector3::UNIT_Z, 0, -240, -233.25, _sceneMgr, 0, -240 + offset, -233.25, _simulator);
//        makePlaneWithPhysics(Ogre::Vector3::UNIT_Y, "_five_three", 426, 960, Ogre::Vector3::UNIT_Z, 0, -239, 0, _sceneMgr,       0, -240 + offset, 0, _simulator);
        
//        makePlaneWithPhysics(Ogre::Vector3::UNIT_Z, "_six_one", 426, 480, Ogre::Vector3::UNIT_Y, 0, 0, -479, _sceneMgr,          0, 0, -480 + offset, _simulator);
//        makePlaneWithPhysics(Ogre::Vector3::UNIT_Z, "_six_two", 480, 426, Ogre::Vector3::UNIT_Y, 0, 0, -480 - offset, _sceneMgr, 0, 0, -480 - offset, _simulator);
    
        //ONE
        makePlane(Ogre::Vector3::NEGATIVE_UNIT_Z, "_one_one", 426, 426, Ogre::Vector3::UNIT_Y, 0, 0, 480, _sceneMgr, "Example/Green");
        makePlane(Ogre::Vector3::NEGATIVE_UNIT_Z, "_one_two", 27, 426, Ogre::Vector3::UNIT_Y, 226.5, 0, 480, _sceneMgr, "Example/Wood");
        makePlane(Ogre::Vector3::NEGATIVE_UNIT_Z, "_one_three", 27, 426, Ogre::Vector3::UNIT_Y, -226.5, 0, 480, _sceneMgr, "Example/Wood");
        makePlane(Ogre::Vector3::NEGATIVE_UNIT_Z, "_one_four", 426, 27, Ogre::Vector3::UNIT_Y, 0, 226.5, 480, _sceneMgr, "Example/Wood");
        makePlane(Ogre::Vector3::NEGATIVE_UNIT_Z, "_one_five", 426, 27, Ogre::Vector3::UNIT_Y, 0, -226.5, 480, _sceneMgr, "Example/Wood");

        //TWO
        makePlane(Ogre::Vector3::UNIT_X, "_two_one", 906, 426, Ogre::Vector3::UNIT_Y, -239, 0, 0, _sceneMgr, "Example/Green");
        makePlane(Ogre::Vector3::UNIT_X, "_two_two", 439.5, 480, Ogre::Vector3::UNIT_Y, -240, 0, 233.25, _sceneMgr, "Example/Wood");
        makePlane(Ogre::Vector3::UNIT_X, "_two_three", 439.5, 480, Ogre::Vector3::UNIT_Y, -240, 0, -233.25, _sceneMgr, "Example/Wood");
        makePlane(Ogre::Vector3::UNIT_X, "_two_four", 27, 426, Ogre::Vector3::UNIT_Y, -240, 0, 466.5, _sceneMgr, "Example/Wood");
        makePlane(Ogre::Vector3::UNIT_X, "_two_five", 27, 426, Ogre::Vector3::UNIT_Y, -240, 0, -466.5, _sceneMgr, "Example/Wood");        

        //THREE
        makePlane(Ogre::Vector3::NEGATIVE_UNIT_Y, "_three_one", 426, 906, Ogre::Vector3::UNIT_Z, 0, 239, 0, _sceneMgr, "Example/Green");
        makePlane(Ogre::Vector3::NEGATIVE_UNIT_Y, "_three_two", 480, 439.5, Ogre::Vector3::UNIT_Z, 0, 240, 233.25, _sceneMgr, "Example/Wood");
        makePlane(Ogre::Vector3::NEGATIVE_UNIT_Y, "_three_three", 480, 439.5, Ogre::Vector3::UNIT_Z, 0, 240, -233.25, _sceneMgr, "Example/Wood");
        makePlane(Ogre::Vector3::NEGATIVE_UNIT_Y, "_three_four", 426, 27, Ogre::Vector3::UNIT_Z, 0, 240, 466.5, _sceneMgr, "Example/Wood");
        makePlane(Ogre::Vector3::NEGATIVE_UNIT_Y, "_three_five", 426, 27, Ogre::Vector3::UNIT_Z, 0, 240, -466.5, _sceneMgr, "Example/Wood");        

        //FOUR
        makePlane(Ogre::Vector3::NEGATIVE_UNIT_X, "_four_one", 906, 426, Ogre::Vector3::UNIT_Y, 239, 0, 0, _sceneMgr, "Example/Green");
        makePlane(Ogre::Vector3::NEGATIVE_UNIT_X, "_four_two", 439.5, 480, Ogre::Vector3::UNIT_Y, 240, 0, 233.25, _sceneMgr, "Example/Wood");
        makePlane(Ogre::Vector3::NEGATIVE_UNIT_X, "_four_three", 439.5, 480, Ogre::Vector3::UNIT_Y, 240, 0, -233.25, _sceneMgr, "Example/Wood");
        makePlane(Ogre::Vector3::NEGATIVE_UNIT_X, "_four_four", 27, 426, Ogre::Vector3::UNIT_Y, 240, 0, 466.5, _sceneMgr, "Example/Wood");
        makePlane(Ogre::Vector3::NEGATIVE_UNIT_X, "_four_five", 27, 426, Ogre::Vector3::UNIT_Y, 240, 0, -466.5, _sceneMgr, "Example/Wood");

        //FIVE       
        makePlane(Ogre::Vector3::UNIT_Y, "_five_one", 426, 906, Ogre::Vector3::UNIT_Z, 0, -239, 0, _sceneMgr, "Example/Green");
        makePlane(Ogre::Vector3::UNIT_Y, "_five_two", 480, 439.5, Ogre::Vector3::UNIT_Z, 0, -240, 233.25, _sceneMgr, "Example/Wood");
        makePlane(Ogre::Vector3::UNIT_Y, "_five_three", 480, 439.5, Ogre::Vector3::UNIT_Z, 0, -240, -233.25, _sceneMgr, "Example/Wood");
        makePlane(Ogre::Vector3::UNIT_Y, "_five_four", 426, 27, Ogre::Vector3::UNIT_Z, 0, -240, 466.5, _sceneMgr, "Example/Wood");
        makePlane(Ogre::Vector3::UNIT_Y, "_five_five", 426, 27, Ogre::Vector3::UNIT_Z, 0, -240, -466.5, _sceneMgr, "Example/Wood");

        //SIX
        makePlane(Ogre::Vector3::UNIT_Z, "_six_one", 426, 426, Ogre::Vector3::UNIT_Y, 0, 0, -480, _sceneMgr, "Example/Green");
        makePlane(Ogre::Vector3::UNIT_Z, "_six_two", 27, 426, Ogre::Vector3::UNIT_Y, 226.5, 0, -480, _sceneMgr, "Example/Wood");
        makePlane(Ogre::Vector3::UNIT_Z, "_six_three", 27, 426, Ogre::Vector3::UNIT_Y, -226.5, 0, -480, _sceneMgr, "Example/Wood");
        makePlane(Ogre::Vector3::UNIT_Z, "_six_four", 426, 27, Ogre::Vector3::UNIT_Y, 0, 226.5, -480, _sceneMgr, "Example/Wood");
        makePlane(Ogre::Vector3::UNIT_Z, "_six_five", 426, 27, Ogre::Vector3::UNIT_Y, 0, -226.5, -480, _sceneMgr, "Example/Wood"); 
    }
};

#endif