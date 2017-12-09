#ifndef __PlayerCamera_h_
#define __PlayerCamera_h_

#include "GameObject.h"
#include "Stick.h"

class PlayerCamera : GameObject {
protected:
    static const Ogre::Vector3 cameraOffsetFromStick;
    
    Ogre::Camera* cam;
    
public:
    PlayerCamera (Ogre::Camera* const cam);
    
    Ogre::Camera* getCamera () const;
    
    bool moveCameraToStick (Stick* const cueStick);
};

#endif // #ifndef __PlayerCamera_h_