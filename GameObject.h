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
#include "Enums.h"

class CollisionContext;
class BulletContactCallback;
class Simulator;
class ComponentNotFoundException;
class GraphicsComponent;
class PhysicsComponent;

class GameObject {
protected:        
    GameObject ();
    
    GameObject (const Ogre::String& _name, Ogre::SceneManager* _sceneMgr,
            Simulator* _simulator, btScalar _mass, btVector3 _inertia, 
            btScalar _restitution, btScalar _friction, 
            btScalar _linearDamping, btScalar _angularDamping,
            bool _kinematic, bool _needsUpdates,
            collisionType _coltype, collisionType _collidesWith);
    
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

    collisionType collidesWith;
    collisionType coltype;
    
    PhysicsComponent* physics;
    GraphicsComponent* graphics;
public:
    void addToSimulator();
    void updateTransform();
    
    virtual PhysicsComponent* getPhysics();
    virtual GraphicsComponent* getGraphics();
    
    virtual btRigidBody* getBody();
    virtual Ogre::SceneNode* getNode();
    
    virtual void removeObject(void);
};

#endif