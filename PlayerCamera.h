#ifndef __PlayerCamera_h_
#define __PlayerCamera_h_

#include "GameObject.h"
#include "Stick.h"

class PlayerCamera : GameObject {
protected:
    Ogre::Camera* cam;
    
    int cameraOffsetFromStick = 100;
    
public:
    PlayerCamera (Ogre::Camera* const cam);
    
    Ogre::Camera* getCamera ();
    
    bool moveCameraToStick (Stick* const cueStick);
};

#endif // #ifndef __PlayerCamera_h_