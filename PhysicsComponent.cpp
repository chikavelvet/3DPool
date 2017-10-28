/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   PhysicsComponent.cpp
 * Author: treyr3
 * 
 * Created on October 24, 2017, 6:10 PM
 */

#include "PhysicsComponent.h"

PhysicsComponent::PhysicsComponent(GameObject* owner, 
        Simulator* _simulator, 
        btScalar _mass, btVector3 _inertia, 
        btScalar _restitution, btScalar _friction, 
        btScalar _linearDamping, btScalar _angularDamping, 
        bool _kinematic, bool _needsUpdates, 
        collisionType _coltype, int _collidesWith, 
        btVector3 _origin, btQuaternion _rotation, btCollisionShape* _shape,
        void* _userPointer) :
Component(owner),
        simulator(_simulator),
        shape(_shape),
        inertia(_inertia),
        mass(_mass),
        restitution(_restitution),
        friction(_friction),
        linearDamping(_linearDamping),
        angularDamping(_angularDamping),
        kinematic(_kinematic),
        needsUpdates(_needsUpdates),
        coltype(_coltype),
        collidesWith(_collidesWith),
        userPointer(_userPointer)
{
    tr.setIdentity();
    tr.setRotation(_rotation);
    tr.setOrigin(_origin);
    
    motionState = new btDefaultMotionState(tr);
}

void PhysicsComponent::addToSimulator() {		
    //rigidbody is dynamic if and only if mass is non zero, otherwise static
    if (mass != 0.0f) 
            shape->calculateLocalInertia(mass, inertia);

    btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, motionState, shape, inertia);
    rbInfo.m_restitution = restitution;
    rbInfo.m_friction = friction;
    body = new btRigidBody(rbInfo);
    body->setUserPointer(userPointer);
    body->setDamping(linearDamping, angularDamping);

    if (kinematic) {
            body->setCollisionFlags(body->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
            body->setActivationState(DISABLE_DEACTIVATION);
    }

    simulator->addObject(owner, coltype, collidesWith, body);
}

void PhysicsComponent::removeObject() {
    simulator->getDynamicsWorld()->removeRigidBody(body);
}