
#include "PlayerCamera.h"

Ogre::Camera* PlayerCamera::getCamera() {
    return this->cam;
}

PlayerCamera::PlayerCamera(Ogre::Camera* const _cam) :
cam(_cam)
{
}