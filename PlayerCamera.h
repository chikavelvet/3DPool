#ifndef __PlayerCamera_h_
#define __PlayerCamera_h_

#include "GameObject.h"

class PlayerCamera : GameObject {
protected:
    Ogre::Camera* cam;
    
public:
    Ogre::Camera* getCamera ();
    
    PlayerCamera (Ogre::Camera* const cam);
};

#endif // #ifndef __PlayerCamera_h_