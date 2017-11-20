
#include "PlayerCamera.h"

const Ogre::Vector3 PlayerCamera::cameraOffsetFromStick(0, 10, 30);

Ogre::Camera* PlayerCamera::getCamera() const {
    return this->cam;
}

PlayerCamera::PlayerCamera(Ogre::Camera* const _cam) :
cam(_cam)
{
}

bool PlayerCamera::moveCameraToStick (Stick* const cueStick) {
    Ogre::Vector3 stickPos = cueStick->getPosition();
    Ogre::Quaternion stickDir = cueStick->getNode()->getOrientation();
    
    cam->setPosition(stickPos);
    cam->setOrientation(stickDir);
    cam->moveRelative(cameraOffsetFromStick);

    return true;
}