#ifndef __Simulator_h_
#define __Simulator_h_

#include <btBulletDynamicsCommon.h>
#include <vector>
#include <map>
#include <string>
#include "BulletContactCallback.h"
#include "OgreMotionState.h"

class GameObject;

class Simulator { 
protected: 
	btDefaultCollisionConfiguration* collisionConfiguration; 
	btCollisionDispatcher* dispatcher; 
	btBroadphaseInterface* overlappingPairCache; 
	btSequentialImpulseConstraintSolver* solver;
	btDiscreteDynamicsWorld* dynamicsWorld;
	btAlignedObjectArray<btCollisionShape*> collisionShapes;
	std::deque<GameObject*> objList; 

public: 
	Simulator(){}; 
	~Simulator(){}; 

	void initObjects();
	void addObject(GameObject* o); 
	bool removeObject(GameObject* o); 
	btDiscreteDynamicsWorld* getDynamicsWorld() { return dynamicsWorld; }
	void stepSimulation(const Ogre::Real elapsedTime, 
		int maxSubSteps = 1, const Ogre::Real fixedTimestep = 1.0f/60.0f); 
	btAlignedObjectArray<btCollisionShape*> & getCollisionShapes(){
		return collisionShapes;
	}
	void trackRigidBodyWithName(btRigidBody* body, std::string name){}

};

#endif