/*
-----------------------------------------------------------------------------
Filename:    TutorialApplication.cpp
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

#include <OGRE/OgreMeshManager.h>
#include <fstream>

#include "ThreeDPool.h"
#include "ManualPlayer.h"
#include "AIPlayer.h"
#include "NetworkPlayer.h"
#include "GUIManager.h"
#include "Simulator.h"
#include "Ball.h"
#include "Stick.h"
#include "Room.h"
#include "PlayerCamera.h"
#include "Pocket.h"
#include "Player.h"
#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>
#include "NetManager.h"

#include <OgreParticleIterator.h>
#include <OgreParticleSystem.h>
#include <OgreParticleSystemManager.h>
#include <OgreParticle.h>

#include <CEGUI/CEGUI.h>
#include <CEGUI/RendererModules/Ogre/Renderer.h>

const float ThreeDPool::CUE_STICK_MAX            = 150.0,
            ThreeDPool::CUE_STICK_MIN            = 50.0, 
            ThreeDPool::STICK_POWER_MULT         = 10.0;
const int   ThreeDPool::BALL_SPEED_SUM_FREQUENCY = 100;

std::vector<Ball*> redBalls;
std::vector<Ball*> blueBalls;

int remainingBalls;
int oppRemainingBalls;

//---------------------------------------------------------------------------
ThreeDPool::ThreeDPool(void) :
        mMoveSpeed(750),
        hitBall(false),
        LMBDown(false),
        cueStickDelta(0),
        cueStickTotal(CUE_STICK_MIN),
        adjustingStick(false),
        adjustingCamera(false),
        cursorDisplaying(false),
        soundOn(true),
        strokes(0),
        opponentStrokes(0),
        cameraCounter(0),
        typeMap(),
        pocketMap(),
        gameStarted(false),
        isMultiplayer(false),
        isAI(false),
        mainMenuScreenCreated(false),
        mpLobbyScreenCreated(false),
        gameScreenCreated(false),
        hostName(""),
        port(59000),
        isWaiting(false),
        ballSpeedSum(0, 0, 0),
        frameCounter(0),
        gameEnded(false),
        player1(NULL),
        player2(NULL),
        player1Turn(true),
        ballsAssignedToPlayers(false)
{
}
//---------------------------------------------------------------------------
ThreeDPool::~ThreeDPool(void)
{
    delete pCamera;
}

bool ThreeDPool::setup(void)
{
    if (!BaseApplication::setup())
        return false;

    mGUIMgr = new GUIManager(this);
    mGUIMgr->createMainMenu();

    return true;
};

void ThreeDPool::hideAllScreens() 
{
    CEGUI::WindowManager& wmgr = CEGUI::WindowManager::getSingleton();
    CEGUI::GUIContext& context = CEGUI::System::getSingleton().getDefaultGUIContext();
    CEGUI::Window* sheet = context.getRootWindow();
    
    for (size_t index = 0; index < sheet->getChildCount(); ++index) {
        CEGUI::Window* screen = sheet->getChildAtIdx(index);
        
        screen->hide();
    }    

    CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().show();
}

void ThreeDPool::startWaiting() {
    if (!isWaiting || !isServer) {
        CEGUI::WindowManager& wmgr = CEGUI::WindowManager::getSingleton();
        CEGUI::GUIContext& context = CEGUI::System::getSingleton().getDefaultGUIContext();
        CEGUI::Window* mpLobby = context.getRootWindow()->getChild("MPLobbyScreen");

        isServer = true;
        
        nm = new NetManager();
        std::cout << "INFO" << std::endl;
        std::cout << isServer << std::endl;
        std::cout << hostName << std::endl;
        std::cout << port << std::endl;
        std::cout << "endINFO" << std::endl;

        nm->initNetManager();
        nm->addNetworkInfo(PROTOCOL_ALL, NULL, port);
        bool started = nm->startServer();
        nm->acceptConnections();

        std::cout << std::boolalpha << started << std::endl;
        std::cout << nm->getIPstring() << std::endl;

        CEGUI::Window* waiting = mpLobby->getChild("WaitingWindow");

        mpLobby->getChild("EnterIPWindow")->hide();
        waiting->getChild("ServerInfo")->setText("Server IP: " + nm->getIPstring());
        waiting->setPosition(CEGUI::UVector2(CEGUI::UDim(0.2, 0), CEGUI::UDim(0.3, 0)));
        waiting->show();

        isWaiting = true;
    }
}

void ThreeDPool::cancelWaiting() {
    if (isWaiting && isServer) {
        CEGUI::WindowManager& wmgr = CEGUI::WindowManager::getSingleton();
        CEGUI::GUIContext& context = CEGUI::System::getSingleton().getDefaultGUIContext();
        CEGUI::Window* mpLobby = context.getRootWindow()->getChild("MPLobbyScreen");
        CEGUI::Window* waiting = mpLobby->getChild("WaitingWindow");

        nm->stopServer(PROTOCOL_TCP);
        nm->stopServer(PROTOCOL_UDP);
        delete nm;

        waiting->hide();

        isWaiting = false;
        isServer = false;
    }
}

void ThreeDPool::joinMultiplayer () 
{
    CEGUI::WindowManager& wmgr = CEGUI::WindowManager::getSingleton();
    CEGUI::GUIContext& context = CEGUI::System::getSingleton().getDefaultGUIContext();
    CEGUI::Editbox* ipEnterBox = static_cast<CEGUI::Editbox*>
            (context.getRootWindow()->getChild("MPLobbyScreen/EnterIPWindow/IPEnterBox"));
    
    hostName = std::string(ipEnterBox->getText().c_str());
    
    isServer = false;
    
    nm = new NetManager();
    std::cout << "INFO" << std::endl;
    std::cout << isServer << std::endl;
    std::cout << hostName << std::endl;
    std::cout << port << std::endl;
    std::cout << "endINFO" << std::endl;

    nm->initNetManager();
    nm->addNetworkInfo(PROTOCOL_ALL, hostName.c_str(), port);
    nm->startClient();
        
    isWaiting = true;
}

void ThreeDPool::hostMultiplayer () 
{
    createMultiplayer();
}

void ThreeDPool::createMultiplayer(void)
{
    isMultiplayer = true;
    createScene();
}

void ThreeDPool::endCurrentTurn(void){
    player1Turn = !player1Turn;
    CEGUI::GUIContext& context = CEGUI::System::getSingleton().getDefaultGUIContext();
    CEGUI::Window* sheet = context.getRootWindow();
    CEGUI::Window* activePlayer = sheet->getChild("GameScreen")->getChild("ActivePlayer"); 
        
    activePlayer->setText(player1Turn ? "Player 1's Turn" : "Player 2's Turn");
    
    if (ballsAssignedToPlayers) {
        CEGUI::Window* targettingColorWin = sheet->getChild("GameScreen/TargettingColor");
        std::string targetting;
        
        if (player1Turn) {
            targetting = player1->targetRedBall ? "Targetting: Red" : "Targetting: Blue";
        } else {
            targetting = player2->targetRedBall ? "Targetting: Red" : "Targetting: Blue";
        }
        
        targettingColorWin->setText(targetting);
    }

    player1->endCurrentTurn();
    player2->endCurrentTurn();
}

//---------------------------------------------------------------------------
void ThreeDPool::createScene(void)
{
    gameStarted = true;

    //-------------basic setup stuff-----------------//
    setUpLighting();

    physicsEngine = new Simulator();
    physicsEngine->initObjects();
    
    // Set up Players //
    player1 = new ManualPlayer();
    player2 = new ManualPlayer();
    
    if (isMultiplayer)
        if (isAI)
            player2 = new AIPlayer();
        else
            player2 = new NetworkPlayer();

    cueBall = new Ball(mSceneMgr, physicsEngine, 0, 0, 240, "cueBall", typeMap, pocketMap, "Example/White", false, true);

    cueStick = new Stick(mSceneMgr, physicsEngine, 0, 0, 240 + CUE_STICK_MIN, "cueStick", CUE_STICK_MAX, CUE_STICK_MIN, STICK_POWER_MULT, cueBall, typeMap);
    
    cameraOffset = Ogre::Vector3(mCamera->getPosition()-cueStick->getPosition());
    btVector3 btPos = cueStick->getBody()->getCenterOfMassPosition();
    Ogre::Vector3 cueStickPos(float(btPos.x()),float(btPos.y()), float(btPos.z()));
    mCamera->lookAt(cueStickPos);
    mCamera->setPosition(cueStickPos + cameraOffset);

    room = new Room(mSceneMgr, physicsEngine);
    
    addPockets();
    addBallPyramid();
    redBallsRemaining = redBalls.size();
    blueBallsRemaining = blueBalls.size();
    oppRemainingBalls = remainingBalls;
    mGUIMgr->setUpGUI();
    setUpSounds();
}

void ThreeDPool::setUpLighting(void){
    mSceneMgr->setAmbientLight(Ogre::ColourValue(0.5, 0.5, 0.5));
    // mSceneMgr->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_ADDITIVE);
    Ogre::Light* pointLight1 = mSceneMgr->createLight("PointLight1");
    pointLight1->setType(Ogre::Light::LT_POINT);
    pointLight1->setDiffuseColour(.4, .4, .4);
    pointLight1->setSpecularColour(.4, .4, .4);
    pointLight1->setPosition(0, 220, 460);
    Ogre::Light* pointLight2 = mSceneMgr->createLight("PointLIght2");
    pointLight2->setType(Ogre::Light::LT_POINT);
    pointLight2->setDiffuseColour(.4, .4, .4);
    pointLight2->setSpecularColour(.4, .4, .4);
    pointLight2->setPosition(0, -220, 460);
    Ogre::Light* pointLight3 = mSceneMgr->createLight("PointLIght3");
    pointLight3->setType(Ogre::Light::LT_POINT);
    pointLight3->setDiffuseColour(.4, .4, .4);
    pointLight3->setSpecularColour(.4, .4, .4);
    pointLight3->setPosition(0, 220, -460);
    Ogre::Light* pointLight4 = mSceneMgr->createLight("PointLIght4");
    pointLight4->setType(Ogre::Light::LT_POINT);
    pointLight4->setDiffuseColour(.4, .4, .4);
    pointLight4->setSpecularColour(.4, .4, .4);
    pointLight4->setPosition(0, -220, -460);    
}

void ThreeDPool::setUpSounds(void){
    SDL_Init(SDL_INIT_VIDEO);
    int audio_rate = 22050;
    Uint16 audio_format = AUDIO_S16SYS;
    int audio_channels = 2;
    int audio_buffers = 4096; 
    
    Mix_OpenAudio(audio_rate, audio_format, audio_channels, audio_buffers);

    Mix_AllocateChannels(64);
    ball_ball = Mix_LoadWAV("cueball_hit_other.wav");
    stick_ball = Mix_LoadWAV("cue_strike_ball.wav");
    pocket = Mix_LoadWAV("pool_ball_into_pocket.wav");
    bgMusic = Mix_LoadWAV("Elevator-music.wav");
    playBGM();
}


void ThreeDPool::playBGM() {
    Mix_PlayChannel(-1, bgMusic, -1);
}


void ThreeDPool::addBallPyramid() {
    blueBalls.push_back(new Ball(mSceneMgr, physicsEngine, 0, 0, -225, "b1", typeMap, pocketMap, "Example/Blue", false));

    redBalls.push_back(new Ball(mSceneMgr, physicsEngine, -5, 5, -235, "b2", typeMap, pocketMap, "Example/Red", true));
    blueBalls.push_back(new Ball(mSceneMgr, physicsEngine, 5, -5, -235, "b3", typeMap, pocketMap, "Example/Blue", false));
    
    blueBalls.push_back(new Ball(mSceneMgr, physicsEngine, 0, 0, -245, "b4", typeMap, pocketMap, "Example/Blue", false));
    redBalls.push_back(new Ball(mSceneMgr, physicsEngine, 10, -10, -245, "b5", typeMap, pocketMap, "Example/Red", true));
    redBalls.push_back(new Ball(mSceneMgr, physicsEngine, -10, 10, -245, "b6", typeMap, pocketMap, "Example/Red", true));
    
    blueBalls.push_back(new Ball(mSceneMgr, physicsEngine, 5, -5, -255, "b7", typeMap, pocketMap, "Example/Blue", false));
    redBalls.push_back(new Ball(mSceneMgr, physicsEngine, -5, 5, -255, "b8", typeMap, pocketMap, "Example/Red", true));
    redBalls.push_back(new Ball(mSceneMgr, physicsEngine, 15, -15, -255, "b9", typeMap, pocketMap, "Example/Red", true));
    blueBalls.push_back(new Ball(mSceneMgr, physicsEngine, -15, 15, -255, "b10", typeMap, pocketMap, "Example/Blue", false));

    // Easy-in Ball
    //balls.push_back(new Ball(mSceneMgr, physicsEngine, 200, -200, 20, "bTest", typeMap, pocketMap, "Example/GreenOther"));
}

void ThreeDPool::addPockets() {
    // 12 Pockets
    Pocket* p1 = new Pocket(mSceneMgr, physicsEngine, 
            -240, -240, 480,
            "p1", typeMap);
    Pocket* p2 = new Pocket(mSceneMgr, physicsEngine, 
            -240, -240, 0,
            "p2", typeMap);
    Pocket* p3 = new Pocket(mSceneMgr, physicsEngine, 
            -240, -240, -480,
            "p3", typeMap);
    Pocket* p4 = new Pocket(mSceneMgr, physicsEngine, 
            -240, 240, 480,
            "p4", typeMap);
    Pocket* p5 = new Pocket(mSceneMgr, physicsEngine, 
            -240, 240, 0,
            "p5", typeMap);
    Pocket* p6 = new Pocket(mSceneMgr, physicsEngine, 
            -240, 240, -480,
            "p6", typeMap);
    Pocket* p7 = new Pocket(mSceneMgr, physicsEngine, 
            240, -240, 480,
            "p7", typeMap);
    Pocket* p8 = new Pocket(mSceneMgr, physicsEngine, 
            240, -240, 0,
            "p8", typeMap);
    Pocket* p9 = new Pocket(mSceneMgr, physicsEngine, 
            240, -240, -480,
            "p9", typeMap);
    Pocket* p10 = new Pocket(mSceneMgr, physicsEngine, 
            240, 240, 480,
            "p10", typeMap);
    Pocket* p11 = new Pocket(mSceneMgr, physicsEngine, 
            240, 240, 0,
            "p11", typeMap);
    Pocket* p12 = new Pocket(mSceneMgr, physicsEngine, 
            240, 240, -480,
            "p12", typeMap);
    
}

void ThreeDPool::incrementStrokeCount() {    
    CEGUI::GUIContext& context = CEGUI::System::getSingleton().getDefaultGUIContext();
    CEGUI::Window* sheet = context.getRootWindow();
    std::stringstream ss;
    
    if (player1Turn) {
        CEGUI::Window* strokesWin = sheet->getChild("GameScreen")->getChild("StrokeCount");
        ++strokes;
        ss << "Strokes: " << strokes;
        strokesWin->setText(ss.str());
    } else {
        CEGUI::Window* oppStrokesWin = sheet->getChild("GameScreen")->getChild("OppStrokeCount");
        ++opponentStrokes;
        ss << "Opp Strokes: " << opponentStrokes;
        oppStrokesWin->setText(ss.str());
    }
    
    if (isMultiplayer) {
        if (isServer) {
            std::stringstream ss2;
            ss2 << "strokes " << strokes;
            std::string msg = ss2.str();
            nm->messageClients(PROTOCOL_TCP, msg.c_str(), msg.length());
        }
        else {
            std::stringstream ss2;
            ss2 << "strokes " << strokes;
            std::string msg = ss2.str();
            nm->messageServer(PROTOCOL_TCP, msg.c_str(), msg.length());
        }
    }
}

void ThreeDPool::decrementRemainingBallCount(bool redBall) {
    CEGUI::GUIContext& context = CEGUI::System::getSingleton().getDefaultGUIContext();
    CEGUI::Window* sheet = context.getRootWindow();
    std::stringstream ss;
    
    if (soundOn)
        Mix_PlayChannel(-1, pocket, 0);
    
    
//    if (player1Turn) {
//        CEGUI::Window* remainingBallWin = sheet->getChild("GameScreen")->getChild("RemainingBalls");
//        --remainingBalls;
//        ss << "Remaining: " << remainingBalls;
//        remainingBallWin->setText(ss.str());
//    } else {
//        CEGUI::Window* oppRemainingBallWin = sheet->getChild("GameScreen")->getChild("OppRemainingBalls");
//        --oppRemainingBalls;
//        ss << "Opp Remaining: " << oppRemainingBalls;
//        oppRemainingBallWin->setText(ss.str());
//    }
    
    if (redBall) {
        CEGUI::Window* redBallWin = sheet->getChild("GameScreen")->getChild("RedBallsRemaining");
        --redBallsRemaining;
        ss << "Red: " << redBallsRemaining;
        redBallWin->setText(ss.str());
        if (redBallsRemaining < 1 && blueBallsRemaining > 0) {
            CEGUI::Window* youWin = sheet->getChild("GameScreen")->getChild("YouWin");
            youWin->setText("Red Player Wins!");
            gameEnded = true;
            youWin->show();
        }
    } else {
        CEGUI::Window* blueBallWin = sheet->getChild("GameScreen")->getChild("BlueBallsRemaining");
        --blueBallsRemaining;
        ss << "Blue: " << blueBallsRemaining;
        blueBallWin->setText(ss.str());
        if (blueBallsRemaining < 1 && redBallsRemaining > 0) {
            CEGUI::Window* youWin = sheet->getChild("GameScreen")->getChild("YouWin");
            youWin->setText("Blue Player Wins!");
            gameEnded = true;
            youWin->show();
        }
    }
    
//    if (isMultiplayer) {
//        if (isServer) {
//            std::stringstream ss2;
//            ss2 << "remaining " << remainingBalls;
//            std::string msg = ss2.str();
//            nm->messageClients(PROTOCOL_TCP, msg.c_str(), msg.length());
//        }
//        else {
//            std::stringstream ss2;
//            ss2 << "remaining " << remainingBalls;
//            std::string msg = ss2.str();
//            nm->messageServer(PROTOCOL_TCP, msg.c_str(), msg.length());
//        }
//    }
}

void ThreeDPool::updateOppStrokeCount(int newVal) {    
    CEGUI::GUIContext& context = CEGUI::System::getSingleton().getDefaultGUIContext();
    CEGUI::Window* sheet = context.getRootWindow();
    CEGUI::Window* strokesWin = sheet->getChild("GameScreen")->getChild("OppStrokeCount");
    std::stringstream ss;
    
    opponentStrokes = newVal;
    ss << "Opp Strokes: " << opponentStrokes;
    strokesWin->setText(ss.str());
}

void ThreeDPool::updateOppRemainingBallCount(int newVal) {
    CEGUI::GUIContext& context = CEGUI::System::getSingleton().getDefaultGUIContext();
    CEGUI::Window* sheet = context.getRootWindow();
    CEGUI::Window* remainingBallWin = sheet->getChild("GameScreen")->getChild("OppRemainingBalls");
    std::stringstream ss;
      
    oppRemainingBalls = newVal;
    ss << "Opp Remaining: " << oppRemainingBalls;
    remainingBallWin->setText(ss.str());
    
    if (oppRemainingBalls < 1 && remainingBalls > 0) {
        CEGUI::Window* youWin = sheet->getChild("GameScreen")->getChild("YouWin");
        youWin->setText("You Lose!! :c");
        gameEnded = true;
        youWin->show();
    }
}

void ThreeDPool::displayQuitCursor () {
    CEGUI::GUIContext& context = CEGUI::System::getSingleton().getDefaultGUIContext();
    CEGUI::Window* sheet = context.getRootWindow();
    CEGUI::Window* quit = sheet->getChild("GameScreen")->getChild("QuitButton");
    
    context.getMouseCursor().show();
    quit->show();
}

void ThreeDPool::hideQuitCursor () {
    CEGUI::GUIContext& context = CEGUI::System::getSingleton().getDefaultGUIContext();
    CEGUI::Window* sheet = context.getRootWindow();
    CEGUI::Window* quit = sheet->getChild("GameScreen")->getChild("QuitButton");
    
    context.getMouseCursor().hide();
    quit->hide();
}

void ThreeDPool::createFrameListener() {
    Ogre::LogManager::getSingletonPtr()->logMessage("*** Initializing OIS ***");
    OIS::ParamList pl;
    size_t windowHnd = 0;
    std::ostringstream windowHndStr;
 
    mWindow->getCustomAttribute("WINDOW", &windowHnd);
    windowHndStr << windowHnd;
    pl.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));
 
    mInputManager = OIS::InputManager::createInputSystem( pl );
 
    mKeyboard = static_cast<OIS::Keyboard*>(mInputManager->createInputObject( OIS::OISKeyboard, true ));
    mMouse = static_cast<OIS::Mouse*>(mInputManager->createInputObject( OIS::OISMouse, true ));
 
    mMouse->setEventCallback(this);
    mKeyboard->setEventCallback(this);
 
    //Set initial mouse clipping size
    windowResized(mWindow);
 
    //Register as a Window listener
    Ogre::WindowEventUtilities::addWindowEventListener(mWindow, this);
 
    mRoot->addFrameListener(this);
}

bool ThreeDPool::keyPressed(const OIS::KeyEvent& arg) {
    // CEGUI Injection
    CEGUI::GUIContext& context = CEGUI::System::getSingleton().getDefaultGUIContext();
    context.injectKeyDown((CEGUI::Key::Scan)arg.key);
    context.injectChar((CEGUI::Key::Scan)arg.text);

    if (player1)
        player1->keyPressed(arg);
    if (player2)
        player2->keyPressed(arg);

    return true;
}

bool ThreeDPool::keyReleased(const OIS::KeyEvent &arg) {
    // CEGUI Injection
    CEGUI::System::getSingleton().getDefaultGUIContext().injectKeyUp((CEGUI::Key::Scan)arg.key);
    
    if (!BaseApplication::keyReleased(arg))
        return false;
    
    if (player1)
        player1->keyReleased(arg);
    if (player2)
        player2->keyReleased(arg);

    switch(arg.key) {
        case OIS::KC_T :
            if (!adjustingCamera){
                preFreeLookCameraPosition = mCamera->getPosition();
                preFreeLookCameraDirection = mCamera->getDirection();
            } else {
                mCamera->setPosition(preFreeLookCameraPosition);
                mCamera->setDirection(preFreeLookCameraDirection);
            }
            adjustingCamera = !adjustingCamera;
            break;
        case OIS::KC_ESCAPE :
            if (gameStarted) {
                if (!cursorDisplaying)
                    displayQuitCursor();
                else 
                    hideQuitCursor();
                cursorDisplaying = !cursorDisplaying;
            } else {
                mShutDown = true;
            }
            break;
        case OIS::KC_Y:
            if (!soundOn) {
                playBGM();
            } else {
                Mix_HaltChannel(-1);
            }
            soundOn = !soundOn;
            break;
        case OIS::KC_N:
            if (!isServer) {
                std::string msg = "I am a client";
                nm->messageServer(PROTOCOL_TCP, msg.c_str(), msg.length());
                std::cout << "Sent " << msg << std::endl;
            }
            break;
        case OIS::KC_M:
            if (isServer) {
                std::string msg = "I am a server";
                nm->messageClients(PROTOCOL_TCP, msg.c_str(), msg.length());
                std::cout << "Sent " << msg << std::endl;
            }
    }
    return true;
}

bool ThreeDPool::mouseMoved(const OIS::MouseEvent &me) {
    // CEGUI Injection
    CEGUI::GUIContext& context = CEGUI::System::getSingleton().getDefaultGUIContext();
    context.injectMouseMove(me.state.X.rel, me.state.Y.rel);

    // Scroll wheel.
    if (me.state.Z.rel)
        context.injectMouseWheelChange(me.state.Z.rel / 120.0f);

    if (!BaseApplication::mouseMoved(me))
        return false;

    if (player1)
        player1->mouseMoved(me);
    if (player2)
        player2->mouseMoved(me);
    
    if (adjustingCamera) {
       mCamera->yaw(Ogre::Degree(-0.13 * me.state.X.rel));
       mCamera->pitch(Ogre::Degree(-0.13 * me.state.Y.rel));
    }
}

CEGUI::MouseButton convertButton(OIS::MouseButtonID buttonID)
{
    switch (buttonID)
    {
    case OIS::MB_Left:
        return CEGUI::LeftButton;
 
    case OIS::MB_Right:
        return CEGUI::RightButton;
 
    case OIS::MB_Middle:
        return CEGUI::MiddleButton;
 
    default:
        return CEGUI::LeftButton;
    }
}

bool ThreeDPool::mouseReleased(const OIS::MouseEvent &me, OIS::MouseButtonID id)
{
    // CEGUI Injection
    CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseButtonUp(convertButton(id));
    
    if (player1)
        player1->mouseReleased(me, id);
    if (player2)
        player2->mouseReleased(me, id);

    // if(!BaseApplication::mouseReleased(me, id))
    //     return false;

    // if(std::abs(cueStickTotal) > 0.1)
    //     if(id==OIS::MB_Left)
    //         hitBall = true;

    return true;
}

bool ThreeDPool::mousePressed(const OIS::MouseEvent &me, OIS::MouseButtonID id)
{
    // CEGUI Injection
    CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseButtonDown(convertButton(id));
    
    if(!BaseApplication::mousePressed(me, id))
        return false;

    if (player1)
        player1->mousePressed(me, id);
    if (player2)
        player2->mousePressed(me, id);

    using namespace std;
    switch(id)
    {
        case OIS::MB_Left:
            LMBDown = true;
            break;
        case OIS::MB_Right:
            break;
        case OIS::MB_Middle:
            break;
        default:
            break;
    }
    return true;
}

bool ThreeDPool::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
    if(mWindow->isClosed())
        return false;

    if(mShutDown)
        return false;

    if(isWaiting) {
        if (isServer) {
            if (nm->scanForActivity()) {
                if (nm->getClients()){
                    ClientData* data = nm->tcpClientData.front();
                    
                    if (std::string(data->output) == "Client Request") {
                        std::string msg = "Server Response";
                        nm->messageClients(PROTOCOL_TCP, msg.c_str(), msg.length());
                        isWaiting = false;
                        createMultiplayer();
                    }
                }
            }
        } else {
            if (nm->scanForActivity()) {
                ClientData& data = nm->tcpServerData;
                if (std::string(data.output) == "Server Response") {
                    isWaiting = false;
                    createMultiplayer();
                }
            } else {
                std::string msg = "Client Request";
                nm->messageServer(PROTOCOL_TCP, msg.c_str(), msg.length());
            }
        }
           
    }

    // Need to capture/update each device
    mKeyboard->capture();
    mMouse->capture();

    //Need to inject timestamps to CEGUI System.
    CEGUI::System::getSingleton().injectTimePulse(evt.timeSinceLastFrame);

    if (gameStarted) {
        if(gameEnded)
           return true;    
        
        player1->frameUpdate(evt);

        if (isMultiplayer) {
            player2->frameUpdate(evt);
            networkLoop();
        }
        gameLoop(evt);
        physicsLoop();
    }


    return true;
}

void ThreeDPool::networkLoop () {
    if (isServer) {
        if (nm->scanForActivity()) {
            //ClientData& data = nm->tcpServerData;
            if (nm->getClients()){
                ClientData* data = nm->tcpClientData.front();

                std::cout << "Rcvd " << std::string(data->output) << std::endl;
                std::stringstream ss(data->output);
                std::string key;
                int val;
                ss >> key >> val;
                if (key == "remaining") {
                    this->updateOppRemainingBallCount(val);
                } else if (key == "strokes") {
                    this->updateOppStrokeCount(val);
                }
                else if (key == "quit") {
                    nm->stopServer();

                    CEGUI::WindowManager &wmgr = CEGUI::WindowManager::getSingleton();
                    CEGUI::GUIContext& context = CEGUI::System::getSingleton().getDefaultGUIContext();
                    CEGUI::Window* sheet = context.getRootWindow();
                    CEGUI::Window* youWin = sheet->getChild("GameScreen/YouWin");

                    youWin->setSize(CEGUI::USize(CEGUI::UDim(0.18, 0), CEGUI::UDim(0.1, 0)));
                    youWin->setPosition(CEGUI::UVector2(CEGUI::UDim(0.41, 0), CEGUI::UDim(0.4, 0)));  

                    youWin->setText("Opponent Disconnected:\nYou Win!");
                    gameEnded = true;
                    youWin->show();
                }
            }
        }
    } else {
        if (nm->scanForActivity()) {
            ClientData& data = nm->tcpServerData;
            std::cout << "Rcvd " << std::string(data.output) << std::endl;
            std::stringstream ss(data.output);
            std::string key;
            int val;
            ss >> key >> val;
            if (key == "remaining") {
                this->updateOppRemainingBallCount(val);
            } else if (key == "strokes") {
                this->updateOppStrokeCount(val);
            }
            else if (key == "quit") {
                nm->stopClient();

                CEGUI::WindowManager &wmgr = CEGUI::WindowManager::getSingleton();
                CEGUI::GUIContext& context = CEGUI::System::getSingleton().getDefaultGUIContext();
                CEGUI::Window* sheet = context.getRootWindow();
                CEGUI::Window* youWin = sheet->getChild("GameScreen/YouWin");

                    youWin->setSize(CEGUI::USize(CEGUI::UDim(0.18, 0), CEGUI::UDim(0.1, 0)));
                    youWin->setPosition(CEGUI::UVector2(CEGUI::UDim(0.41, 0), CEGUI::UDim(0.4, 0)));  

                youWin->setText("Opponent Disconnected:\nYou Win!");
                gameEnded = true;
                youWin->show();
            }
        }
    }
}

static bool needToUpdateCamera = false;

void ThreeDPool::gameLoop(const Ogre::FrameEvent& evt)
{
    if (player1Turn) {
        player1->giveGamePlayerInput(cueStickDelta, cueStickRotationX, cueStickRotationY, hitBall);
    } else {
        player2->giveGamePlayerInput(cueStickDelta, cueStickRotationX, cueStickRotationY, hitBall);
    }

    if(adjustingStick) {
        if(std::abs(cueBall->getBody()->getLinearVelocity().length())>0.01f){
            cueStick->getBody()->setLinearVelocity(btVector3(0, 0, 0));
        }
        
        // bool ballsStopped = ballSpeedSum.length() < 0.1f;
        bool ballsStopped = true;
        
        bool done = cueStick->readjustStickToCueball(adjustingStick, ballsStopped);
        if (done){
            endCurrentTurn();
            cameraFollowStick();   
        } 

        // adjustingCamera = true;
    } else if(adjustingCamera){
        using namespace Ogre;
        Vector3 camDirVec = Vector3::ZERO;
        Real thisMove = mKeyboard->isKeyDown(OIS::KC_LSHIFT) ? 
            mMoveSpeed : mMoveSpeed / 2;
    
        if (mKeyboard->isKeyDown(OIS::KC_W))
            camDirVec += (mCamera->getDirection() * thisMove);

        if (mKeyboard->isKeyDown(OIS::KC_S))
            camDirVec -= (mCamera->getDirection() * thisMove);

        if (mKeyboard->isKeyDown(OIS::KC_A))
            camDirVec -= (mCamera->getDirection().crossProduct(mCamera->getUp()) * thisMove);

        if (mKeyboard->isKeyDown(OIS::KC_D))
            camDirVec += (mCamera->getDirection().crossProduct(mCamera->getUp()) * thisMove);
        
        mCamera->move(camDirVec * evt.timeSinceLastFrame);
    } else if(hitBall) {        
        if (cueStickTotal > CUE_STICK_MIN) 
            incrementStrokeCount();
        cueStick->releaseStick(adjustingStick, hitBall, cueStickTotal, cueStickDelta);

    }
    else {
        cueStick->rotateToMouseInput(cueStickRotationX, cueStickRotationY);
        needToUpdateCamera = true;
        cueStick->chargeStick(adjustingStick, cueStickTotal, cueStickDelta, LMBDown);
    }
}

void ThreeDPool::physicsLoop()
{
    if (physicsEngine == NULL)
        return;
    
    physicsEngine->getDynamicsWorld()->stepSimulation(1.0f/60.0f); //suppose you have 60 frames per second
    if (needToUpdateCamera) {
        pCamera->moveCameraToStick(cueStick);
        needToUpdateCamera = false;
    }

    int numManifolds = physicsEngine->getDynamicsWorld()->getDispatcher()->getNumManifolds();
    
    int soundsToPlay = 1;
    
    const float ballSoundThreshold = 5.f;
    
    for (int i = 0; i < numManifolds; i++)
    {
        btPersistentManifold* contactManifold =  physicsEngine->getDynamicsWorld()->getDispatcher()->getManifoldByIndexInternal(i);
        const btCollisionObject* obA = contactManifold->getBody0();
        const btCollisionObject* obB = contactManifold->getBody1();

        objType obAType = typeMap[(size_t) (obA->getUserPointer())];
        objType obBType = typeMap[(size_t) (obB->getUserPointer())];
 
        if (soundOn) {
            if((obAType == stickType && obBType == cueBallType) || (obBType == stickType && obAType == cueBallType)) {
                Mix_PlayChannel(-1, stick_ball, 0);
            }
            else if(obAType == ballType && obBType == ballType) {
                const btRigidBody* body1 = btRigidBody::upcast(obA);
                const btRigidBody* body2 = btRigidBody::upcast(obB);
                if (body1->getLinearVelocity().length() > ballSoundThreshold || body2->getLinearVelocity().length() > ballSoundThreshold) {
                    if (soundsToPlay-- > 0)
                        Mix_PlayChannel(-1, ball_ball, 0);
                }
            }
            else if((obAType == ballType && obBType == cueBallType) || (obBType == ballType && obAType == cueBallType)) {
                const btRigidBody* body1 = btRigidBody::upcast(obA);
                const btRigidBody* body2 = btRigidBody::upcast(obB);
                if (body1->getLinearVelocity().length() > ballSoundThreshold || body2->getLinearVelocity().length() > ballSoundThreshold)
                    if (soundsToPlay-- > 0)
                        Mix_PlayChannel(-1, ball_ball, 0);
            }
        }
        
        if((obAType == pocketType && obBType == ballType) || (obBType == pocketType && obAType == ballType)) {
            void* usr = obAType == ballType ? obA->getUserPointer() : obB->getUserPointer();
            Ogre::SceneNode* node = static_cast<Ogre::SceneNode*>(usr);
                        
            Ball* ball = pocketMap[node];
            
            ball->removeFromWorld();
            
            if (!ballsAssignedToPlayers) {
                getActivePlayer()->setRedBall(ball->redBall);
                getInactivePlayer()->setRedBall(!(ball->redBall));
                ballsAssignedToPlayers = true;
            }
            
            decrementRemainingBallCount(ball->redBall);
        }
        
            
        if((obAType == pocketType && obBType == cueBallType) || (obBType == pocketType && obAType == cueBallType)) {
            void* usr = obAType == cueBallType ? obA->getUserPointer() : obB->getUserPointer();
            Ogre::SceneNode* node = static_cast<Ogre::SceneNode*>(usr);

            Ball* cueBall = pocketMap[node];
            
            incrementStrokeCount();
            
            cueBall->resetCueBall();
        }
    }
}

bool ThreeDPool::quit (const CEGUI::EventArgs& e) {
    std::string msg = "quit";
    if (!isServer) {
        nm->messageServer(PROTOCOL_TCP, msg.c_str(), msg.length());
    }
    else {
        nm->messageClients(PROTOCOL_TCP, msg.c_str(), msg.length());
    }

    mShutDown = true;
    return true;
}

//--------Camera Section-------//
void ThreeDPool::createCamera(void){
    pCamera = new PlayerCamera(mSceneMgr->createCamera("PlayerCam"));
    mCamera = pCamera->getCamera();
    mCamera->setPosition(Ogre::Vector3(200, 200, 200));
    mCamera->setNearClipDistance(1);
}

void ThreeDPool::cameraFollowStick(void)
{
    btVector3 btPos = cueStick->getBody()->getCenterOfMassPosition();
    Ogre::Vector3 cueStickPos(float(btPos.x()),float(btPos.y()), float(btPos.z()));
    mCamera->setPosition(cueStickPos + cameraOffset);
    mCamera->lookAt(cueStickPos);
}

bool ThreeDPool::configure() {
    if(mRoot->showConfigDialog()) {
        mWindow = mRoot->initialise(true, "3-D Pool");
    } else {
        return false;
    }
}

//---------------------------------------------------------------------------

//#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
//#define WIN32_LEAN_AND_MEAN
//#include "windows.h"
//#endif

#ifdef __cplusplus
extern "C" {
#endif

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
    INT WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT)
#else
    int main(int argc, char *argv[])
#endif
    {
        // Create application object
        ThreeDPool app;

        try {
            app.go();
        } catch(Ogre::Exception& e)  {
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
            MessageBox(NULL, e.getFullDescription().c_str(), "An exception has occurred!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
#else
            std::cerr << "An exception has occurred: " <<
                e.getFullDescription().c_str() << std::endl;
#endif
        }

        return 0;
    }

#ifdef __cplusplus
}
#endif

//---------------------------------------------------------------------------
