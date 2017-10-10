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
#include "Simulator.h"
#include "BulletContactCallback.h"
#include "OgreMotionState.h"

class GameObject {
protected:
	Ogre::String name;
	Ogre::SceneManager* sceneMgr;
	Simulator* simulator;
	Ogre::SceneNode* rootNode;
	Ogre::Entity* geom;
	btCollisionShape* shape;
	btScalar mass;
	OgreMotionState* motionState;
	btRigidBody* body;
	btTransform tr;
	btVector3 inertia;

	btScalar restitution;
	btScalar friction;
	btScalar linearDamping;
	bool kinematic;
	bool needsUpdates;

	CollisionContext* context;
	BulletContactCallback* cCallBack;

public:
	void addToSimulator();
	void updateTransform();

	btRigidBody* getBody(){ return body; }
};

#endif