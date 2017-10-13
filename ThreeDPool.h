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
#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>
#include <map>

class PlayerCamera;
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

    void gameLoop(const Ogre::FrameEvent& evt);
    void physicsLoop(void);
    void makeGround(void);
    void cameraFollowStick(void);
    
    bool keyPressed(const OIS::KeyEvent &arg);
    bool keyReleased(const OIS::KeyEvent &arg);
    bool mouseMoved(const OIS::MouseEvent &me);
    bool mouseReleased(const OIS::MouseEvent &me, OIS::MouseButtonID id);
    bool mousePressed(const OIS::MouseEvent &me, OIS::MouseButtonID id);
    
    void displayQuitCursor(void);
    void hideQuitCursor(void);
    
    void createFrameListener(void);
    
    bool quit (const CEGUI::EventArgs& e);

    CEGUI::OgreRenderer* mRenderer;
        
    PlayerCamera* pCamera;
    Simulator* physicsEngine;
    bool adjustingCamera;
    int cameraCounter = 0;
    Ogre::Vector3 newLookAt;
    Ogre::Vector3 newCamPos;
    Ogre::Real mMoveSpeed;
    bool hitBall;
    bool LMBDown;
    float cueStickDelta;
    float cueStickTotal;
    bool adjustingStick;
    bool cursorDisplaying;

    Mix_Chunk* ball_ball;
    Mix_Chunk* stick_ball;
    Mix_Chunk* pocket;

    std::map<btCollisionShape*, objType> typeMap;
};

//---------------------------------------------------------------------------

#endif // #ifndef __TutorialApplication_h_

//---------------------------------------------------------------------------
