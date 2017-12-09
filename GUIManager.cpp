/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   GUIManager.cpp
 * Author: treyr3
 * 
 * Created on November 19, 2017, 1:37 PM
 */

#include "GUIManager.h"
#include "ThreeDPool.h"
#include <CEGUI/CEGUI.h>
#include <CEGUI/RendererModules/Ogre/Renderer.h>

const std::string GUIManager::lookNFeelClass = "TaharezLook";

const std::string GUIManager::MAIN_MENU   = "MainMenuScreen",
                  GUIManager::MP_LOBBY    = "MPLobbyScreen",
                  GUIManager::BACKGROUND  = "DefaultBackground",
                  GUIManager::GAME_SCREEN = "GameScreen";

GUIManager::GUIManager(ThreeDPool* _game) : 
    game(_game)
{
    mRenderer = &CEGUI::OgreRenderer::bootstrapSystem();
    CEGUI::ImageManager::setImagesetDefaultResourceGroup("Imagesets");
    CEGUI::Font::setDefaultResourceGroup("Fonts");
    CEGUI::Scheme::setDefaultResourceGroup("Schemes");
    CEGUI::WidgetLookManager::setDefaultResourceGroup("LookNFeel");
    CEGUI::WindowManager::setDefaultResourceGroup("Layouts");

    context = &CEGUI::System::getSingleton().getDefaultGUIContext();
    
    CEGUI::SchemeManager::getSingleton().createFromFile(lookNFeelClass + ".scheme");
    context->getMouseCursor().setDefaultImage(lookNFeelClass + "/MouseArrow");

    wmgr = &CEGUI::WindowManager::getSingleton();
    sheet = wmgr->createWindow("DefaultWindow", "ThreeDPool/Sheet");
    context->setRootWindow(sheet);
}

GUIManager::~GUIManager() {
}

void GUIManager::makeBackground(const std::string& filename)
{
    CEGUI::ImageManager::getSingleton().addFromImageFile("BackgroundImage", filename, "Imagesets");
    CEGUI::Window* background = wmgr->createWindow(lookNFeelClass + "/StaticImage", BACKGROUND);
    background->setSize(CEGUI::USize(CEGUI::UDim(1, 0), CEGUI::UDim(1, 0)));
    background->setPosition(CEGUI::UVector2(CEGUI::UDim(0, 0), CEGUI::UDim(0, 0)));
    background->setAlwaysOnTop(false);
    background->setProperty("Image", "BackgroundImage");
    this->screens[BACKGROUND] = background;
    sheet->addChild(background);
}

void GUIManager::makeScreen(const std::string& name)
{
    CEGUI::Window* screen = wmgr->createWindow("DefaultWindow", name);
    screen->setSize(CEGUI::USize(CEGUI::UDim(1, 0), CEGUI::UDim(1, 0)));
    screen->setPosition(CEGUI::UVector2(CEGUI::UDim(0, 0), CEGUI::UDim(0, 0)));
    screen->setAlwaysOnTop(true);
    this->screens[name] = screen;
    sheet->addChild(screen);
}

CEGUI::Window* GUIManager::makeWindow (const std::string& parentScreen,
        const std::string& type, const std::string& name,
        float xRelSize, float yRelSize, float xRelPos, float yRelPos,
        bool useLookNFeel)
{
    CEGUI::Window* win = wmgr->createWindow(
            useLookNFeel ? lookNFeelClass + "/" + type : type, name);
    win->setSize(CEGUI::USize(CEGUI::UDim(xRelSize, 0), CEGUI::UDim(yRelSize, 0)));
    win->setPosition(CEGUI::UVector2(CEGUI::UDim(xRelPos, 0), CEGUI::UDim(yRelPos, 0)));
    this->screens[parentScreen]->addChild(win);
    
    return win;
}

void GUIManager::hideAllScreens()
{
    for (size_t index = 0; index < sheet->getChildCount(); ++index) {
        CEGUI::Window* screen = sheet->getChildAtIdx(index);
        
        screen->hide();
    }    

    context->getMouseCursor().show();
}

void GUIManager::createMainMenu()
{
    if (!this->screens[MAIN_MENU]) {
        hideAllScreens();
        
        //----Back Ground----//   
        makeBackground("ThreeDPoolBackground.png");
        
        //----Main Menu Screen----//
        makeScreen(MAIN_MENU);        
 
        //----Quit Button----//
        CEGUI::Window* quit = makeWindow(MAIN_MENU, "Button", "QuitButton", 0.15, 0.05, 0.425, 0.86);  
        quit->setText("Quit");
        quit->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&ThreeDPool::quit, game));
      
        //----Single Player----//
        CEGUI::Window* singlePlayer = makeWindow(MAIN_MENU, "Button", "StartSinglePlayerButton", 0.15, 0.05, 0.425, 0.74);
        singlePlayer->setText("Single Player");    
        singlePlayer->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&ThreeDPool::createScene, game));

        //----Multi Player----//
        CEGUI::Window *multiPlayer = makeWindow(MAIN_MENU, "Button", "StartMultiPlayerButton", 0.15, 0.05, 0.425, 0.80);
        multiPlayer->setText("Multi Player");    
        multiPlayer->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&GUIManager::createMPLobby, this));
    } else {
        hideAllScreens();
        this->screens[MAIN_MENU]->show();
        this->screens[BACKGROUND]->show();
    }
}

void GUIManager::createMPLobby() 
{
    if (!this->screens[MP_LOBBY]) {
        hideAllScreens();
        
        // Create Lobby             
        makeScreen(MP_LOBBY);
        
        //----Back to Main----//
        CEGUI::Window *back = makeWindow(MP_LOBBY, "Button", "BackToMainButton", 0.15, 0.05, 0.0, 0.0);
        back->setText("Back");
        back->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&GUIManager::createMainMenu, this));
        
        //----Host Game-----------------//
        CEGUI::Window *hostGame = makeWindow(MP_LOBBY, "Button", "HostGameButton", 0.15, 0.05, 0.3, 0.8);
        hostGame->setText("Host Game");
        hostGame->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&ThreeDPool::startWaiting, game));
        
        //----Join Game-----------------//
        CEGUI::Window *joinGame = makeWindow(MP_LOBBY, "Button", "JoinGameButton", 0.15, 0.05, 0.55, 0.8);
        joinGame->setText("Join Game");
        joinGame->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&GUIManager::showEnterIPWindow, this));
        
        //--------Enter Server IP-------//
        CEGUI::FrameWindow *enterIP = static_cast<CEGUI::FrameWindow*>(makeWindow(MP_LOBBY, "FrameWindow", "EnterIPWindow", 0.6, 0.4, 0.2, 0.3));
        enterIP->setText("Enter Server IP");
        enterIP->setAlwaysOnTop(true);
        enterIP->subscribeEvent(CEGUI::FrameWindow::EventCloseClicked, CEGUI::Event::Subscriber(&GUIManager::hideEnterIPWindow, this));
        enterIP->hide();
                
        //------------IP Enter Box------//
        CEGUI::Editbox *ipEnterBox = static_cast<CEGUI::Editbox*>(wmgr->createWindow(lookNFeelClass + "/Editbox", "IPEnterBox"));
        ipEnterBox->setSize(CEGUI::USize(CEGUI::UDim(0.5, 0), CEGUI::UDim(0.2, 0)));
        ipEnterBox->setPosition(CEGUI::UVector2(CEGUI::UDim(0.25, 0), CEGUI::UDim(0.4, 0)));
        ipEnterBox->subscribeEvent(CEGUI::Editbox::EventKeyDown, CEGUI::Event::Subscriber(&GUIManager::onIPEnterBoxKeyPressed, this));
        enterIP->addChild(ipEnterBox);
        
        //------------Go Button---------//
        CEGUI::Window *goButton = wmgr->createWindow(lookNFeelClass + "/Button", "JoinGameButton");
        goButton->setText("Go");
        goButton->setSize(CEGUI::USize(CEGUI::UDim(0.1, 0), CEGUI::UDim(0.1, 0)));
        goButton->setPosition(CEGUI::UVector2(CEGUI::UDim(0.45, 0), CEGUI::UDim(0.7, 0)));
        goButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&ThreeDPool::joinMultiplayer, game));
        enterIP->addChild(goButton);
        
        //--------Waiting for Client----//
        CEGUI::FrameWindow *waiting = static_cast<CEGUI::FrameWindow*>(makeWindow(MP_LOBBY, "FrameWindow", "WaitingWindow", 0.6, 0.4, 0.2, 0.3));
        waiting->setText("Waiting for Client...");
        waiting->setAlwaysOnTop(true);
        waiting->subscribeEvent(CEGUI::FrameWindow::EventCloseClicked, CEGUI::Event::Subscriber(&ThreeDPool::cancelWaiting, game));
        waiting->hide();
        
        //------------Information-------//
        CEGUI::Window *serverInfo = wmgr->createWindow(lookNFeelClass + "/StaticText", "ServerInfo");
        serverInfo->setSize(CEGUI::USize(CEGUI::UDim(0.4, 0), CEGUI::UDim(0.15, 0)));
        serverInfo->setPosition(CEGUI::UVector2(CEGUI::UDim(0.3, 0), CEGUI::UDim(0.425, 0)));
        waiting->addChild(serverInfo);
        
        this->screens[BACKGROUND]->show();
    } else {
        hideAllScreens();
        this->screens[MP_LOBBY]->show();
        this->screens[BACKGROUND]->show();
    }
}

void GUIManager::showEnterIPWindow()
{
    CEGUI::Window* enterIP = this->screens[MP_LOBBY]->getChild("EnterIPWindow");
    
    game->cancelWaiting();
    
    enterIP->setPosition(CEGUI::UVector2(CEGUI::UDim(0.2, 0), CEGUI::UDim(0.3, 0)));
    enterIP->show();
}

void GUIManager::hideEnterIPWindow()
{
    game->isWaiting = false;
    this->screens[MP_LOBBY]->getChild("EnterIPWindow")->hide();
}

void GUIManager::onIPEnterBoxKeyPressed(const CEGUI::EventArgs& e)
{
    using namespace CEGUI;
    
    //Cast it to a key event
    const KeyEventArgs& key = static_cast<const KeyEventArgs&>(e);
    
    if (key.scancode == Key::Return) {
        game->joinMultiplayer();
    } else if (key.scancode == Key::Backspace) {
        CEGUI::Window* ipEnterBox = sheet->getChild(MP_LOBBY + "/EnterIPWindow/IPEnterBox");

        std::string text = ipEnterBox->getText().c_str();
        std::string backspacedText = text.substr(0, text.length() - 1);
        ipEnterBox->setText(backspacedText);
    }
}

void GUIManager::setUpGUI()
{    
    if (!this->screens[GAME_SCREEN]) {
        hideAllScreens();
        CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().hide();

        //----Game Screen----//
        makeScreen(GAME_SCREEN);

        //----Quit Button----//
        CEGUI::Window *quit = makeWindow(GAME_SCREEN, "Button", "QuitButton", 0.15, 0.05, 0.0, 0.0);
        quit->setText("Quit");    
        quit->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&ThreeDPool::quit, game));
        quit->hide();

        // Stroke counter
        CEGUI::Window *strokesWin = makeWindow(GAME_SCREEN, "StaticText", "StrokeCount", 0.15, 0.05, 0.8, 0.84);
        std::stringstream ss;
        ss << "Strokes: " << game->strokes;
        strokesWin->setText(ss.str());
        strokesWin->setSize(CEGUI::USize(CEGUI::UDim(0.15, 0), CEGUI::UDim(0.05, 0)));
        strokesWin->setPosition(CEGUI::UVector2(CEGUI::UDim(0.80, 0), CEGUI::UDim(0.84, 0)));
        strokesWin->hide();

        // Red Remaining Ball Counter
        CEGUI::Window *redBallsRemainingWin = makeWindow(GAME_SCREEN, "StaticText", "RedBallsRemaining", 0.15, 0.05, 0.8, 0.79);
        std::stringstream ss3;
        ss3 << "Red: " << game->redBallsRemaining;
        redBallsRemainingWin->setText(ss3.str());
        redBallsRemainingWin->setSize(CEGUI::USize(CEGUI::UDim(0.15, 0), CEGUI::UDim(0.05, 0)));
        redBallsRemainingWin->setPosition(CEGUI::UVector2(CEGUI::UDim(0.80, 0), CEGUI::UDim(0.79, 0)));
        
        // Blue Remaining Ball Counter
        CEGUI::Window *blueBallsRemainingWin = makeWindow(GAME_SCREEN, "StaticText", "BlueBallsRemaining", 0.15, 0.05, 0.8, 0.7);
        std::stringstream ss4;
        ss4 << "Blue: " << game->blueBallsRemaining;
        blueBallsRemainingWin->setText(ss4.str());
        blueBallsRemainingWin->setSize(CEGUI::USize(CEGUI::UDim(0.15, 0), CEGUI::UDim(0.05, 0)));
        blueBallsRemainingWin->setPosition(CEGUI::UVector2(CEGUI::UDim(0.80, 0), CEGUI::UDim(0.70, 0)));
        
        // Opponent Remaining Ball Counter
        CEGUI::Window *targettingColorWin = makeWindow(GAME_SCREEN, "StaticText", "TargettingColor", 0.15, 0.05, 0.8, 0.55);
        targettingColorWin->setText("Targetting: All");
        targettingColorWin->setSize(CEGUI::USize(CEGUI::UDim(0.15, 0), CEGUI::UDim(0.05, 0)));
        targettingColorWin->setPosition(CEGUI::UVector2(CEGUI::UDim(0.80, 0), CEGUI::UDim(0.55, 0)));
        
        // Opponent Title
        CEGUI::Window *activePlayer = makeWindow(GAME_SCREEN, "StaticText", "ActivePlayer", 0.15, 0.05, 0.8, 0.5);
        activePlayer->setText("Player 1's Turn");
        activePlayer->setSize(CEGUI::USize(CEGUI::UDim(0.15, 0), CEGUI::UDim(0.05, 0)));
        activePlayer->setPosition(CEGUI::UVector2(CEGUI::UDim(0.80, 0), CEGUI::UDim(0.5, 0)));

        // Opponent Stroke counter
        CEGUI::Window *oppStrokesWin = makeWindow(GAME_SCREEN, "StaticText", "OppStrokeCount", 0.15, 0.05, 0.8, 0.6);
        std::stringstream ss2;
        ss2 << "Strokes: " << game->opponentStrokes;
        oppStrokesWin->setText(ss2.str());
        oppStrokesWin->hide();

        // You Win Window
        CEGUI::Window *youWin = makeWindow(GAME_SCREEN, "StaticText", "YouWin", 0.15, 0.05, 0.425, 0.475);
        youWin->setText("You Win!");
        youWin->hide();
        
        // Shot Power Bar
        CEGUI::ProgressBar* powerBar = static_cast<CEGUI::ProgressBar*>(makeWindow(GAME_SCREEN, "ProgressBar", "PowerBar", 0.4, 0.05, 0.3, 0.9));
        powerBar->setProgress(0.0);
        powerBar->hide();
        
    } else {
        hideAllScreens();
        this->screens[GAME_SCREEN]->show();
    }
}

void GUIManager::setPowerBar(float progress) {
    CEGUI::ProgressBar* powerBar = static_cast<CEGUI::ProgressBar*>(this->screens[GAME_SCREEN]->getChild("PowerBar"));
    powerBar->setProgress(progress);
    powerBar->show();
}

void GUIManager::hidePowerBar() {
    CEGUI::ProgressBar* powerBar = static_cast<CEGUI::ProgressBar*>(this->screens[GAME_SCREEN]->getChild("PowerBar"));
    powerBar->setProgress(0.0);
    powerBar->hide();
}