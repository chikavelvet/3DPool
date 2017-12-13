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
    justCheckedBox(false), justCheckedBox2(false)
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
        quit->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&ThreeDPool::quit, game));
      
        //----Single Player----//
        CEGUI::Window* singlePlayer = makeWindow(MAIN_MENU, "Button", "StartSinglePlayerButton", 0.15, 0.05, 0.425, 0.74);
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
    this->screens[PLAYER_SELECT]->getChild("Player1SelectEasy")->hide();
    this->screens[PLAYER_SELECT]->getChild("Player1SelectMedium")->hide();
    this->screens[PLAYER_SELECT]->getChild("Player1SelectHard")->hide();
}

void GUIManager::player2SelectManual(){
    p2Type = 0;
    this->screens[PLAYER_SELECT]->getChild("Player2SelectEasy")->show();
    this->screens[PLAYER_SELECT]->getChild("Player2SelectMedium")->show();
    this->screens[PLAYER_SELECT]->getChild("Player2SelectHard")->show();
}

void GUIManager::player1SelectAI(){
    p1Type = 1;
    this->screens[PLAYER_SELECT]->getChild("Player1SelectEasy")->show();
    this->screens[PLAYER_SELECT]->getChild("Player1SelectMedium")->show();
    this->screens[PLAYER_SELECT]->getChild("Player1SelectHard")->show();
}

void GUIManager::player2SelectAI(){
    p2Type = 1;
    this->screens[PLAYER_SELECT]->getChild("Player2SelectEasy")->show();
    this->screens[PLAYER_SELECT]->getChild("Player2SelectMedium")->show();
    this->screens[PLAYER_SELECT]->getChild("Player2SelectHard")->show();
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
     if (!this->screens[PLAYER_SELECT]) {
        hideAllScreens();

        this->screens[BACKGROUND]->show();

        makeScreen(PLAYER_SELECT);

        CEGUI::Window *back = makeWindow(PLAYER_SELECT, "Button", "BackToMainButton2", 0.15, 0.05, 0.05, 0.9);
        back->setText("Back to Main Menu");
        back->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&ThreeDPool::createScene, game));        


        //Player 1 (LEFT) Side
        CEGUI::Window *p1Title = makeWindow(PLAYER_SELECT, "StaticText", "Player1Title", 0.15, 0.05, 0.25, 0.1);
        p1Title->setProperty("BackgroundEnabled", "False");
        p1Title->setProperty("FrameEnabled", "False");
        p1Title->setText("Player 1");
        
        CEGUI::Window *p1SelectManual = makeWindow(PLAYER_SELECT, "Button", "Player1SelectManual", 0.15, 0.05, 0.25, 0.3);
        p1SelectManual->setText("Human");
        p1SelectManual->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&GUIManager::player1SelectManual, this));        

        CEGUI::Window *p1SelectAI = makeWindow(PLAYER_SELECT, "Button", "Player1SelectAI", 0.15, 0.05, 0.25, 0.4);
        p1SelectAI->setText("Computer");
        p1SelectAI->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&GUIManager::player1SelectAI, this));        

        CEGUI::Window *p1SelectEasy = makeWindow(PLAYER_SELECT, "Checkbox", "Player1SelectEasy", 0.1, 0.05, 0.3, 0.5);
        p1SelectEasy->setProperty("Text", "Easy");
        p1SelectEasy->setProperty("Selected", "False");
        p1SelectEasy->subscribeEvent(CEGUI::ToggleButton::EventSelectStateChanged, CEGUI::Event::Subscriber(&GUIManager::player1SelectEasy, this));
        p1SelectEasy->hide();

        CEGUI::Window *p1SelectMedium = makeWindow(PLAYER_SELECT, "Checkbox", "Player1SelectMedium", 0.1, 0.05, 0.3, 0.53);
        p1SelectMedium->setProperty("Text", "Normal");
        p1SelectMedium->setProperty("Selected", "True");
        p1SelectMedium->subscribeEvent(CEGUI::ToggleButton::EventSelectStateChanged, CEGUI::Event::Subscriber(&GUIManager::player1SelectMedium, this));
        p1SelectMedium->hide();

        CEGUI::Window *p1SelectHard = makeWindow(PLAYER_SELECT, "Checkbox", "Player1SelectHard", 0.1, 0.05, 0.3, 0.56);
        p1SelectHard->setProperty("Text", "Hard");
        p1SelectHard->setProperty("Selected", "False");
        p1SelectHard->subscribeEvent(CEGUI::ToggleButton::EventSelectStateChanged, CEGUI::Event::Subscriber(&GUIManager::player1SelectHard, this));
        p1SelectHard->hide();

        //Player 2 (RIGHT) Side
        CEGUI::Window *p2Title = makeWindow(PLAYER_SELECT, "StaticText", "Player2Title", 0.15, 0.05, 0.62, 0.1);
        p2Title->setProperty("BackgroundEnabled", "False");
        p2Title->setProperty("FrameEnabled", "False");
        p2Title->setText("Player 2");

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

        //----Quit Button----//
        CEGUI::Window *quit = makeWindow(GAME_SCREEN, "Button", "QuitButton", 0.15, 0.05, 0.1, 0.1);
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
        strokesWin->setSize(CEGUI::USize(CEGUI::UDim(0.15, 0), CEGUI::UDim(0.05, 0)));
        strokesWin->setPosition(CEGUI::UVector2(CEGUI::UDim(0.80, 0), CEGUI::UDim(0.84, 0)));
        strokesWin->setProperty("FrameEnabled", "True");
        // strokesWin->setProperty("HorizontalAlignment", "Centre");
        strokesWin->setProperty("TextColours", "tl:FF00FF00 tr:FF00FF00 bl:FF00FF00 br:FF00FF00");
        // strokesWin->setProperty("BackgroundEnabled", "True");
        strokesWin->hide();

        // Red Remaining Ball Counter
        CEGUI::Window *redBallsRemainingWin = makeWindow(GAME_SCREEN, "StaticText", "SolidBallsRemaining", 0.15, 0.05, 0.8, 0.79);
        std::stringstream ss3;
        ss3 << "Solid: " << game->solidBallsRemaining;
        redBallsRemainingWin->setText(ss3.str());
        redBallsRemainingWin->setSize(CEGUI::USize(CEGUI::UDim(0.15, 0), CEGUI::UDim(0.05, 0)));
        redBallsRemainingWin->setPosition(CEGUI::UVector2(CEGUI::UDim(0.80, 0), CEGUI::UDim(0.79, 0)));
        redBallsRemainingWin->setProperty("FrameEnabled", "True");
        redBallsRemainingWin->setProperty("TextColours", "tl:FF00FF00 tr:FF00FF00 bl:FF00FF00 br:FF00FF00");
        // redBallsRemainingWin->setProperty("BackgroundEnabled", "True");
        // redBallsRemainingWin->setProperty("BackgroundColours", "tl:FFFFFF tr:FFFFFF bl:FFFFFF br:FFFFFF");
        redBallsRemainingWin->setProperty("FrameColours", "tl:FF00FF00 tr:FF00FF00 bl:FF00FF00 br:FF00FF00");
        
        // Blue Remaining Ball Counter
        CEGUI::Window *blueBallsRemainingWin = makeWindow(GAME_SCREEN, "StaticText", "StripedBallsRemaining", 0.15, 0.05, 0.8, 0.7);
        std::stringstream ss4;
        ss4 << "Striped: " << game->stripedBallsRemaining;
        blueBallsRemainingWin->setText(ss4.str());
        blueBallsRemainingWin->setSize(CEGUI::USize(CEGUI::UDim(0.15, 0), CEGUI::UDim(0.05, 0)));
        blueBallsRemainingWin->setPosition(CEGUI::UVector2(CEGUI::UDim(0.80, 0), CEGUI::UDim(0.70, 0)));
        blueBallsRemainingWin->setProperty("FrameEnabled", "True");
        // blueBallsRemainingWin->setProperty("HorizontalAlignment", "Centre");
        blueBallsRemainingWin->setProperty("TextColours", "tl:FF00FF00 tr:FF00FF00 bl:FF00FF00 br:FF00FF00");
        blueBallsRemainingWin->setProperty("FrameColours", "tl:FF00FF00 tr:FF00FF00 bl:FF00FF00 br:FF00FF00");
        // blueBallsRemainingWin->setProperty("BackgroundEnabled", "True");
        // blueBallsRemainingWin->setProperty("BackgroundColours", "tl:FFFFFF tr:FFFFFF bl:FFFFFF br:FFFFFF");
        
        // Opponent Remaining Ball Counter
        CEGUI::Window *targettingColorWin = makeWindow(GAME_SCREEN, "StaticText", "TargettingColor", 0.15, 0.05, 0.8, 0.55);
        targettingColorWin->setText("Targetting: All");
        targettingColorWin->setSize(CEGUI::USize(CEGUI::UDim(0.15, 0), CEGUI::UDim(0.05, 0)));
        targettingColorWin->setPosition(CEGUI::UVector2(CEGUI::UDim(0.80, 0), CEGUI::UDim(0.55, 0)));
        targettingColorWin->setProperty("FrameEnabled", "True");
        // targettingColorWin->setProperty("HorizontalAlignment", "Centre");
        targettingColorWin->setProperty("TextColours", "tl:FF00FF00 tr:FF00FF00 bl:FF00FF00 br:FF00FF00");
        targettingColorWin->setProperty("FrameColours", "tl:FF00FF00 tr:FF00FF00 bl:FF00FF00 br:FF00FF00");
        // targettingColorWin->setProperty("BackgroundEnabled", "True");
        // targettingColorWin->setProperty("BackgroundColours", "tl:FFFFFF tr:FFFFFF bl:FFFFFF br:FFFFFF");
        
        // Opponent Title
        CEGUI::Window *activePlayer = makeWindow(GAME_SCREEN, "StaticText", "ActivePlayer", 0.15, 0.05, 0.8, 0.5);
        activePlayer->setText("Player 1's Turn");
        activePlayer->setSize(CEGUI::USize(CEGUI::UDim(0.15, 0), CEGUI::UDim(0.05, 0)));
        activePlayer->setPosition(CEGUI::UVector2(CEGUI::UDim(0.80, 0), CEGUI::UDim(0.5, 0)));
        activePlayer->setProperty("FrameEnabled", "True");
        // activePlayer->setProperty("HorizontalAlignment", "Centre");
        activePlayer->setProperty("TextColours", "tl:FF00FF00 tr:FF00FF00 bl:FF00FF00 br:FF00FF00");
        activePlayer->setProperty("FrameColours", "tl:FF00FF00 tr:FF00FF00 bl:FF00FF00 br:FF00FF00");
        // activePlayer->setProperty("BackgroundEnabled", "True");
        // activePlayer->setProperty("BackgroundColours", "tl:FFFFFF tr:FFFFFF bl:FFFFFF br:FFFFFF");

        // Opponent Stroke counter
        CEGUI::Window *oppStrokesWin = makeWindow(GAME_SCREEN, "StaticText", "OppStrokeCount", 0.15, 0.05, 0.8, 0.6);
        std::stringstream ss2;
        ss2 << "Strokes: " << game->opponentStrokes;
        oppStrokesWin->setText(ss2.str());
        oppStrokesWin->hide();
        oppStrokesWin->setProperty("FrameEnabled", "True");
        // oppStrokesWin->setProperty("HorizontalAlignment", "Centre");
        oppStrokesWin->setProperty("TextColours", "tl:FF00FF00 tr:FF00FF00 bl:FF00FF00 br:FF00FF00");
        oppStrokesWin->setProperty("FrameColours", "tl:FF00FF00 tr:FF00FF00 bl:FF00FF00 br:FF00FF00");
        // oppStrokesWin->setProperty("BackgroundEnabled", "True");
        // oppStrokesWin->setProperty("BackgroundColours", "tl:FFFFFF tr:FFFFFF bl:FFFFFF br:FFFFFF");


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
        
        // spawnBallImages();

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
    std::stringstream ss;
    
    if (redBall) {
        ss << "Solid: " << game->solidBallsRemaining;
        this->screens["GameScreen"]->getChild("SolidBallsRemaining")->setText(ss.str());
    } else {
        ss << "Striped: " << game->stripedBallsRemaining;
        this->screens["GameScreen"]->getChild("StripedBallsRemaining")->setText(ss.str());
    }
}

void GUIManager::endCurrentTurn() {
    if (game->ballsAssignedToPlayers) {
        std::string targetting;
        
        if (game->player1Turn) {
            targetting = game->player1->getTargetSolids() ? "Targetting: Solid" : "Targetting: Striped";
        } else {
            targetting = game->player2->getTargetSolids() ? "Targetting: Solid" : "Targetting: Striped";
        }
        
        this->screens["GameScreen"]->getChild("TargettingColor")->setText(targetting);
    }
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