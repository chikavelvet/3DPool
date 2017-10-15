#include "Stick.h"

Stick::Stick(Ogre::SceneManager* _sceneMgr,
             Simulator* _simulator,
             btScalar x, btScalar y, btScalar z,
             Ogre::String _name,
             float _cueStickMax, float _cueStickMin, float _powerMultiplier,
             btRigidBody* _cueBall, std::map<size_t, objType> &typeMap) :
    GameObject(_name, _sceneMgr, _simulator,
            10, btVector3(0, 0, 0),
            1.0, 1.0, 
            0.1, 1.0,
            false, false,
            COL_STICK, COL_CUEBALL),
        cueStickMax(_cueStickMax),
        cueStickMin(_cueStickMin),
        powerMultiplier(_powerMultiplier),
        cueBall(_cueBall)
{
    geom = sceneMgr->createEntity("cube.mesh");
    geom->setMaterialName("Example/Stick");
    
    rootNode = sceneMgr->getRootSceneNode()->createChildSceneNode(name);
    rootNode->attachObject(geom);
    rootNode->setPosition(x, y, z);
    rootNode->scale(0.01, 0.01, 0.5);

    shape = new btBoxShape(btVector3(1, 1, 23));
    
    typeMap[((size_t) rootNode)] = stickType;

    tr.setIdentity();
    tr.setRotation(btQuaternion(0.0f, 0.0f, 0.0f, 1));
    tr.setOrigin(btVector3(x, y, z));
        
    // motionState = new OgreMotionState(tr, rootNode);
    motionState = new btDefaultMotionState(tr);
    
    addToSimulator();
}

bool Stick::readjustStickToCueball (bool& adjustingStick) {
    bool cueStickStopped = (fabs(body->getLinearVelocity().length()) < 0.05f)
                        && (fabs(body->getTotalForce().length()) < 0.05f);
    
    bool cueBallStopped = (fabs(cueBall->getLinearVelocity().length()) < 0.03f)
                       && (fabs(cueBall->getTotalForce().length()) < 0.03f);
    
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
    
    std::cout << "Node: " << rootNode->getPosition() << std::endl;
    std::cout << "Body: " << body->getCenterOfMassPosition();
    
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

void Stick::releaseStick (bool& adjustingStick, bool& hitBall, float& cueStickTotal, float& cueStickDelta) {
    if(cueStickTotal >= cueStickMin){
        body->activate(true);
        btVector3 movement = btVector3(body->getCenterOfMassPosition()-cueBall->getCenterOfMassPosition()).normalize() * -powerMultiplier * cueStickTotal * fabs(cueStickTotal);    
        body->applyCentralImpulse(movement);
    }
    cueStickTotal = cueStickMin;
    cueStickDelta = 0;
    adjustingStick = true;
    hitBall = false;
}

void Stick::rotateToMouseInput (float& deltaRotationX, float& deltaRotationY) {
    rotateToMouseXInput(deltaRotationX);
    rotateToMouseYInput(deltaRotationY);
}

void Stick::rotateToMouseXInput (float& deltaRotationX) {
    btVector3 difference = cueBall->getCenterOfMassPosition() - body->getCenterOfMassPosition();
    body->translate(difference);

    btVector3 yAxis(0.0, 1.0, 0.0);
    btQuaternion q = body->getCenterOfMassTransform().getRotation();
    btVector3 bodyAxis = btMatrix3x3(q) * yAxis;

    
     //make X rotation
    btQuaternion rotation(bodyAxis,btRadians(deltaRotationX));
    rotation *= body->getOrientation();

    //actually apply the rotations
    body->setCenterOfMassTransform(btTransform(rotation, body->getCenterOfMassPosition()));

    btVector3 invDifference = difference;
    invDifference = invDifference.rotate(bodyAxis,btRadians(deltaRotationX));
    body->translate(-invDifference);

    //reset delta rotations
    deltaRotationX = 0.0f;
}

void Stick::rotateToMouseYInput (float& deltaRotationY) {
    btVector3 difference = cueBall->getCenterOfMassPosition() - body->getCenterOfMassPosition();
    body->translate(difference);

    btVector3 xAxis(1.0, 0.0, 0.0);
    btQuaternion q = body->getCenterOfMassTransform().getRotation();
    btVector3 bodyAxis = btMatrix3x3(q) * xAxis;

    
    //make Y rotation
    btQuaternion rotation(bodyAxis,btRadians(deltaRotationY));
    rotation *= body->getOrientation();

    //actually apply the rotations
    body->setCenterOfMassTransform(btTransform(rotation, body->getCenterOfMassPosition()));

    btVector3 invDifference = difference;
    invDifference = invDifference.rotate(bodyAxis, btRadians(deltaRotationY));
    body->translate(-invDifference);

    //reset delta rotations
    deltaRotationY = 0.0f;
}

Ogre::Vector3 Stick::getPosition() {
    return rootNode->getPosition();
}
