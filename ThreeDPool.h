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
#include <btBulletDynamicsCommon.h>
#include <map>
#include "Enums.h"
#include "Player.h"

//--Forward Declarations--//
//----Game Objects--------//
class GameObject;
class PlayerCamera;
class Player;
class Stick;
class Room;
class Pocket;
class Ball;

//----Game Managers-------//
class Simulator; 
class GUIManager;
class NetManager;

//----SDL-mix-------------//
class Mix_Chunk;

//----CEGUI---------------//
namespace CEGUI {
    class EventArgs;
}

class ThreeDPool : public BaseApplication
{
public:
    ThreeDPool(void);
    virtual ~ThreeDPool(void);
    
    int solidBallsRemaining;
    int stripedBallsRemaining;
    float cueStickTotal;
    static const float CUE_STICK_MAX,
                       CUE_STICK_MIN,
                       STICK_POWER_MULT;

protected:
    friend class GUIManager;
    friend class AIPlayer;
    friend class Stick;
    
    static const int   BALL_SPEED_SUM_FREQUENCY;
    
    virtual void createScene(void);
    virtual void createMultiplayer(void);
    virtual void createCamera(void);
    virtual bool setup(void);
    virtual bool configure(void);
    virtual bool frameRenderingQueued(const Ogre::FrameEvent& evt);

    void restart(void);

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
    
    virtual void startWaiting(void);
    virtual void cancelWaiting(void);
    virtual void joinMultiplayer ();
    virtual void hostMultiplayer ();
    
    void incrementStrokeCount(void);
    void decrementRemainingBallCount(bool redBall);
    void increaseScore(void);
//    void updateOppStrokeCount(int newVal);
//    void updateOppRemainingBallCount(int newVal);
    
    void createFrameListener(void);
    
    void addPockets(void);
    void addBallPyramid(void);
    void playBGM(void);
    
    void playerWon(Player* winning);
    
    bool quit (const CEGUI::EventArgs& e);

    void hideAllScreens(void);

    void setUpLighting(void);

    void updateBallSpeedSum(void);

    void endCurrentTurn(void);
    
    bool ballsStopped();
    
    Player* getActivePlayer (void) { return player1Turn ? player1 : player2; }
    Player* getInactivePlayer (void) { return player1Turn ? player2 : player1; }
    
    bool activePlayerReadyToHitEightBall(void);
    
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
    Ogre::Vector3 cameraOffset;
    Ogre::Vector3 preFreeLookCameraPosition;
    Ogre::Vector3 preFreeLookCameraDirection;
    Ogre::Real mMoveSpeed;
    bool hitBall;
    bool LMBDown;
    float cueStickDelta;
    float cueStickTotalProgress;
    bool adjustingStick;
    bool cursorDisplaying;
    bool soundOn;
    bool player1Turn;
    bool gameStarted;
    bool gameEnded;
    bool gamePaused;
    bool isServer;
    
    bool letTurnEnd;
    
    bool firstBallHit;
    bool ballInThisTurn;
    
    bool scratchedInPocket;
    bool scratchedOnBall;
    
    bool eightBallIn;
    
    bool firstAssignment;
    bool redBallToAssign;
    
    bool ballsAssignedToPlayers;
    
    std::vector<Ball*> stripedBalls;
    std::vector<Ball*> solidBalls;
    std::vector<Pocket*> pockets;
    
    bool isMultiplayer;
    bool isWaiting;
    std::string hostName;
    int port;
    
    bool isAI;
    int AIDifficulty;
    
    int strokes;
    int opponentStrokes;
    int score;
    
    Player* player1;
    Player* player2;
    
    Stick* cueStick;
    Ball* cueBall;
    Ball* eightBall;
    Room* room;

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
