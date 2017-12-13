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
#include <sstream>
#include "Player.h"

const std::string GUIManager::lookNFeelClass = "TaharezLook";

const float GUIManager::PROGRESS_MAX_ALPHA = 0.75f;

const std::string GUIManager::MAIN_MENU   = "MainMenuScreen",
                  GUIManager::PLAYER_SELECT   = "PlayerSelectScreen",
                  GUIManager::MP_LOBBY    = "MPLobbyScreen",
                  GUIManager::BACKGROUND  = "DefaultBackground",
                  GUIManager::GAME_SCREEN = "GameScreen";

GUIManager::GUIManager(ThreeDPool* _game) : 
    game(_game),
    progressAlpha(PROGRESS_MAX_ALPHA),
    justCheckedBox(false), justCheckedBox2(false), p1Selected(false), p2Selected(false),
    p1Type(0), p2Type(0), p1Diff(0), p2Diff(0)
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
        CEGUI::Window* quit = makeWindow(MAIN_MENU, "Button", "QuitButton", 0.15, 0.05, 0.425, 0.80);  
        quit->setText("Quit");
        quit->setProperty("HoverTextColour", "tl:FFFF0000 tr:FFFF0000 bl:FFFF0000 br:FFFF0000");
        quit->setProperty("PushedTextColour", "tl:FFFF0000 tr:FFFF0000 bl:FFFF0000 br:FFFF0000");
        quit->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&ThreeDPool::quit, game));
      
        //----Single Player----//
        CEGUI::Window* singlePlayer = makeWindow(MAIN_MENU, "Button", "StartSinglePlayerButton", 0.15, 0.05, 0.425, 0.74);
        singlePlayer->setProperty("HoverTextColour", "tl:FF00FF00 tr:FF00FF00 bl:FF00FF00 br:FF00FF00");
        singlePlayer->setProperty("PushedTextColour", "tl:FF00FF00 tr:FF00FF00 bl:FF00FF00 br:FF00FF00");
        singlePlayer->setText("Play");    
        singlePlayer->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&GUIManager::createPlayersLobby, this));

        //----Multi Player----//
        // CEGUI::Window *multiPlayer = makeWindow(MAIN_MENU, "Button", "StartMultiPlayerButton", 0.15, 0.05, 0.425, 0.80);
        // multiPlayer->setText("Multi Player");    
        // multiPlayer->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&GUIManager::createMPLobby, this));
    } else {
        hideAllScreens();
        this->screens[MAIN_MENU]->show();
        this->screens[BACKGROUND]->show();
    }
}

void GUIManager::player1SelectManual(){
    p1Type = 0;
    p1Selected = true;
    this->screens[PLAYER_SELECT]->getChild("Player1SelectManual")->setProperty("NormalTextColour", "tl:FF00FF00 tr:FF00FF00 bl:FF00FF00 br:FF00FF00");
    this->screens[PLAYER_SELECT]->getChild("Player1SelectAI")->setProperty("NormalTextColour", "tl:FFFFFFFF tr:FFFFFFFF bl:FFFFFFFF br:FFFFFFFF");
    this->screens[PLAYER_SELECT]->getChild("Player1SelectEasy")->hide();
    this->screens[PLAYER_SELECT]->getChild("Player1SelectMedium")->hide();
    this->screens[PLAYER_SELECT]->getChild("Player1SelectHard")->hide();

    if(p1Selected && p2Selected)
        this->screens[PLAYER_SELECT]->getChild("ReadyToGo")->show();
}

void GUIManager::player2SelectManual(){
    p2Type = 0;
    p2Selected = true;
    this->screens[PLAYER_SELECT]->getChild("Player2SelectManual")->setProperty("NormalTextColour", "tl:FF00FF00 tr:FF00FF00 bl:FF00FF00 br:FF00FF00");
    this->screens[PLAYER_SELECT]->getChild("Player2SelectAI")->setProperty("NormalTextColour", "tl:FFFFFFFF tr:FFFFFFFF bl:FFFFFFFF br:FFFFFFFF");
    this->screens[PLAYER_SELECT]->getChild("Player2SelectEasy")->hide();
    this->screens[PLAYER_SELECT]->getChild("Player2SelectMedium")->hide();
    this->screens[PLAYER_SELECT]->getChild("Player2SelectHard")->hide();

    if(p1Selected && p2Selected)
        this->screens[PLAYER_SELECT]->getChild("ReadyToGo")->show();
}

void GUIManager::player1SelectAI(){
    p1Type = 1;
    p1Selected = true;
    this->screens[PLAYER_SELECT]->getChild("Player1SelectAI")->setProperty("NormalTextColour", "tl:FF00FF00 tr:FF00FF00 bl:FF00FF00 br:FF00FF00");
    this->screens[PLAYER_SELECT]->getChild("Player1SelectManual")->setProperty("NormalTextColour", "tl:FFFFFFFF tr:FFFFFFFF bl:FFFFFFFF br:FFFFFFFF");
    this->screens[PLAYER_SELECT]->getChild("Player1SelectEasy")->show();
    this->screens[PLAYER_SELECT]->getChild("Player1SelectMedium")->show();
    this->screens[PLAYER_SELECT]->getChild("Player1SelectHard")->show();

    if(p1Selected && p2Selected)
        this->screens[PLAYER_SELECT]->getChild("ReadyToGo")->show();
}

void GUIManager::player2SelectAI(){
    p2Type = 1;
    p2Selected = true;
    this->screens[PLAYER_SELECT]->getChild("Player2SelectAI")->setProperty("NormalTextColour", "tl:FF00FF00 tr:FF00FF00 bl:FF00FF00 br:FF00FF00");
    this->screens[PLAYER_SELECT]->getChild("Player2SelectManual")->setProperty("NormalTextColour", "tl:FFFFFFFF tr:FFFFFFFF bl:FFFFFFFF br:FFFFFFFF");
    this->screens[PLAYER_SELECT]->getChild("Player2SelectEasy")->show();
    this->screens[PLAYER_SELECT]->getChild("Player2SelectMedium")->show();
    this->screens[PLAYER_SELECT]->getChild("Player2SelectHard")->show();

    if(p1Selected && p2Selected)
        this->screens[PLAYER_SELECT]->getChild("ReadyToGo")->show();
}

void GUIManager::player1SelectEasy(){
    if(justCheckedBox) return;
    p1Diff = 0;
    justCheckedBox = true;
    this->screens[PLAYER_SELECT]->getChild("Player1SelectMedium")->setProperty("Selected", "False");
    this->screens[PLAYER_SELECT]->getChild("Player1SelectHard")->setProperty("Selected", "False");
    justCheckedBox = false;
}

void GUIManager::player1SelectMedium(){
    if(justCheckedBox) return;
    p1Diff = 1;
    justCheckedBox = true;
    this->screens[PLAYER_SELECT]->getChild("Player1SelectEasy")->setProperty("Selected", "False");
    this->screens[PLAYER_SELECT]->getChild("Player1SelectHard")->setProperty("Selected", "False");
    justCheckedBox = false;
}

void GUIManager::player1SelectHard(){
    if(justCheckedBox) return;
    p1Diff = 2;
    justCheckedBox = true;
    this->screens[PLAYER_SELECT]->getChild("Player1SelectEasy")->setProperty("Selected", "False");
    this->screens[PLAYER_SELECT]->getChild("Player1SelectMedium")->setProperty("Selected", "False");
    justCheckedBox = false;
}

void GUIManager::player2SelectEasy(){
    if(justCheckedBox2) return;
    p2Diff = 0;
    justCheckedBox2 = true;
    this->screens[PLAYER_SELECT]->getChild("Player2SelectMedium")->setProperty("Selected", "False");
    this->screens[PLAYER_SELECT]->getChild("Player2SelectHard")->setProperty("Selected", "False");
    justCheckedBox2 = false;
}

void GUIManager::player2SelectMedium(){
    if(justCheckedBox2) return;
    p2Diff = 1;
    justCheckedBox2 = true;
    this->screens[PLAYER_SELECT]->getChild("Player2SelectEasy")->setProperty("Selected", "False");
    this->screens[PLAYER_SELECT]->getChild("Player2SelectHard")->setProperty("Selected", "False");
    justCheckedBox2 = false;
}

void GUIManager::player2SelectHard(){
    if(justCheckedBox2) return;
    p2Diff = 2;
    justCheckedBox2 = true;
    this->screens[PLAYER_SELECT]->getChild("Player2SelectEasy")->setProperty("Selected", "False");
    this->screens[PLAYER_SELECT]->getChild("Player2SelectMedium")->setProperty("Selected", "False");
    justCheckedBox2 = false;
}


void GUIManager::createPlayersLobby(){
    hideAllScreens();
    this->screens[BACKGROUND]->show();

     if (!this->screens[PLAYER_SELECT]) {

        makeScreen(PLAYER_SELECT);

        CEGUI::Window *back = makeWindow(PLAYER_SELECT, "Button", "BackToMainButton2", 0.15, 0.05, 0.05, 0.9);
        back->setProperty("HoverTextColour", "tl:FF00FF00 tr:FF00FF00 bl:FF00FF00 br:FF00FF00");
        back->setProperty("PushedTextColour", "tl:FF00FF00 tr:FF00FF00 bl:FF00FF00 br:FF00FF00");
        back->setText("Main Menu");
        back->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&GUIManager::createMainMenu, this));        


        //Player 1 (LEFT) Side
        CEGUI::Window *p1Title = makeWindow(PLAYER_SELECT, "StaticText", "Player1Title", 0.15, 0.05, 0.29, 0.26);
        p1Title->setProperty("BackgroundEnabled", "False");
        p1Title->setProperty("FrameEnabled", "False");
        p1Title->setText("Player 1");
        
        CEGUI::Window *p1SelectManual = makeWindow(PLAYER_SELECT, "Button", "Player1SelectManual", 0.13, 0.04, 0.25, 0.32);
        p1SelectManual->setText("Human");
        p1SelectManual->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&GUIManager::player1SelectManual, this));        
        p1SelectManual->setProperty("HoverTextColour", "tl:FF00FF00 tr:FF00FF00 bl:FF00FF00 br:FF00FF00");
        
        CEGUI::Window *p1SelectAI = makeWindow(PLAYER_SELECT, "Button", "Player1SelectAI", 0.13, 0.04, 0.25, 0.37);
        p1SelectAI->setText("Computer");
        p1SelectAI->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&GUIManager::player1SelectAI, this));        
        p1SelectAI->setProperty("HoverTextColour", "tl:FF00FF00 tr:FF00FF00 bl:FF00FF00 br:FF00FF00");

        CEGUI::Window *p1SelectEasy = makeWindow(PLAYER_SELECT, "Checkbox", "Player1SelectEasy", 0.1, 0.05, 0.27, 0.42);
        p1SelectEasy->setProperty("Text", "Easy");
        p1SelectEasy->setProperty("Selected", "True");
        p1SelectEasy->subscribeEvent(CEGUI::ToggleButton::EventSelectStateChanged, CEGUI::Event::Subscriber(&GUIManager::player1SelectEasy, this));
        p1SelectEasy->hide();

        CEGUI::Window *p1SelectMedium = makeWindow(PLAYER_SELECT, "Checkbox", "Player1SelectMedium", 0.1, 0.05, 0.27, 0.45);
        p1SelectMedium->setProperty("Text", "Medium");
        p1SelectMedium->setProperty("Selected", "False");
        p1SelectMedium->subscribeEvent(CEGUI::ToggleButton::EventSelectStateChanged, CEGUI::Event::Subscriber(&GUIManager::player1SelectMedium, this));
        p1SelectMedium->hide();

        CEGUI::Window *p1SelectHard = makeWindow(PLAYER_SELECT, "Checkbox", "Player1SelectHard", 0.1, 0.05, 0.27, 0.48);
        p1SelectHard->setProperty("Text", "Hard");
        p1SelectHard->setProperty("Selected", "False");
        p1SelectHard->subscribeEvent(CEGUI::ToggleButton::EventSelectStateChanged, CEGUI::Event::Subscriber(&GUIManager::player1SelectHard, this));
        p1SelectHard->hide();



        //Player 2 (RIGHT) Side
        CEGUI::Window *p2Title = makeWindow(PLAYER_SELECT, "StaticText", "Player2Title", 0.15, 0.05, 0.66, 0.26);
        p2Title->setProperty("BackgroundEnabled", "False");
        p2Title->setProperty("FrameEnabled", "False");
        p2Title->setText("Player 2");

        CEGUI::Window *p2SelectManual = makeWindow(PLAYER_SELECT, "Button", "Player2SelectManual", 0.13, 0.04, 0.62, 0.32);
        p2SelectManual->setText("Human");
        p2SelectManual->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&GUIManager::player2SelectManual, this));        
        p2SelectManual->setProperty("HoverTextColour", "tl:FF00FF00 tr:FF00FF00 bl:FF00FF00 br:FF00FF00");

        CEGUI::Window *p2SelectAI = makeWindow(PLAYER_SELECT, "Button", "Player2SelectAI", 0.13, 0.04, 0.62, 0.37);
        p2SelectAI->setText("Computer");
        p2SelectAI->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&GUIManager::player2SelectAI, this));        
        p2SelectAI->setProperty("HoverTextColour", "tl:FF00FF00 tr:FF00FF00 bl:FF00FF00 br:FF00FF00");

        CEGUI::Window *p2SelectEasy = makeWindow(PLAYER_SELECT, "Checkbox", "Player2SelectEasy", 0.1, 0.05, 0.64, 0.42);
        p2SelectEasy->setProperty("Text", "Easy");
        p2SelectEasy->setProperty("Selected", "True");
        p2SelectEasy->subscribeEvent(CEGUI::ToggleButton::EventSelectStateChanged, CEGUI::Event::Subscriber(&GUIManager::player2SelectEasy, this));
        p2SelectEasy->hide();

        CEGUI::Window *p2SelectMedium = makeWindow(PLAYER_SELECT, "Checkbox", "Player2SelectMedium", 0.1, 0.05, 0.64, 0.45);
        p2SelectMedium->setProperty("Text", "Medium");
        p2SelectMedium->setProperty("Selected", "False");
        p2SelectMedium->subscribeEvent(CEGUI::ToggleButton::EventSelectStateChanged, CEGUI::Event::Subscriber(&GUIManager::player2SelectMedium, this));
        p2SelectMedium->hide();

        CEGUI::Window *p2SelectHard = makeWindow(PLAYER_SELECT, "Checkbox", "Player2SelectHard", 0.1, 0.05, 0.64, 0.48);
        p2SelectHard->setProperty("Text", "Hard");
        p2SelectHard->setProperty("Selected", "False");
        p2SelectHard->subscribeEvent(CEGUI::ToggleButton::EventSelectStateChanged, CEGUI::Event::Subscriber(&GUIManager::player2SelectHard, this));
        p2SelectHard->hide();


        CEGUI::Window *readyToGo = makeWindow(PLAYER_SELECT, "Button", "ReadyToGo", 0.15, 0.05, 0.415, 0.55);
        readyToGo->setText("Ready to Go");
        readyToGo->setProperty("HoverTextColour", "tl:FFFF0000 tr:FFFF0000 bl:FFFF0000 br:FFFF0000");
        readyToGo->setProperty("PushedTextColour", "tl:FFFF0000 tr:FFFF0000 bl:FFFF0000 br:FFFF0000");
        readyToGo->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&ThreeDPool::createScene, game));
        readyToGo->hide();
    } else {
        this->screens[PLAYER_SELECT]->show();
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

void GUIManager::restartGame(){
    // delete game;
    game = new ThreeDPool();
}

void GUIManager::setUpGUI()
{    
    if (!this->screens[GAME_SCREEN]) {
        hideAllScreens();
        CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().hide();

        //----Game Screen----//
        makeScreen(GAME_SCREEN);


        if(p1Type == 0) p1Name = "Human";
        else {
            p1Name = "Computer (" + std::string((p1Diff == 0) ? "Easy)" : 
                                        (p1Diff == 1) ? "Med)" : 
                                                             "Hard)");    
        }

        if(p2Type == 0) p2Name = "Human";
        else {
            p2Name = "Computer (" + std::string((p2Diff == 0) ? "Easy)" : 
                                        (p2Diff == 1) ? "Med)" : 
                                                            "Hard)");    
        }


        //----Quit Button----//
        CEGUI::Window *quit = makeWindow(GAME_SCREEN, "Button", "QuitButton", 0.15, 0.05, 0.05, 0.05);
        quit->setText("Quit");    
        quit->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&ThreeDPool::quit, game));
        quit->hide();

        // CEGUI::Window *restart = makeWindow(GAME_SCREEN, "Button", "RestartButton", 0.15, 0.05, 0.3, 0.1);
        // restart->setText("Main Menu");    
        // restart->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&GUIManager::restartGame, this));
        // restart->hide();

        // Stroke counter
        CEGUI::Window *strokesWin = makeWindow(GAME_SCREEN, "StaticText", "StrokeCount", 0.15, 0.05, 0.8, 0.84);
        std::stringstream ss;
        ss << "Strokes: " << game->strokes;
        strokesWin->setText(ss.str());
        strokesWin->setProperty("TextColours", "tl:FF00FF00 tr:FF00FF00 bl:FF00FF00 br:FF00FF00");
        strokesWin->hide();

        // Red Remaining Ball Counter
        CEGUI::Window *redBallsRemainingWin = makeWindow(GAME_SCREEN, "StaticText", "SolidBallsRemaining", 0.17, 0.05, 0.75, 0.810);
        std::stringstream ss3;
        ss3 << "Solid: " << game->solidBallsRemaining;
        redBallsRemainingWin->setText(ss3.str());
        redBallsRemainingWin->hide();

        // Blue Remaining Ball Counter
        CEGUI::Window *blueBallsRemainingWin = makeWindow(GAME_SCREEN, "StaticText", "StripedBallsRemaining", 0.17, 0.05, 0.75, 0.755);
        std::stringstream ss4;
        ss4 << "Striped: " << game->stripedBallsRemaining;
        blueBallsRemainingWin->setText(ss4.str());
        blueBallsRemainingWin->hide();

        CEGUI::Window *stats = makeWindow(GAME_SCREEN, "StaticText", "BallsRemaining", 0.17, 0.05, 0.75, 0.7);
        stats->setText("     Balls Remaining  ");
        stats->hide();

        // Opponent Remaining Ball Counter
        CEGUI::Window *targettingColorWin = makeWindow(GAME_SCREEN, "StaticText", "TargetingColor", 0.17, 0.05, 0.75, 0.56);
        targettingColorWin->setText("Targeting: All");
        
        // Opponent Title
        CEGUI::Window *activePlayer = makeWindow(GAME_SCREEN, "StaticText", "ActivePlayer", 0.17, 0.05, 0.75, 0.505);
        activePlayer->setText("Player 1's Turn");


        CEGUI::Window *curTurnTitle = makeWindow(GAME_SCREEN, "StaticText", "curTurnTitle", 0.17, 0.05, 0.75, 0.45);
        curTurnTitle->setText("       Current Turn     ");

        endCurrentTurn();

        // Opponent Stroke counter
        CEGUI::Window *oppStrokesWin = makeWindow(GAME_SCREEN, "StaticText", "OppStrokeCount", 0.17, 0.05, 0.75, 0.6);
        std::stringstream ss2;
        ss2 << "Strokes: " << game->opponentStrokes;
        oppStrokesWin->setText(ss2.str());
        oppStrokesWin->hide();


        // You Win Window
        CEGUI::Window *youWin = makeWindow(GAME_SCREEN, "StaticText", "YouWin", 0.15, 0.05, 0.425, 0.475);
        youWin->setText("You Win!");
        youWin->hide();
        
        // Shot Power Bar
        CEGUI::ProgressBar* powerBar = static_cast<CEGUI::ProgressBar*>(makeWindow(GAME_SCREEN, "ProgressBar", "PowerBar", 0.4, 0.02, 0.3, 0.9));
        std::stringstream ssz;
        ssz << progressAlpha;       
        progressAlpha = 0.0f;
        powerBar->setProperty("Alpha", ssz.str());
        powerBar->setProperty("ReversedProgress", "True");
        powerBar->setProgress(1.0);
        powerBar->hide();

    } else {
        hideAllScreens();
        this->screens[GAME_SCREEN]->show();
    }
}


// void GUIManager::addBallImage(std::string num, float xScale, float yScale, float xpos, float ypos){
//     std::string name = "Ball" + num + "Image";
//     std::string filename = num + ".png";
//     CEGUI::ImageManager::getSingleton().addFromImageFile(name, filename, "Imagesets");
//     CEGUI::Window *ball = makeWindow(GAME_SCREEN, "StaticImage", "Image" + num, xScale, yScale, xpos, ypos);
//     ball->setProperty("Image", name);
//     ball->setProperty("BackgroundEnabled", "True");
//     ball->setProperty("FrameEnabled", "True");
//     ball->setProperty("AlwaysOnTop", "True");    
// }

// void GUIManager::spawnBallImages(){
//     addBallImage("1", 0.02, 0.025, 0.09, 0.2);
//     addBallImage("2", 0.02, 0.025, 0.12, 0.2);
//     addBallImage("3", 0.02, 0.025, 0.15, 0.2);
//     addBallImage("4", 0.02, 0.025, 0.18, 0.2);
//     addBallImage("5", 0.02, 0.025, 0.21, 0.2);
//     addBallImage("6", 0.02, 0.025, 0.24, 0.2);
//     addBallImage("7", 0.02, 0.025, 0.27, 0.2);
//     addBallImage("8", 0.02, 0.025, 0.30, 0.2);
//     addBallImage("9", 0.02, 0.025, 0.33, 0.2);
//     addBallImage("10", 0.02, 0.025, 0.36, 0.2);
//     addBallImage("11", 0.02, 0.025, 0.39, 0.2);
//     addBallImage("12", 0.02, 0.025, 0.42, 0.2);
//     addBallImage("13", 0.02, 0.025, 0.45, 0.2);
//     addBallImage("14", 0.02, 0.025, 0.48, 0.2);
//     addBallImage("15", 0.02, 0.025, 0.51, 0.2);
// }

void GUIManager::fadeInPowerBar() {
    progressAlpha = std::min(progressAlpha + 0.01f, PROGRESS_MAX_ALPHA);
//    std::cout << "fading in "<< progressAlpha << std::endl;
    CEGUI::ProgressBar* powerBar = static_cast<CEGUI::ProgressBar*>(this->screens[GAME_SCREEN]->getChild("PowerBar"));
    powerBar->show();
    std::stringstream ss;
    ss << progressAlpha;       
    powerBar->setProperty("Alpha", ss.str());
}


void GUIManager::setPowerBar(float progress) {
    CEGUI::ProgressBar* powerBar = static_cast<CEGUI::ProgressBar*>(this->screens[GAME_SCREEN]->getChild("PowerBar"));
    powerBar->setProgress(progress);
}

void GUIManager::fadeOutPowerBar() {
    progressAlpha = std::max(std::min(progressAlpha - 0.003f, PROGRESS_MAX_ALPHA), 0.0f);

    CEGUI::ProgressBar* powerBar = static_cast<CEGUI::ProgressBar*>(this->screens[GAME_SCREEN]->getChild("PowerBar"));

    std::stringstream ss;
    ss << progressAlpha;       
    powerBar->setProperty("Alpha", ss.str());
}

void GUIManager::decrementRemainingBallCount(bool redBall) {
    this->screens["GameScreen"]->getChild("SolidBallsRemaining")->show();
    this->screens["GameScreen"]->getChild("StripedBallsRemaining")->show();
    this->screens["GameScreen"]->getChild("BallsRemaining")->show();

    std::stringstream ss, ss2;
    ss << "           " << game->solidBallsRemaining << " Solids";
    this->screens["GameScreen"]->getChild("SolidBallsRemaining")->setText(ss.str());
    ss2 << "           " << game->stripedBallsRemaining << " Striped";
    this->screens["GameScreen"]->getChild("StripedBallsRemaining")->setText(ss2.str());
}

void GUIManager::endCurrentTurn() {
    CEGUI::Window* activePlayer = sheet->getChild("GameScreen")->getChild("ActivePlayer"); 
    activePlayer->setText(game->player1Turn ? ("  P1: " + p1Name) : ("  P2: " + p1Name));
    std::string targetting;

    if (game->ballsAssignedToPlayers) {
        
        if (game->player1Turn) {
            targetting = game->player1->getTargetSolids() ? "      Targeting - Solids" : "      Targeting - Stripes";
        } else {
            targetting = game->player2->getTargetSolids() ? "      Targeting - Solids" : "      Targeting - Stripes";
        }        
    }
    else{
        targetting = "      Targeting - All";
    }

    this->screens["GameScreen"]->getChild("TargetingColor")->setText(targetting);    
}

void GUIManager::playerWon(Player* winning) {
    using namespace CEGUI;
    
    Window* youWin = this->screens["GameScreen"]->getChild("YouWin");
    
    if (winning->getTargetSolids()) {
        // Solids win
        youWin->setText("Solid Player Wins!");
        youWin->show();
    } else {
        // Stripes win
        youWin->setText("Striped Player Wins!");
        youWin->show();
    }
}