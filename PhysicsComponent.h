/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   PhysicsComponent.h
 * Author: treyr3
 *
 * Created on October 24, 2017, 6:10 PM
 */

#ifndef PHYSICSCOMPONENT_H
#define PHYSICSCOMPONENT_H

#include "Component.h"

#include <btBulletDynamicsCommon.h>
#include "Simulator.h"

class Simulator;

class PhysicsComponent : public Component {
public:
    PhysicsComponent (GameObject* owner,
            Simulator* _simulator,
            btScalar _mass, btVector3 _inertia,
            btScalar _restitution, btScalar _friction,
            btScalar _linearDamping, btScalar _angularDamping,
            bool _kinematic, bool _needsUpdates,
            collisionType _coltype, collisionType _collidesWith,
            btVector3 _origin, btQuaternion _rotation,
            btCollisionShape* _shape,
            void* _userPointer);
    
    void addToSimulator (void);
    
//    btRigidBody* getBody (void) { return body; }
//    Simulator* getSimulator (void) { return simulator; }
    
    void removeObject (void);

//protected:
    Simulator* simulator;
    btCollisionShape* shape;
    btMotionState* motionState;
    btRigidBody* body;
    void* userPointer;
    
    btTransform tr;
    btVector3 inertia;
    btScalar mass;
    btScalar restitution;
    btScalar friction;
    btScalar linearDamping;
    btScalar angularDamping;
    
    bool kinematic;
    bool needsUpdates;
    
    collisionType coltype;
    collisionType collidesWith;
};

#endif /* PHYSICSCOMPONENT_H */

