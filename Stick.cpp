#include "Stick.h"
#include "Ball.h"
#include "GraphicsComponent.h"
#include "PhysicsComponent.h"

Stick::Stick(Ogre::SceneManager* _sceneMgr,
             Simulator* _simulator,
             btScalar x, btScalar y, btScalar z,
             Ogre::String _name,
             float _cueStickMax, float _cueStickMin, float _powerMultiplier,
             Ball* _cueBall, std::map<size_t, objType> &typeMap) :
        cueStickMax(_cueStickMax),
        cueStickMin(_cueStickMin),
        powerMultiplier(_powerMultiplier),
        cueBall(_cueBall)
{
    graphics = new GraphicsComponent(this, _sceneMgr, Ogre::String(_name),
            Ogre::Vector3(x, y, z),
            Ogre::Vector3(STICK_DEFAULT::GRAPHICS::SCALE_FACTOR_XY, 
                          STICK_DEFAULT::GRAPHICS::SCALE_FACTOR_XY,
                          STICK_DEFAULT::GRAPHICS::SCALE_FACTOR_Z),
            STICK_DEFAULT::GRAPHICS::MESH, STICK_DEFAULT::GRAPHICS::MATERIAL);
    
    Ogre::SceneNode* rootNode = graphics->rootNode;
    
    physics = new PhysicsComponent(this, _simulator,
            STICK_DEFAULT::PHYSICS::MASS, STICK_DEFAULT::PHYSICS::INERTIA,
            STICK_DEFAULT::PHYSICS::RESTITUTION, STICK_DEFAULT::PHYSICS::FRICTION,
            STICK_DEFAULT::PHYSICS::LINEAR_DAMPING, STICK_DEFAULT::PHYSICS::ANGULAR_DAMPING,
            STICK_DEFAULT::PHYSICS::KINEMATIC, STICK_DEFAULT::PHYSICS::NEEDS_UPDATES,
            STICK_DEFAULT::PHYSICS::COLTYPE, STICK_DEFAULT::PHYSICS::COLLIDES_WITH,
            btVector3(x, y, z), STICK_DEFAULT::PHYSICS::ROTATION,
            new btBoxShape(STICK_DEFAULT::PHYSICS::DIMENSIONS),
            rootNode);
    
    typeMap[((size_t) rootNode)] = stickType;
    
    physics->addToSimulator();

    guideLineParticle = graphics->sceneMgr->createParticleSystem("GuideLine", "Examples/GuideLine");
    guideLineNode = rootNode->createChildSceneNode("Particle");
    guideLineNode->attachObject(guideLineParticle);
}

bool Stick::readjustStickToCueball (bool& adjustingStick, bool ballsStopped, bool& letTurnEnd) {
    try {
        GraphicsComponent* graph = getGraphics();
        PhysicsComponent* phys = getPhysics();
        
        btRigidBody* body    = phys->body;
        Simulator* simulator = phys->simulator;
        Ogre::Entity* geom   = graph->geom;
        
        btRigidBody* cueBallBody = cueBall->getBody();
        
        bool cueStickStopped = body->getLinearVelocity().length() < 0.05f
                            && body->getTotalForce().length() < 0.05f;

        bool cueBallStopped = cueBallBody->getLinearVelocity().length() < 1.f
                           && cueBallBody->getTotalForce().length() < 1.f;

        if (cueStickStopped) {
            simulator->getDynamicsWorld()->removeRigidBody(body);
            geom->setVisible(false);
        }

        bool turnIsOver = cueBallStopped && ballsStopped && cueStickStopped && letTurnEnd;

        if (!turnIsOver)
            return false;

        cueBallBody->setLinearVelocity(btVector3(0, 0, 0));
        cueBallBody->setAngularVelocity(btVector3(0, 0, 0));
        cueBallBody->clearForces();
        cueBallBody->setLinearVelocity(btVector3(0, 0, 0));
        cueBallBody->setAngularVelocity(btVector3(0, 0, 0));

        btVector3 ballPos = cueBallBody->getCenterOfMassPosition();
        btTransform newTransform(btQuaternion(0, 0, 0, 1), 
                btVector3(ballPos.getX(), ballPos.getY(), ballPos.getZ() + cueStickMin));
        body->setCenterOfMassTransform(newTransform);
        adjustingStick = false;

        simulator->getDynamicsWorld()->addRigidBody(body);
        geom->setVisible(true);

        Ogre::Vector3 stickDirection(graph->rootNode->getPosition() 
                    - cueBall->getNode()->getPosition());
        stickDirection.normalise();

        // guideLineNode->setPosition(-stickDirection * (cueStickMin*2.f));
        guideLineNode->setPosition(stickDirection * (cueStickMin + 5.f));
        guideLineNode->setVisible(true);
        return true;
    } catch (ComponentNotFoundException& e) {
        
    }
}

void Stick::chargeStick (bool adjustingStick, float& cueStickTotal, 
                         float& cueStickDelta, bool LMBDown) {
    if (adjustingStick)
        return;
    
    try {
        GraphicsComponent* graph = getGraphics();
        PhysicsComponent* phys = getPhysics();
        
        btRigidBody* body    = phys->body;
        Ogre::SceneNode* rootNode   = graph->rootNode;
        
        body->activate(true);

        guideLineNode->setOrientation(Ogre::Quaternion(-1.f, 1.f, 0.f, 0.f));

        Ogre::Vector3 stickDirection(rootNode->getPosition() 
                    - cueBall->getNode()->getPosition());
        stickDirection.normalise();

        guideLineNode->setPosition(0.f, 0.f, -(cueStickTotal + (cueStickMin  * 3.f)));
        guideLineNode->setVisible(true);


        if (cueStickTotal <= cueStickMax && cueStickTotal >= cueStickMin) {
            if (cueStickTotal + cueStickDelta > cueStickMax) 
                cueStickDelta = cueStickMax - cueStickTotal;

            if (cueStickTotal + cueStickDelta < cueStickMin) {
                std::cout << "Below Minimum Wage" << std::endl;
                cueStickTotal = cueStickMin;
                cueStickDelta = 0.0f;
                return;
            }

            if (LMBDown) {
                btVector3 movement = btVector3(body->getCenterOfMassPosition() 
                    - cueBall->getBody()->getCenterOfMassPosition()).normalize() * cueStickDelta;
                body->translate(movement);
            }

            cueStickTotal += cueStickDelta;        
            cueStickDelta = 0;
        }
    } catch (ComponentNotFoundException& e) {
        
    }
}

void Stick::releaseStick (bool& adjustingStick, bool& hitBall, float& cueStickTotal, float& cueStickDelta) {
    if(cueStickTotal > cueStickMin){
        try {
            PhysicsComponent* phys = getPhysics();

            btRigidBody* body    = phys->body;
            
            body->activate(true);

            btVector3 zAxis(0.0, 0.0, 1.0);
            btQuaternion q = body->getCenterOfMassTransform().getRotation();
            btVector3 direction = btMatrix3x3(q) * zAxis;

            btVector3 movement = -direction * powerMultiplier * cueStickTotal * std::abs(cueStickTotal);    

            body->applyCentralForce(movement);
            adjustingStick = true;
            guideLineNode->setVisible(false);
        } catch (ComponentNotFoundException& e) {
        
        }
    }
    cueStickTotal = cueStickMin;
    cueStickDelta = 0;
    hitBall = false;
}

void Stick::rotateToMouseInput (float& deltaRotationX, float& deltaRotationY) {
    rotateToMouseXInput(deltaRotationX);
    rotateToMouseYInput(deltaRotationY);
}

void Stick::rotateToMouseXInput (float& deltaRotationX) {
    try {
        PhysicsComponent* phys = getPhysics();
        
        btRigidBody* body = phys->body;
        
        btVector3 difference = cueBall->getBody()->getCenterOfMassPosition() - body->getCenterOfMassPosition();
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
    } catch (ComponentNotFoundException& e) {
        
    }
}

void Stick::rotateToMouseYInput (float& deltaRotationY) {
    try {
        PhysicsComponent* phys = getPhysics();
        
        btRigidBody* body    = phys->body;
        
        btVector3 difference = cueBall->getBody()->getCenterOfMassPosition() 
                - body->getCenterOfMassPosition();
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
    } catch (ComponentNotFoundException& e) {
        
    }
}

Ogre::Vector3 Stick::getPosition() const {
    try {
        GraphicsComponent* graph  = getGraphics();
        
        Ogre::SceneNode* rootNode = graph->rootNode;
        
        return rootNode->getPosition();
    } catch (ComponentNotFoundException& e) {
        
    }
}