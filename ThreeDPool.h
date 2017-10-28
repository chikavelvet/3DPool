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
#include "NetManager.h"
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

    void setUpGUI(void);
    void setUpSounds(void);

    void gameLoop(const Ogre::FrameEvent& evt);
    void physicsLoop(void);
    void networkLoop (void);
    void makeGround(void);
    void cameraFollowStick(void);
    
    bool keyPressed(const OIS::KeyEvent &arg);
    bool keyReleased(const OIS::KeyEvent &arg);
    bool mouseMoved(const OIS::MouseEvent &me);
    bool mouseReleased(const OIS::MouseEvent &me, OIS::MouseButtonID id);
    bool mousePressed(const OIS::MouseEvent &me, OIS::MouseButtonID id);
    
    void displayQuitCursor(void);
    void hideQuitCursor(void);
    
    void incrementStrokeCount(void);
    void decrementRemainingBallCount(void);
    void updateOppStrokeCount(int newVal);
    void updateOppRemainingBallCount(int newVal);
    
    void createFrameListener(void);
    
    void addPockets(void);
    void addBallPyramid(void);
    void playBGM(void);
    
    bool quit (const CEGUI::EventArgs& e);

    CEGUI::OgreRenderer* mRenderer;
        
    PlayerCamera* pCamera;
    NetManager* nm;
    Simulator* physicsEngine;
    bool adjustingCamera;
    int cameraCounter;
    Ogre::Vector3 newLookAt;
    Ogre::Vector3 newCamPos;
    Ogre::Real mMoveSpeed;
    bool hitBall;
    bool LMBDown;
    float cueStickDelta;
    float cueStickTotal;
    bool adjustingStick;
    bool cursorDisplaying;
    bool soundOn;
    bool yourTurn;
    
    int strokes;
    int opponentStrokes;

    Mix_Chunk* ball_ball;
    Mix_Chunk* stick_ball;
    Mix_Chunk* pocket;
    Mix_Chunk* bgMusic;

    std::map<size_t, objType> typeMap;
    std::map<Ogre::SceneNode*, Ball*> pocketMap;
};

//---------------------------------------------------------------------------

#endif // #ifndef __TutorialApplication_h_

//---------------------------------------------------------------------------
