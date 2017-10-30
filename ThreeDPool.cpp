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

Ogre::Vector3 cameraOffset;
Stick* cueStickObject;
btRigidBody* cueStick;
Ball* cueBallObject;
btRigidBody* cueBall;
Room* room;
int isServer;

Ogre::Vector3 preFreeLookCameraPosition;
Ogre::Vector3 preFreeLookCameraDirection;

const float CUE_STICK_MAX = 150.0f, CUE_STICK_MIN = 50.0f, STICK_POWER_MULT = 0.15f;

std::vector<Ball*> balls;

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
        guiInitialized(false),
        mainMenuScreenCreated(false),
        mpLobbyScreenCreated(false),
        gameScreenCreated(false),
        hostName(""),
        port(59000),
        isWaiting(false)
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

    createMainMenu();

    return true;
};

void ThreeDPool::initGUI()
{
    mRenderer = &CEGUI::OgreRenderer::bootstrapSystem();
    CEGUI::ImageManager::setImagesetDefaultResourceGroup("Imagesets");
    CEGUI::Font::setDefaultResourceGroup("Fonts");
    CEGUI::Scheme::setDefaultResourceGroup("Schemes");
    CEGUI::WidgetLookManager::setDefaultResourceGroup("LookNFeel");
    CEGUI::WindowManager::setDefaultResourceGroup("Layouts");

    CEGUI::SchemeManager::getSingleton().createFromFile("TaharezLook.scheme");
    CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().setDefaultImage("TaharezLook/MouseArrow");

    CEGUI::WindowManager &wmgr = CEGUI::WindowManager::getSingleton();
    CEGUI::Window *sheet = wmgr.createWindow("DefaultWindow", "ThreeDPool/Sheet");
    CEGUI::GUIContext& context = CEGUI::System::getSingleton().getDefaultGUIContext();
    context.setRootWindow(sheet);
    
    guiInitialized = true;
}

void ThreeDPool::hideAllScreens() 
{
    if (!guiInitialized)
        initGUI();
    
    CEGUI::WindowManager& wmgr = CEGUI::WindowManager::getSingleton();
    CEGUI::GUIContext& context = CEGUI::System::getSingleton().getDefaultGUIContext();
    CEGUI::Window* sheet = context.getRootWindow();
    
    for (size_t index = 0; index < sheet->getChildCount(); ++index) {
        CEGUI::Window* screen = sheet->getChildAtIdx(index);
        
        screen->hide();
    }    

    CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().show();
}

void ThreeDPool::createMainMenu() 
{       
    if (!guiInitialized)
        initGUI();
    
    CEGUI::WindowManager& wmgr = CEGUI::WindowManager::getSingleton();
    CEGUI::GUIContext& context = CEGUI::System::getSingleton().getDefaultGUIContext();
    CEGUI::Window* sheet = context.getRootWindow();
    
    if (!mainMenuScreenCreated) {
        hideAllScreens();
        
        //----Main Menu Screen----//
        CEGUI::Window* mainMenu = wmgr.createWindow("DefaultWindow", "MainMenuScreen");
        mainMenu->setSize(CEGUI::USize(CEGUI::UDim(1, 0), CEGUI::UDim(1, 0)));
        mainMenu->setPosition(CEGUI::UVector2(CEGUI::UDim(0, 0), CEGUI::UDim(0, 0)));
        mainMenu->setAlwaysOnTop(true);
        
        sheet->addChild(mainMenu);
        
        //----Back Ground----//   
        CEGUI::ImageManager::getSingleton().addFromImageFile("BackgroundImage", "ThreeDPoolBackground.png", "Imagesets");
        CEGUI::Window* background = wmgr.createWindow("TaharezLook/StaticImage", "DefaultBackground");
        background->setProperty("Image", "BackgroundImage");
        background->setSize(CEGUI::USize(CEGUI::UDim(1, 0), CEGUI::UDim(1, 0)));
        background->setPosition(CEGUI::UVector2(CEGUI::UDim(0, 0), CEGUI::UDim(0, 0)));
        background->setAlwaysOnTop(false);
        sheet->addChild(background);
        
        //----Quit Button----//
        CEGUI::Window *quit = wmgr.createWindow("TaharezLook/Button", "QuitButton");
        quit->setText("Quit");    

        // In UDim, only set one of the two params, the other should be 0
        quit->setSize(CEGUI::USize(CEGUI::UDim(0.15, 0), CEGUI::UDim(0.05, 0)));
        quit->setPosition(CEGUI::UVector2(CEGUI::UDim(0.425, 0), CEGUI::UDim(0.86, 0)));
        quit->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&ThreeDPool::quit, this));

        mainMenu->addChild(quit);

        //----Single Player----//
        CEGUI::Window *singlePlayer = wmgr.createWindow("TaharezLook/Button", "StartSinglePlayerButton");
        singlePlayer->setText("Single Player");    

        // In UDim, only set one of the two params, the other should be 0
        singlePlayer->setSize(CEGUI::USize(CEGUI::UDim(0.15, 0), CEGUI::UDim(0.05, 0)));
        singlePlayer->setPosition(CEGUI::UVector2(CEGUI::UDim(0.425, 0), CEGUI::UDim(0.74, 0)));
        singlePlayer->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&ThreeDPool::createScene, this));

        mainMenu->addChild(singlePlayer);

        //----Multi Player----//
        CEGUI::Window *multiPlayer = wmgr.createWindow("TaharezLook/Button", "StartMultiPlayerButton");
        multiPlayer->setText("Multi Player");    

        // In UDim, only set one of the two params, the other should be 0
        multiPlayer->setSize(CEGUI::USize(CEGUI::UDim(0.15, 0), CEGUI::UDim(0.05, 0)));
        multiPlayer->setPosition(CEGUI::UVector2(CEGUI::UDim(0.425, 0), CEGUI::UDim(0.80, 0)));
        multiPlayer->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&ThreeDPool::createMPLobby, this));

        mainMenu->addChild(multiPlayer);
        
        mainMenuScreenCreated = true;
    } else {
        hideAllScreens();
        sheet->getChild("MainMenuScreen")->show();
        sheet->getChild("DefaultBackground")->show();
    }
}

void ThreeDPool::onIPEnterBoxKeyPressed (const CEGUI::EventArgs& e) 
{
    using namespace CEGUI;
    
    //Cast it to a key event
    const KeyEventArgs& key = static_cast<const KeyEventArgs&>(e);
    
    if (key.scancode == Key::Return) {
        joinMultiplayer();
    }
}

void ThreeDPool::createMPLobby(void) 
{ 
    if (!guiInitialized)
        initGUI();
    
    CEGUI::WindowManager& wmgr = CEGUI::WindowManager::getSingleton();
    CEGUI::GUIContext& context = CEGUI::System::getSingleton().getDefaultGUIContext();
    CEGUI::Window* sheet = context.getRootWindow();
        
    if (!mpLobbyScreenCreated) {
        hideAllScreens();
        
        // Create Lobby                
        CEGUI::Window* mpLobby = wmgr.createWindow("DefaultWindow", "MPLobbyScreen");
        mpLobby->setSize(CEGUI::USize(CEGUI::UDim(1, 0), CEGUI::UDim(1, 0)));
        mpLobby->setPosition(CEGUI::UVector2(CEGUI::UDim(0, 0), CEGUI::UDim(0, 0)));
        
        sheet->addChild(mpLobby);
        
        //----Back to Main----//
        CEGUI::Window *back = wmgr.createWindow("TaharezLook/Button", "BackToMainButton");
        back->setText("Back");
        back->setSize(CEGUI::USize(CEGUI::UDim(0.15, 0), CEGUI::UDim(0.05, 0)));
        back->setPosition(CEGUI::UVector2(CEGUI::UDim(0, 0), CEGUI::UDim(0, 0)));
        back->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&ThreeDPool::createMainMenu, this));
        mpLobby->addChild(back);
        
        //----Host Game-----------------//
        CEGUI::Window *hostGame = wmgr.createWindow("TaharezLook/Button", "HostGameButton");
        hostGame->setText("Host Game");
        hostGame->setSize(CEGUI::USize(CEGUI::UDim(0.15, 0), CEGUI::UDim(0.05, 0)));
        hostGame->setPosition(CEGUI::UVector2(CEGUI::UDim(0.3, 0), CEGUI::UDim(0.8, 0)));
        hostGame->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&ThreeDPool::startWaiting, this));
        mpLobby->addChild(hostGame);
        
        //----Join Game-----------------//
        CEGUI::Window *joinGame = wmgr.createWindow("TaharezLook/Button", "JoinGameButton");
        joinGame->setText("Join Game");
        joinGame->setSize(CEGUI::USize(CEGUI::UDim(0.15, 0), CEGUI::UDim(0.05, 0)));
        joinGame->setPosition(CEGUI::UVector2(CEGUI::UDim(0.55, 0), CEGUI::UDim(0.8, 0)));
        joinGame->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&ThreeDPool::showEnterIPWindow, this));
        mpLobby->addChild(joinGame);
        
        //--------Enter Server IP-------//
        CEGUI::FrameWindow *enterIP = static_cast<CEGUI::FrameWindow*>(wmgr.createWindow("TaharezLook/FrameWindow", "EnterIPWindow"));
        enterIP->setText("Enter Server IP");
        enterIP->setSize(CEGUI::USize(CEGUI::UDim(0.6, 0), CEGUI::UDim(0.4, 0)));
        enterIP->setPosition(CEGUI::UVector2(CEGUI::UDim(0.2, 0), CEGUI::UDim(0.3, 0)));
        enterIP->setAlwaysOnTop(true);
        enterIP->subscribeEvent(CEGUI::FrameWindow::EventCloseClicked, CEGUI::Event::Subscriber(&ThreeDPool::hideEnterIPWindow, this));
        enterIP->hide();
        mpLobby->addChild(enterIP);
                
        //------------IP Enter Box------//
        CEGUI::Editbox *ipEnterBox = static_cast<CEGUI::Editbox*>(wmgr.createWindow("TaharezLook/Editbox", "IPEnterBox"));
        ipEnterBox->setSize(CEGUI::USize(CEGUI::UDim(0.5, 0), CEGUI::UDim(0.2, 0)));
        ipEnterBox->setPosition(CEGUI::UVector2(CEGUI::UDim(0.25, 0), CEGUI::UDim(0.4, 0)));
        ipEnterBox->subscribeEvent(CEGUI::Editbox::EventKeyDown, CEGUI::Event::Subscriber(&ThreeDPool::onIPEnterBoxKeyPressed, this));
        enterIP->addChild(ipEnterBox);
        
        //------------Go Button---------//
        CEGUI::Window *goButton = wmgr.createWindow("TaharezLook/Button", "JoinGameButton");
        goButton->setText("Go");
        goButton->setSize(CEGUI::USize(CEGUI::UDim(0.1, 0), CEGUI::UDim(0.1, 0)));
        goButton->setPosition(CEGUI::UVector2(CEGUI::UDim(0.45, 0), CEGUI::UDim(0.7, 0)));
        goButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&ThreeDPool::joinMultiplayer, this));
        enterIP->addChild(goButton);
        
        //--------Waiting for Client----//
        CEGUI::FrameWindow *waiting = static_cast<CEGUI::FrameWindow*>(wmgr.createWindow("TaharezLook/FrameWindow", "WaitingWindow"));
        waiting->setText("Waiting for Client...");
        waiting->setSize(CEGUI::USize(CEGUI::UDim(0.6, 0), CEGUI::UDim(0.4, 0)));
        waiting->setPosition(CEGUI::UVector2(CEGUI::UDim(0.2, 0), CEGUI::UDim(0.3, 0)));
        waiting->setAlwaysOnTop(true);
        waiting->subscribeEvent(CEGUI::FrameWindow::EventCloseClicked, CEGUI::Event::Subscriber(&ThreeDPool::cancelWaiting, this));
        waiting->hide();
        mpLobby->addChild(waiting);
        
        //------------Information-------//
        CEGUI::Window *serverInfo = wmgr.createWindow("TaharezLook/StaticText", "ServerInfo");
        serverInfo->setSize(CEGUI::USize(CEGUI::UDim(0.4, 0), CEGUI::UDim(0.15, 0)));
        serverInfo->setPosition(CEGUI::UVector2(CEGUI::UDim(0.3, 0), CEGUI::UDim(0.425, 0)));
        waiting->addChild(serverInfo);
        
        sheet->getChild("DefaultBackground")->show();
        
        mpLobbyScreenCreated = true;
    } else {
        hideAllScreens();
        sheet->getChild("MPLobbyScreen")->show();
        sheet->getChild("DefaultBackground")->show();
    }
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
    
//    std::string msg = "Client Request";
//    nm->messageServer(PROTOCOL_TCP, msg.c_str(), msg.length());
    
    isWaiting = true;
    
//    createMultiplayer();
}

void ThreeDPool::hostMultiplayer () 
{
    createMultiplayer();
}

void ThreeDPool::showEnterIPWindow()
{
    CEGUI::WindowManager& wmgr = CEGUI::WindowManager::getSingleton();
    CEGUI::GUIContext& context = CEGUI::System::getSingleton().getDefaultGUIContext();
    CEGUI::Window* mpLobby = context.getRootWindow()->getChild("MPLobbyScreen");
    
    CEGUI::Window* enterIP = mpLobby->getChild("EnterIPWindow");
    
    cancelWaiting();
    enterIP->setPosition(CEGUI::UVector2(CEGUI::UDim(0.2, 0), CEGUI::UDim(0.3, 0)));
    enterIP->show();
}

void ThreeDPool::hideEnterIPWindow()
{
    CEGUI::WindowManager& wmgr = CEGUI::WindowManager::getSingleton();
    CEGUI::GUIContext& context = CEGUI::System::getSingleton().getDefaultGUIContext();
    CEGUI::Window* mpLobby = context.getRootWindow()->getChild("MPLobbyScreen");
        
    mpLobby->getChild("EnterIPWindow")->hide();
    
    isWaiting = false;
}

void ThreeDPool::setUpGUI(void) {    
    if (!guiInitialized)
           initGUI();

    CEGUI::WindowManager &wmgr = CEGUI::WindowManager::getSingleton();
    CEGUI::GUIContext& context = CEGUI::System::getSingleton().getDefaultGUIContext();
    CEGUI::Window* sheet = context.getRootWindow();
    
    if (!gameScreenCreated) {
        hideAllScreens();
        CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().hide();

        //----Game Screen----//
        CEGUI::Window* gameScreen = wmgr.createWindow("DefaultWindow", "GameScreen");
        gameScreen->setSize(CEGUI::USize(CEGUI::UDim(1, 0), CEGUI::UDim(1, 0)));
        gameScreen->setPosition(CEGUI::UVector2(CEGUI::UDim(0, 0), CEGUI::UDim(0, 0)));
        
        sheet->addChild(gameScreen);

        //----Quit Button----//
        CEGUI::Window *quit = wmgr.createWindow("TaharezLook/Button", "QuitButton");
        quit->setText("Quit");    

        // In UDim, only set one of the two params, the other should be 0
        quit->setSize(CEGUI::USize(CEGUI::UDim(0.15, 0), CEGUI::UDim(0.05, 0)));
        quit->setPosition(CEGUI::UVector2(CEGUI::UDim(0, 0), CEGUI::UDim(0, 0)));
        quit->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&ThreeDPool::quit, this));

        quit->hide();
        gameScreen->addChild(quit);

        // Stroke counter
        CEGUI::Window *strokesWin = wmgr.createWindow("TaharezLook/StaticText", "StrokeCount");
        std::stringstream ss;
        ss << "Strokes: " << strokes;
        strokesWin->setText(ss.str());
        strokesWin->setSize(CEGUI::USize(CEGUI::UDim(0.15, 0), CEGUI::UDim(0.05, 0)));
        strokesWin->setPosition(CEGUI::UVector2(CEGUI::UDim(0.80, 0), CEGUI::UDim(0.84, 0)));

        gameScreen->addChild(strokesWin);

        // Remaining Ball Counter
        CEGUI::Window *remainingBallWin = wmgr.createWindow("TaharezLook/StaticText", "RemainingBalls");
        std::stringstream ss3;
        ss3 << "Remaining: " << remainingBalls;
        remainingBallWin->setText(ss3.str());
        remainingBallWin->setSize(CEGUI::USize(CEGUI::UDim(0.15, 0), CEGUI::UDim(0.05, 0)));
        remainingBallWin->setPosition(CEGUI::UVector2(CEGUI::UDim(0.85, 0), CEGUI::UDim(0.1, 0)));
        gameScreen->addChild(remainingBallWin);

        if (isMultiplayer) {
            // Opponent Stroke counter
            CEGUI::Window *oppStrokesWin = wmgr.createWindow("TaharezLook/StaticText", "OppStrokeCount");
            std::stringstream ss2;
            ss2 << "Opp Strokes: " << opponentStrokes;
            oppStrokesWin->setText(ss2.str());
            oppStrokesWin->setSize(CEGUI::USize(CEGUI::UDim(0.15, 0), CEGUI::UDim(0.05, 0)));
            oppStrokesWin->setPosition(CEGUI::UVector2(CEGUI::UDim(0.80, 0), CEGUI::UDim(0.9, 0)));

            gameScreen->addChild(oppStrokesWin);

            // Opponent Remaining Ball Counter
            CEGUI::Window *oppRemainingBallWin = wmgr.createWindow("TaharezLook/StaticText", "OppRemainingBalls");
            std::stringstream ss4;
            ss4 << "Opp Remaining: " << oppRemainingBalls;
            oppRemainingBallWin->setText(ss4.str());
            oppRemainingBallWin->setSize(CEGUI::USize(CEGUI::UDim(0.15, 0), CEGUI::UDim(0.05, 0)));
            oppRemainingBallWin->setPosition(CEGUI::UVector2(CEGUI::UDim(0.85, 0), CEGUI::UDim(0.16, 0)));
            gameScreen->addChild(oppRemainingBallWin);
        }

        CEGUI::Window *youWin = wmgr.createWindow("TaharezLook/StaticText", "YouWin");
        youWin->setText("You Win!");
        youWin->setSize(CEGUI::USize(CEGUI::UDim(0.1, 0), CEGUI::UDim(0.05, 0)));
        youWin->setPosition(CEGUI::UVector2(CEGUI::UDim(0.45, 0), CEGUI::UDim(0.45, 0)));

        youWin->hide();
        gameScreen->addChild(youWin);
        
    } else {
        hideAllScreens();
        sheet->getChild("GameScreen")->show();
    }
}


void ThreeDPool::createMultiplayer(void)
{
    isMultiplayer = true;
    createScene();
}

//---------------------------------------------------------------------------
void ThreeDPool::createScene(void)
{
    gameStarted = true;

    //-------------basic setup stuff-----------------//
    mSceneMgr->setAmbientLight(Ogre::ColourValue(0.5, 0.5, 0.5));
    mSceneMgr->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_ADDITIVE);
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

    physicsEngine = new Simulator();
    physicsEngine->initObjects();

    cueBallObject = new Ball(mSceneMgr, physicsEngine, 0, 0, 240, "cueBall", typeMap, pocketMap, "Example/White", true);
    cueBall = cueBallObject->getBody();

    cueStickObject = new Stick(mSceneMgr, physicsEngine, 0, 0, 240 + CUE_STICK_MIN, "cueStick", CUE_STICK_MAX, CUE_STICK_MIN, STICK_POWER_MULT, cueBall, typeMap);
    cueStick = cueStickObject->getBody();
    
    cameraOffset = Ogre::Vector3(mCamera->getPosition()-cueStickObject->getPosition());
    btVector3 btPos = cueStick->getCenterOfMassPosition();
    Ogre::Vector3 cueStickPos(float(btPos.x()),float(btPos.y()), float(btPos.z()));
    mCamera->lookAt(cueStickPos);
    mCamera->setPosition(cueStickPos + cameraOffset);

    room = new Room(mSceneMgr, physicsEngine);
    
    addPockets();
    addBallPyramid();
    remainingBalls = balls.size();
    oppRemainingBalls = remainingBalls;
    setUpGUI();
    setUpSounds();
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
    // 1st Layer
    /*balls.push_back(new Ball(mSceneMgr, physicsEngine, -15, -15, -255, "b1",  typeMap, pocketMap, "Example/Red"));
    balls.push_back(new Ball(mSceneMgr, physicsEngine, -15,  -5, -255, "b2",  typeMap, pocketMap, "Example/Red"));
    balls.push_back(new Ball(mSceneMgr, physicsEngine, -15,   5, -255, "b3",  typeMap, pocketMap, "Example/Red"));
    balls.push_back(new Ball(mSceneMgr, physicsEngine, -15,  15, -255, "b4",  typeMap, pocketMap, "Example/Red"));
    balls.push_back(new Ball(mSceneMgr, physicsEngine,  -5, -15, -255, "b5",  typeMap, pocketMap, "Example/Red"));
    balls.push_back(new Ball(mSceneMgr, physicsEngine,  -5,  -5, -255, "b6",  typeMap, pocketMap, "Example/Red"));
    balls.push_back(new Ball(mSceneMgr, physicsEngine,  -5,   5, -255, "b7",  typeMap, pocketMap, "Example/Red"));
    balls.push_back(new Ball(mSceneMgr, physicsEngine,  -5,  15, -255, "b8",  typeMap, pocketMap, "Example/Red"));
    balls.push_back(new Ball(mSceneMgr, physicsEngine,   5, -15, -255, "b9",  typeMap, pocketMap, "Example/Red"));
    balls.push_back(new Ball(mSceneMgr, physicsEngine,   5,  -5, -255, "b10", typeMap, pocketMap, "Example/Red"));
    balls.push_back(new Ball(mSceneMgr, physicsEngine,   5,   5, -255, "b11", typeMap, pocketMap, "Example/Red"));
    balls.push_back(new Ball(mSceneMgr, physicsEngine,   5,  15, -255, "b12", typeMap, pocketMap, "Example/Red"));
    balls.push_back(new Ball(mSceneMgr, physicsEngine,  15, -15, -255, "b13", typeMap, pocketMap, "Example/Red"));
    balls.push_back(new Ball(mSceneMgr, physicsEngine,  15,  -5, -255, "b14", typeMap, pocketMap, "Example/Red"));
    balls.push_back(new Ball(mSceneMgr, physicsEngine,  15,   5, -255, "b15", typeMap, pocketMap, "Example/Red"));
    balls.push_back(new Ball(mSceneMgr, physicsEngine,  15,  15, -255, "b16", typeMap, pocketMap, "Example/Red"));
    
    // 2nd Layer
    balls.push_back(new Ball(mSceneMgr, physicsEngine, -10, -10, -245, "b17", typeMap, pocketMap, "Example/Purple"));
    balls.push_back(new Ball(mSceneMgr, physicsEngine, -10,   0, -245, "b18", typeMap, pocketMap, "Example/Purple"));
    balls.push_back(new Ball(mSceneMgr, physicsEngine, -10,  10, -245, "b19", typeMap, pocketMap, "Example/Purple"));
    balls.push_back(new Ball(mSceneMgr, physicsEngine,   0, -10, -245, "b20", typeMap, pocketMap, "Example/Purple"));
    balls.push_back(new Ball(mSceneMgr, physicsEngine,   0,   0, -245, "b21", typeMap, pocketMap, "Example/Purple"));
    balls.push_back(new Ball(mSceneMgr, physicsEngine,   0,  10, -245, "b22", typeMap, pocketMap, "Example/Purple"));
    balls.push_back(new Ball(mSceneMgr, physicsEngine,  10, -10, -245, "b23", typeMap, pocketMap, "Example/Purple"));
    balls.push_back(new Ball(mSceneMgr, physicsEngine,  10,   0, -245, "b24", typeMap, pocketMap, "Example/Purple"));
    balls.push_back(new Ball(mSceneMgr, physicsEngine,  10,  10, -245, "b25", typeMap, pocketMap, "Example/Purple"));
    
    // 3rd Layer
    balls.push_back(new Ball(mSceneMgr, physicsEngine, -5, -5, -235, "b26", typeMap, pocketMap, "Example/Orange"));
    balls.push_back(new Ball(mSceneMgr, physicsEngine, -5,  5, -235, "b27", typeMap, pocketMap, "Example/Orange"));
    balls.push_back(new Ball(mSceneMgr, physicsEngine,  5, -5, -235, "b28", typeMap, pocketMap, "Example/Orange"));
    balls.push_back(new Ball(mSceneMgr, physicsEngine,  5,  5, -235, "b29", typeMap, pocketMap, "Example/Orange"));
    
    // 4th Layer
    balls.push_back(new Ball(mSceneMgr, physicsEngine, 0, 0, -225, "b30", typeMap, pocketMap, "Example/Blue"));

    balls.push_back(new Ball(mSceneMgr, physicsEngine, 200, -200, 20, "b31", typeMap, pocketMap, "Example/GreenOther"));*/
    
    balls.push_back(new Ball(mSceneMgr, physicsEngine, 0, 0, -225, "b1", typeMap, pocketMap, "Example/Blue"));

    balls.push_back(new Ball(mSceneMgr, physicsEngine, -5, 5, -235, "b2", typeMap, pocketMap, "Example/Orange"));
    balls.push_back(new Ball(mSceneMgr, physicsEngine, 5, -5, -235, "b3", typeMap, pocketMap, "Example/Purple"));
    
    balls.push_back(new Ball(mSceneMgr, physicsEngine, 0, 0, -245, "b4", typeMap, pocketMap, "Example/Black"));
    balls.push_back(new Ball(mSceneMgr, physicsEngine, 10, -10, -245, "b5", typeMap, pocketMap, "Example/Red"));
    balls.push_back(new Ball(mSceneMgr, physicsEngine, -10, 10, -245, "b6", typeMap, pocketMap, "Example/GreenOther"));
    
    balls.push_back(new Ball(mSceneMgr, physicsEngine, 5, -5, -255, "b7", typeMap, pocketMap, "Example/Teal"));
    balls.push_back(new Ball(mSceneMgr, physicsEngine, -5, 5, -255, "b8", typeMap, pocketMap, "Example/PinkPurple"));
    balls.push_back(new Ball(mSceneMgr, physicsEngine, 15, -15, -255, "b9", typeMap, pocketMap, "Example/Yellow"));
    balls.push_back(new Ball(mSceneMgr, physicsEngine, -15, 15, -255, "b10", typeMap, pocketMap, "Example/BOrange"));

    balls.push_back(new Ball(mSceneMgr, physicsEngine, 200, -200, 20, "bTest", typeMap, pocketMap, "Example/GreenOther"));
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
    CEGUI::Window* strokesWin = sheet->getChild("GameScreen")->getChild("StrokeCount");
    std::stringstream ss;
    
    ++strokes;
    ss << "Strokes: " << strokes;
    strokesWin->setText(ss.str());
    
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

void ThreeDPool::decrementRemainingBallCount() {
    CEGUI::GUIContext& context = CEGUI::System::getSingleton().getDefaultGUIContext();
    CEGUI::Window* sheet = context.getRootWindow();
    CEGUI::Window* remainingBallWin = sheet->getChild("GameScreen")->getChild("RemainingBalls");
    std::stringstream ss;
    
    if (soundOn)
        Mix_PlayChannel(-1, pocket, 0);
    
    --remainingBalls;
    ss << "Remaining: " << remainingBalls;
    remainingBallWin->setText(ss.str());
    
    if (remainingBalls < 1 && oppRemainingBalls > 0) {
        CEGUI::Window* youWin = sheet->getChild("GameScreen")->getChild("YouWin");
        youWin->show();
    }
    
    if (isMultiplayer) {
        if (isServer) {
            std::stringstream ss2;
            ss2 << "remaining " << remainingBalls;
            std::string msg = ss2.str();
            nm->messageClients(PROTOCOL_TCP, msg.c_str(), msg.length());
        }
        else {
            std::stringstream ss2;
            ss2 << "remaining " << remainingBalls;
            std::string msg = ss2.str();
            nm->messageServer(PROTOCOL_TCP, msg.c_str(), msg.length());
        }
    }
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
    return true;
}

bool ThreeDPool::keyReleased(const OIS::KeyEvent &arg) {
    // CEGUI Injection
    CEGUI::System::getSingleton().getDefaultGUIContext().injectKeyUp((CEGUI::Key::Scan)arg.key);
    
    if (!BaseApplication::keyReleased(arg))
        return false;
    
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
    
    if (!adjustingCamera) {
        if(me.state.buttonDown(OIS::MB_Left))
        {
            cueStickDelta = me.state.Y.rel * 0.05;
        }
        else{
            cueStickRotationX = 0.01 * me.state.X.rel;
            cueStickRotationY = 0.01 * me.state.Y.rel;
        }
    } else {
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
    
    // if(!BaseApplication::mouseReleased(me, id))
    //     return false;
    if(fabs(cueStickTotal) > 0.1)
        if(id==OIS::MB_Left)
            hitBall = true;
    return true;
}

bool ThreeDPool::mousePressed(const OIS::MouseEvent &me, OIS::MouseButtonID id)
{
    // CEGUI Injection
    CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseButtonDown(convertButton(id));
    
    if(!BaseApplication::mousePressed(me, id))
        return false;

    using namespace std;
    switch(id)
    {
        case OIS::MB_Left:
            cout << "Left" << endl;
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
        if (isMultiplayer)
            networkLoop();
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
        }
    }
}

void ThreeDPool::gameLoop(const Ogre::FrameEvent& evt)
{
    if(adjustingStick) {
        if(fabs(cueBall->getLinearVelocity().length())>0.01f){
            cueStick->setLinearVelocity(btVector3(0, 0, 0));
        }
        
        bool ballsStopped = true;
        
        bool done = cueStickObject->readjustStickToCueball(adjustingStick, ballsStopped);
        if (done) cameraFollowStick();

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
        cueStickObject->releaseStick(adjustingStick, hitBall, cueStickTotal, cueStickDelta);

    }
    else {
        cueStickObject->rotateToMouseInput(cueStickRotationX, cueStickRotationY);
        pCamera->moveCameraToStick(cueStickObject);
        cueStickObject->chargeStick(adjustingStick, cueStickTotal, cueStickDelta, LMBDown);
    }
}

void ThreeDPool::physicsLoop()
{
    if (physicsEngine == NULL)
        return;

    physicsEngine->getDynamicsWorld()->stepSimulation(1.0f/60.0f); //suppose you have 60 frames per second
    int length = physicsEngine->getDynamicsWorld()->getCollisionObjectArray().size();
    for (int i = 0; i< length; i++) {
        btCollisionObject* obj = physicsEngine->getDynamicsWorld()->getCollisionObjectArray()[i];
        btRigidBody* body = btRigidBody::upcast(obj);
        if (body && body->getMotionState()){
            btTransform trans;
            body->getMotionState()->getWorldTransform(trans);
            void *userPointer = body->getUserPointer();
            if (userPointer) {
                btQuaternion orientation = trans.getRotation();
                Ogre::SceneNode *sceneNode = static_cast<Ogre::SceneNode*>(userPointer);
                
                sceneNode->setOrientation(Ogre::Quaternion(orientation.getW(), orientation.getX(), orientation.getY(), orientation.getZ()));                
                sceneNode->setPosition(Ogre::Vector3(trans.getOrigin().getX(), trans.getOrigin().getY(), trans.getOrigin().getZ()));
            }
        }
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
            
            decrementRemainingBallCount();
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
    btVector3 btPos = cueStick->getCenterOfMassPosition();
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
