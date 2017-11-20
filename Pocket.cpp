/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
#include "Pocket.h"
#include "GraphicsComponent.h"
#include "PhysicsComponent.h"

Pocket::Pocket(Ogre::SceneManager* _sceneMgr, 
        Simulator* _simulator, 
        btScalar x, btScalar y, btScalar z, 
        std::string _name, 
        std::map<size_t,objType>& typeMap)
{   
    graphics = new GraphicsComponent(this, _sceneMgr, _name,
            Ogre::Vector3(x, y, z), 
            Ogre::Vector3(
                POCKET_DEFAULT::GRAPHICS::SCALE_FACTOR_XYZ,
                POCKET_DEFAULT::GRAPHICS::SCALE_FACTOR_XYZ,
                POCKET_DEFAULT::GRAPHICS::SCALE_FACTOR_XYZ),
            POCKET_DEFAULT::GRAPHICS::MESH,
            POCKET_DEFAULT::GRAPHICS::MATERIAL);

    Ogre::SceneNode* rootNode = graphics->rootNode;
    rootNode->setVisible(false);
    
    physics = new PhysicsComponent(this, _simulator,
            POCKET_DEFAULT::PHYSICS::MASS, 
            POCKET_DEFAULT::PHYSICS::INERTIA,
            POCKET_DEFAULT::PHYSICS::RESTITUTION, 
            POCKET_DEFAULT::PHYSICS::FRICTION,
            POCKET_DEFAULT::PHYSICS::LINEAR_DAMPING, 
            POCKET_DEFAULT::PHYSICS::ANGULAR_DAMPING,
            POCKET_DEFAULT::PHYSICS::KINEMATIC, 
            POCKET_DEFAULT::PHYSICS::NEEDS_UPDATES,
            POCKET_DEFAULT::PHYSICS::COLTYPE, 
            POCKET_DEFAULT::PHYSICS::COLLIDES_WITH,
            btVector3(x, y, z), POCKET_DEFAULT::PHYSICS::ROTATION,
            new btSphereShape(27), 
            rootNode);
       
    typeMap[((size_t) rootNode)] = pocketType;
    physics->addToSimulator();
}