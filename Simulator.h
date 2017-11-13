#ifndef __Simulator_h_
#define __Simulator_h_

#include <btBulletDynamicsCommon.h>
#include <vector>
#include <map>
#include <string>
#include "BulletContactCallback.h"
#include "OgreMotionState.h"
//#include "GameObject.h"
#include "Enums.h"

class GameObject;

class Simulator { 
protected: 
	btDefaultCollisionConfiguration* collisionConfiguration; 
	btCollisionDispatcher* dispatcher; 
	btBroadphaseInterface* overlappingPairCache; 
	btSequentialImpulseConstraintSolver* solver;
	btDiscreteDynamicsWorld* dynamicsWorld;
	btAlignedObjectArray<btCollisionShape*> collisionShapes;
	// std::deque<GameObject*> objList; 
	std::vector<GameObject*> objList; 

public: 
	Simulator(){}; 
	~Simulator(){}; 

	void initObjects();

	GameObject* getObject(int index){
		return objList.at(index);
	}

	void addObject(GameObject* o, collisionType coltype, int collidesWith); 
        void addObject(GameObject* o, collisionType coltype, int collidesWith, btRigidBody* body);
        
	bool removeObject(GameObject* o); 
	btDiscreteDynamicsWorld* getDynamicsWorld() { return dynamicsWorld; }
	void stepSimulation(const Ogre::Real elapsedTime, 
		int maxSubSteps = 1, const Ogre::Real fixedTimestep = 1.0f/60.0f); 
	btAlignedObjectArray<btCollisionShape*> & getCollisionShapes(){
		return collisionShapes;
	}
};

#endif