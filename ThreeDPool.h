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
#include <OgreParticleIterator.h>
#include <OgreParticleSystem.h>
#include <OgreParticleSystemManager.h>
#include <OgreParticle.h>

class GameObject;
#include "Simulator.h"
class Simulator; 
#include "Ball.h"
#include "Stick.h"
#include "Room.h"
#include "PlayerCamera.h"
#include "Pocket.h"

#include "Player.h"

class PlayerCamera;
class GUIManager;
//---------------------------------------------------------------------------

class ThreeDPool : public BaseApplication
{
public:
    ThreeDPool(void);
    virtual ~ThreeDPool(void);
    
    int redBallsRemaining;
    int blueBallsRemaining;

protected:
    friend class GUIManager;
    virtual void createScene(void);
    virtual void createMultiplayer(void);
    virtual void createMainMenu();
    virtual void createMPLobby(void);
    virtual void createCamera(void);
    virtual bool setup(void);
    virtual bool configure(void);
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
    
    virtual void showEnterIPWindow(void);
    virtual void hideEnterIPWindow(void);
    virtual void startWaiting(void);
    virtual void cancelWaiting(void);
    virtual void onIPEnterBoxKeyPressed (const CEGUI::EventArgs& e);
    virtual void joinMultiplayer ();
    virtual void hostMultiplayer ();
    
    void incrementStrokeCount(void);
    void decrementRemainingBallCount(bool redBall);
    void increaseScore(void);
    void updateOppStrokeCount(int newVal);
    void updateOppRemainingBallCount(int newVal);
    
    void createFrameListener(void);
    
    void addPockets(void);
    void addBallPyramid(void);
    void playBGM(void);
    
    bool quit (const CEGUI::EventArgs& e);

    void hideAllScreens(void);

    void setUpLighting(void);

    void updateBallSpeedSum(void);

    void endCurrentTurn(void);
    
    Player* getActivePlayer (void) { return player1Turn ? player1 : player2; }
    Player* getInactivePlayer (void) { return player1Turn ? player2 : player1; }

    bool mainMenuScreenCreated;
    bool mpLobbyScreenCreated;
    bool gameScreenCreated;
    
    GUIManager* mGUIMgr;
        
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
    bool player1Turn;
    bool gameStarted;
    bool gameEnded;
    
    bool ballsAssignedToPlayers;
    
    bool isMultiplayer;
    bool isWaiting;
    std::string hostName;
    int port;
    
    bool isAI;
    
    int strokes;
    int opponentStrokes;
    int score;
    
    Player* player1;
    Player* player2;

    Mix_Chunk* ball_ball;
    Mix_Chunk* stick_ball;
    Mix_Chunk* pocket;
    Mix_Chunk* bgMusic;

    std::map<size_t, objType> typeMap;
    std::map<Ogre::SceneNode*, Ball*> pocketMap;

    btVector3 ballSpeedSum;
    int frameCounter;
};

//---------------------------------------------------------------------------

#endif // #ifndef __TutorialApplication_h_

//---------------------------------------------------------------------------
