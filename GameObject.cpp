#include "GameObject.h"
#include "ComponentNotFoundException.h"
#include "GraphicsComponent.h"
#include "PhysicsComponent.h"
#include "Simulator.h"

GameObject::GameObject ()
{
    
}

PhysicsComponent* GameObject::getPhysics() const {
    if (!physics)
        throw ComponentNotFoundException("Physics");
    return physics;
}

GraphicsComponent* GameObject::getGraphics() const {
    if (!graphics)
        throw ComponentNotFoundException("Graphics");
    return graphics;
}

btRigidBody* GameObject::getBody() const {
    return getPhysics()->body;
}

Ogre::SceneNode* GameObject::getNode() const {
    return getGraphics()->rootNode;
}