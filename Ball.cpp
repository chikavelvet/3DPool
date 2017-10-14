#include "Ball.h"

Ball::Ball(Ogre::SceneManager* mSceneMgr, Simulator* physicsEngine, 
        btScalar x, btScalar y, btScalar z, 
        std::string name, 
        std::map<size_t,objType>& typeMap) :
    colShape(new btSphereShape(5)),
    mass(5),
    localInertia(btVector3(0, 0, 0))
{
    //----------------make a sphere-------------------//
    entity = mSceneMgr->createEntity("sphere.mesh"); 
    node = mSceneMgr->getRootSceneNode()->createChildSceneNode(name);
    node->attachObject(entity);
    node->setPosition(x, y, z);
    node->scale(0.05, 0.05, 0.05);

    typeMap[((size_t) colShape)] = ballType;

    physicsEngine->getCollisionShapes().push_back(colShape);
    btTransform startTransform;
    startTransform.setIdentity();
    startTransform.setRotation(btQuaternion(0.0f, 0.0f, 0.0f, 1));

    btVector3 initialPosition(x, y, z);
    startTransform.setOrigin(initialPosition);
    colShape->calculateLocalInertia(mass, localInertia);

    //actually contruct the body and add it to the dynamics world
    btDefaultMotionState *myMotionState = new btDefaultMotionState(startTransform); 
    btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, colShape, localInertia);
    body = new btRigidBody(rbInfo);
    body->setUserPointer(node);
    body->setFriction(btScalar(1.0));
    body->setRollingFriction(btScalar(1.0));

    body->setRestitution(0.8);
    body->setDamping(0.1, 0);

    physicsEngine->getDynamicsWorld()->addRigidBody(body);
    physicsEngine->trackRigidBodyWithName(body, name); 
}

Ogre::Vector3 Ball::getPosition() {
    btVector3 btPos = body->getCenterOfMassPosition();
    return Ogre::Vector3(float(btPos.x()), float(btPos.y()), float(btPos.z()));
}

