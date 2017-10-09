/*
-----------------------------------------------------------------------------
Filename:    ThreeDPool.h
-----------------------------------------------------------------------------

This source file is part of the
   ___                 __    __ _ _    _
  /___\__ _ _ __ ___  / / /\ \ (_) | _(_)
 //  // _` | '__/ _ \ \ \/  \/ / | |/ / |
/ \_// (_| | | |  __/  \  /\  /| |   <| |
\___/ \__, |_|  \___|   \/  \/ |_|_|\_\_|
      |___/
Tutorial Framework (for Ogre 1.9)
http://www.ogre3d.org/wiki/
-----------------------------------------------------------------------------
*/

#ifndef __ThreeDPool_h_
#define __ThreeDPool_h_

#include "BaseApplication.h"

//---------------------------------------------------------------------------

class ThreeDPool : public BaseApplication
{
public:
    ThreeDPool(void);
    virtual ~ThreeDPool(void);

protected:
    virtual void createScene(void);
    virtual void createCamera(void);
    virtual bool frameRenderingQueued(const Ogre::FrameEvent& evt);
    
    void gameLoop(void);
    void physicsLoop(void);
    void makeGround(void);
    void cameraFollowStick(void);

    Simulator* physicsEngine;
    bool adjustingCamera;
    int cameraCounter = 0;
    Ogre::Vector3 newLookAt;
    Ogre::Vector3 newCamPos;
};

//---------------------------------------------------------------------------

#endif // #ifndef __TutorialApplication_h_

//---------------------------------------------------------------------------
