#ifndef __GameObject_h_
#define __GameObject_h_

#include <btBulletDynamicsCommon.h>
#include <OgreCamera.h>
#include <OgreEntity.h>
#include <OgreLogManager.h>
#include <OgreRoot.h>
#include <OgreViewport.h>
#include <OgreSceneManager.h>
#include <OgreRenderWindow.h>
#include <OgreConfigFile.h>
// #include "BaseApplication.h"
#include "Simulator.h"
#include "Enums.h"
#include "ComponentNotFoundException.h"

class CollisionContext;
class BulletContactCallback;
class PhysicsComponent;
class GraphicsComponent;
class Simulator;

//const int cueBallType = 0,
//          ballType    = 1,
//          roomType    = 2, 
//          pocketType  = 4, 
//          stickType   = 5;

class GameObject {
protected:        
    GameObject ();
    
    GameObject (const Ogre::String& _name, Ogre::SceneManager* _sceneMgr,
            Simulator* _simulator, btScalar _mass, btVector3 _inertia, 
            btScalar _restitution, btScalar _friction, 
            btScalar _linearDamping, btScalar _angularDamping,
            bool _kinematic, bool _needsUpdates,
            collisionType _coltype, int _collidesWith);
    
    Ogre::String name;
    Ogre::SceneManager* sceneMgr;
    Ogre::SceneNode* rootNode;
    Ogre::Entity* geom;
    
    Simulator* simulator;
    btCollisionShape* shape;
    btScalar mass;
    // OgreMotionState* motionState;
    btMotionState* motionState;
    btRigidBody* body;
    btTransform tr;
    btVector3 inertia;

    btScalar restitution;
    btScalar friction;
    btScalar linearDamping;
    btScalar angularDamping;
    bool kinematic;
    bool needsUpdates;

    CollisionContext* context;
    BulletContactCallback* cCallBack;

    int collidesWith;
    collisionType coltype;
    
    PhysicsComponent* physics;
    GraphicsComponent* graphics;
public:
    void addToSimulator();
    void updateTransform();
    
    virtual PhysicsComponent* getPhysics();
    virtual GraphicsComponent* getGraphics();
    virtual btRigidBody* getBody(){ return body; }
    virtual Ogre::SceneNode* getNode() { return rootNode; }
    virtual void removeObject(void);
};

#endif