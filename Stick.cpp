#include "Stick.h"

Stick::Stick(Ogre::SceneManager* _sceneMgr,
             Simulator* _simulator,
             btScalar x, btScalar y, btScalar z,
             Ogre::String _name,
             float _cueStickMax, float _cueStickMin, float _powerMultiplier,
             btRigidBody* _cueBall, std::map<size_t, objType> &typeMap) :
        cueStickMax(_cueStickMax),
        cueStickMin(_cueStickMin),
        powerMultiplier(_powerMultiplier),
        cueBall(_cueBall)
{
    simulator = _simulator;
    name = _name;
    sceneMgr = _sceneMgr;
    
    kinematic = false;
    needsUpdates = false;
    mass = 10;
    inertia = btVector3(0, 0, 0);
    restitution = 1.0;
    friction = 1.0;
    linearDamping = 0.1;
    angularDamping = 1;
    
    geom = sceneMgr->createEntity("cube.mesh");
    rootNode = sceneMgr->getRootSceneNode()->createChildSceneNode(name);
    rootNode->attachObject(geom);
    rootNode->setPosition(x, y, z);
    rootNode->scale(0.01, 0.01, 0.5);
    
    // Create the new shape, and tell the physics that is a sphere
    shape = new btBoxShape(btVector3(1, 1, 23));
    
    typeMap[((size_t)shape)] = stickType;

    // TODO: make sure this isn't causing a double addition to the simulator
    simulator->getCollisionShapes().push_back(shape);
    //----------------------------------------------------------------------
    
    tr.setIdentity();
    tr.setRotation(btQuaternion(0.0f, 0.0f, 0.0f, 1));
    tr.setOrigin(btVector3(x, y, z));
    
    shape->calculateLocalInertia(mass, inertia);
    
    // motionState = new OgreMotionState(tr, rootNode);
    motionState = new btDefaultMotionState(tr);
    
    addToSimulator();
}

bool Stick::readjustStickToCueball (bool& adjustingStick) {
    bool cueStickStopped = (fabs(body->getLinearVelocity().length()) < 0.05f)
                        && (fabs(body->getTotalForce().length()) < 0.05f);
    
    bool cueBallStopped = (fabs(cueBall->getLinearVelocity().length()) < 0.01f)
                       && (fabs(cueBall->getTotalForce().length()) < 0.01f);
    
    if (cueStickStopped) {
        simulator->getDynamicsWorld()->removeRigidBody(body);
        geom->setVisible(false);
    }
    
    bool turnIsOver = cueBallStopped && cueStickStopped;
    
    if (!turnIsOver)
        return false;
    
    btVector3 ballPos = cueBall->getCenterOfMassPosition();
    btTransform newTransform(btQuaternion(0, 0, 0, 1), 
            btVector3(ballPos.getX(), ballPos.getY(), ballPos.getZ() + cueStickMin));
    body->setCenterOfMassTransform(newTransform);
    adjustingStick = false;
    simulator->getDynamicsWorld()->addRigidBody(body);
    geom->setVisible(true);
    return true;
}

void Stick::chargeStick (bool adjustingStick, float& cueStickTotal, 
                         float& cueStickDelta, bool LMBDown) {
    if (adjustingStick)
        return;
   
    body->activate(true);
    
    if (cueStickTotal <= cueStickMax && cueStickTotal >= cueStickMin) {
        if (cueStickTotal + cueStickDelta > cueStickMax) 
            cueStickDelta = cueStickMax - cueStickTotal;
        
        if (cueStickTotal + cueStickDelta < cueStickMin) {
            std::cout << "Below Minimum" << std::endl;
            cueStickTotal = cueStickMin;
            cueStickDelta = 0.0f;
            return;
        }
        
        if (LMBDown) {
            btVector3 movement = btVector3(body->getCenterOfMassPosition() 
                - cueBall->getCenterOfMassPosition()).normalize() * cueStickDelta;
            body->translate(movement);
        }
        
        cueStickTotal += cueStickDelta;        
        cueStickDelta = 0;
    }
}