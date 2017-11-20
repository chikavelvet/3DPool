#include "GameObject.h"
#include "ComponentNotFoundException.h"
#include "GraphicsComponent.h"
#include "PhysicsComponent.h"
#include "Simulator.h"

GameObject::GameObject ()
{
    
}

PhysicsComponent* GameObject::getPhysics() {
    if (!physics)
        throw ComponentNotFoundException("Physics");
    return physics;
}

GraphicsComponent* GameObject::getGraphics() {
    if (!graphics)
        throw ComponentNotFoundException("Graphics");
    return graphics;
}

btRigidBody* GameObject::getBody() {
    return getPhysics()->body;
}

Ogre::SceneNode* GameObject::getNode() {
    return getGraphics()->rootNode;
}