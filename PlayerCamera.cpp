
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


/*bool PlayerCamera::moveCameraToStick (Stick* const cueStick) {
    bool done = true;
    Ogre::Vector3 goalPos = cameraOffsetFromStick + cueStick->getPosition();
    Ogre::Quaternion goalRot = cueStick->getNode()->getOrientation();

    Ogre::Vector3 difPos = goalPos - cam->getPosition();
    Ogre::Vector3 difRot((goalRot.getRoll()-cam->getOrientation().getRoll()).valueDegrees(), (goalRot.getPitch()-cam->getOrientation().getPitch()).valueDegrees(), (goalRot.getYaw()-cam->getOrientation().getYaw()).valueDegrees());

	if(!difPos.isZeroLength()){
		done = false;
		float difX = difPos.x > 0.0000000f ? std::min(difPos.x, 0.001f): difPos.x < 0.0000000f ? std::max(difPos.x, -0.001f): 0.0f;
		float difY = difPos.x > 0.0000000f ? std::min(difPos.y, 0.001f): difPos.y < 0.0000000f ? std::max(difPos.y, -0.001f): 0.0f;
		float difZ = difPos.z > 0.0000000f ? std::min(difPos.z, 0.001f): difPos.z < 0.0000000f ? std::max(difPos.z, -0.001f): 0.0f;
		std::cout << "off pos : " << difX << " " << difY << " " << difZ << std::endl;
		cam->moveRelative(Ogre::Vector3(difX, difY, difZ));
	}
	// else if(!difRot.isZeroLength()){
	// 	std::cout << "off rot" << std::endl;
	// 	done = false;
	// 	cam->rotate(Ogre::Vector3(1, 0, 0), Ogre::Degree( difRot.x > 0 ? std::min(1.0f, difRot.x) : std::max(-1.0f, difRot.x) ));
	// 	cam->rotate(Ogre::Vector3(0, 1, 0), Ogre::Degree( difRot.y > 0 ? std::min(1.0f, difRot.y) : std::max(-1.0f, difRot.y) ));
	// 	cam->rotate(Ogre::Vector3(0, 0, 1), Ogre::Degree( difRot.z > 0 ? std::min(1.0f, difRot.z) : std::max(-1.0f, difRot.z) ));
	// }    
    return done;
}*/