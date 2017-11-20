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
    
    PhysicsComponent* physics;
    GraphicsComponent* graphics;
public:    
    PhysicsComponent* getPhysics() const;
    GraphicsComponent* getGraphics() const;
    
    btRigidBody* getBody() const;
    Ogre::SceneNode* getNode() const;
};

#endif