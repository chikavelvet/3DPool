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
    
    CEGUI::SchemeManager::getSingleton().createFromFile("TaharezLook.scheme");
    context->getMouseCursor().setDefaultImage("TaharezLook/MouseArrow");

    wmgr = &CEGUI::WindowManager::getSingleton();
    sheet = wmgr->createWindow("DefaultWindow", "ThreeDPool/Sheet");
    context->setRootWindow(sheet);
}

GUIManager::~GUIManager() {
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
        
        //----Main Menu Screen----//
        CEGUI::Window* mainMenu = wmgr->createWindow("DefaultWindow", MAIN_MENU);
        mainMenu->setSize(CEGUI::USize(CEGUI::UDim(1, 0), CEGUI::UDim(1, 0)));
        mainMenu->setPosition(CEGUI::UVector2(CEGUI::UDim(0, 0), CEGUI::UDim(0, 0)));
        mainMenu->setAlwaysOnTop(true);
        
        sheet->addChild(mainMenu);
        
        this->screens[MAIN_MENU] = mainMenu;
        
        //----Back Ground----//   
        CEGUI::ImageManager::getSingleton().addFromImageFile("BackgroundImage", "ThreeDPoolBackground.png", "Imagesets");
        CEGUI::Window* background = wmgr->createWindow("TaharezLook/StaticImage", "DefaultBackground");
        background->setProperty("Image", "BackgroundImage");
        background->setSize(CEGUI::USize(CEGUI::UDim(1, 0), CEGUI::UDim(1, 0)));
        background->setPosition(CEGUI::UVector2(CEGUI::UDim(0, 0), CEGUI::UDim(0, 0)));
        background->setAlwaysOnTop(false);
        sheet->addChild(background);
        
        //----Quit Button----//
        CEGUI::Window *quit = wmgr->createWindow("TaharezLook/Button", "QuitButton");
        quit->setText("Quit");    

        // In UDim, only set one of the two params, the other should be 0
        quit->setSize(CEGUI::USize(CEGUI::UDim(0.15, 0), CEGUI::UDim(0.05, 0)));
        quit->setPosition(CEGUI::UVector2(CEGUI::UDim(0.425, 0), CEGUI::UDim(0.86, 0)));
        quit->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&ThreeDPool::quit, game));

        mainMenu->addChild(quit);

        //----Single Player----//
        CEGUI::Window *singlePlayer = wmgr->createWindow("TaharezLook/Button", "StartSinglePlayerButton");
        singlePlayer->setText("Single Player");    

        // In UDim, only set one of the two params, the other should be 0
        singlePlayer->setSize(CEGUI::USize(CEGUI::UDim(0.15, 0), CEGUI::UDim(0.05, 0)));
        singlePlayer->setPosition(CEGUI::UVector2(CEGUI::UDim(0.425, 0), CEGUI::UDim(0.74, 0)));
        singlePlayer->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&ThreeDPool::createScene, game));

        mainMenu->addChild(singlePlayer);

        //----Multi Player----//
        CEGUI::Window *multiPlayer = wmgr->createWindow("TaharezLook/Button", "StartMultiPlayerButton");
        multiPlayer->setText("Multi Player");    

        // In UDim, only set one of the two params, the other should be 0
        multiPlayer->setSize(CEGUI::USize(CEGUI::UDim(0.15, 0), CEGUI::UDim(0.05, 0)));
        multiPlayer->setPosition(CEGUI::UVector2(CEGUI::UDim(0.425, 0), CEGUI::UDim(0.80, 0)));
        multiPlayer->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&GUIManager::createMPLobby, this));

        mainMenu->addChild(multiPlayer);
    } else {
        hideAllScreens();
        sheet->getChild(MAIN_MENU)->show();
        sheet->getChild("DefaultBackground")->show();
    }
}

void GUIManager::createMPLobby() 
{
    if (!this->screens[MP_LOBBY]) {
        hideAllScreens();
        
        // Create Lobby                
        CEGUI::Window* mpLobby = wmgr->createWindow("DefaultWindow", MP_LOBBY);
        mpLobby->setSize(CEGUI::USize(CEGUI::UDim(1, 0), CEGUI::UDim(1, 0)));
        mpLobby->setPosition(CEGUI::UVector2(CEGUI::UDim(0, 0), CEGUI::UDim(0, 0)));
        
        sheet->addChild(mpLobby);
        
        this->screens[MP_LOBBY] = mpLobby;
        
        //----Back to Main----//
        CEGUI::Window *back = wmgr->createWindow("TaharezLook/Button", "BackToMainButton");
        back->setText("Back");
        back->setSize(CEGUI::USize(CEGUI::UDim(0.15, 0), CEGUI::UDim(0.05, 0)));
        back->setPosition(CEGUI::UVector2(CEGUI::UDim(0, 0), CEGUI::UDim(0, 0)));
        back->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&GUIManager::createMainMenu, this));
        mpLobby->addChild(back);
        
        //----Host Game-----------------//
        CEGUI::Window *hostGame = wmgr->createWindow("TaharezLook/Button", "HostGameButton");
        hostGame->setText("Host Game");
        hostGame->setSize(CEGUI::USize(CEGUI::UDim(0.15, 0), CEGUI::UDim(0.05, 0)));
        hostGame->setPosition(CEGUI::UVector2(CEGUI::UDim(0.3, 0), CEGUI::UDim(0.8, 0)));
        hostGame->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&ThreeDPool::startWaiting, game));
        mpLobby->addChild(hostGame);
        
        //----Join Game-----------------//
        CEGUI::Window *joinGame = wmgr->createWindow("TaharezLook/Button", "JoinGameButton");
        joinGame->setText("Join Game");
        joinGame->setSize(CEGUI::USize(CEGUI::UDim(0.15, 0), CEGUI::UDim(0.05, 0)));
        joinGame->setPosition(CEGUI::UVector2(CEGUI::UDim(0.55, 0), CEGUI::UDim(0.8, 0)));
        joinGame->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&GUIManager::showEnterIPWindow, this));
        mpLobby->addChild(joinGame);
        
        //--------Enter Server IP-------//
        CEGUI::FrameWindow *enterIP = static_cast<CEGUI::FrameWindow*>(wmgr->createWindow("TaharezLook/FrameWindow", "EnterIPWindow"));
        enterIP->setText("Enter Server IP");
        enterIP->setSize(CEGUI::USize(CEGUI::UDim(0.6, 0), CEGUI::UDim(0.4, 0)));
        enterIP->setPosition(CEGUI::UVector2(CEGUI::UDim(0.2, 0), CEGUI::UDim(0.3, 0)));
        enterIP->setAlwaysOnTop(true);
        enterIP->subscribeEvent(CEGUI::FrameWindow::EventCloseClicked, CEGUI::Event::Subscriber(&GUIManager::hideEnterIPWindow, this));
        enterIP->hide();
        mpLobby->addChild(enterIP);
                
        //------------IP Enter Box------//
        CEGUI::Editbox *ipEnterBox = static_cast<CEGUI::Editbox*>(wmgr->createWindow("TaharezLook/Editbox", "IPEnterBox"));
        ipEnterBox->setSize(CEGUI::USize(CEGUI::UDim(0.5, 0), CEGUI::UDim(0.2, 0)));
        ipEnterBox->setPosition(CEGUI::UVector2(CEGUI::UDim(0.25, 0), CEGUI::UDim(0.4, 0)));
        ipEnterBox->subscribeEvent(CEGUI::Editbox::EventKeyDown, CEGUI::Event::Subscriber(&GUIManager::onIPEnterBoxKeyPressed, this));
        enterIP->addChild(ipEnterBox);
        
        //------------Go Button---------//
        CEGUI::Window *goButton = wmgr->createWindow("TaharezLook/Button", "JoinGameButton");
        goButton->setText("Go");
        goButton->setSize(CEGUI::USize(CEGUI::UDim(0.1, 0), CEGUI::UDim(0.1, 0)));
        goButton->setPosition(CEGUI::UVector2(CEGUI::UDim(0.45, 0), CEGUI::UDim(0.7, 0)));
        goButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&ThreeDPool::joinMultiplayer, game));
        enterIP->addChild(goButton);
        
        //--------Waiting for Client----//
        CEGUI::FrameWindow *waiting = static_cast<CEGUI::FrameWindow*>(wmgr->createWindow("TaharezLook/FrameWindow", "WaitingWindow"));
        waiting->setText("Waiting for Client...");
        waiting->setSize(CEGUI::USize(CEGUI::UDim(0.6, 0), CEGUI::UDim(0.4, 0)));
        waiting->setPosition(CEGUI::UVector2(CEGUI::UDim(0.2, 0), CEGUI::UDim(0.3, 0)));
        waiting->setAlwaysOnTop(true);
        waiting->subscribeEvent(CEGUI::FrameWindow::EventCloseClicked, CEGUI::Event::Subscriber(&ThreeDPool::cancelWaiting, game));
        waiting->hide();
        mpLobby->addChild(waiting);
        
        //------------Information-------//
        CEGUI::Window *serverInfo = wmgr->createWindow("TaharezLook/StaticText", "ServerInfo");
        serverInfo->setSize(CEGUI::USize(CEGUI::UDim(0.4, 0), CEGUI::UDim(0.15, 0)));
        serverInfo->setPosition(CEGUI::UVector2(CEGUI::UDim(0.3, 0), CEGUI::UDim(0.425, 0)));
        waiting->addChild(serverInfo);
        
        sheet->getChild("DefaultBackground")->show();
    } else {
        hideAllScreens();
        sheet->getChild(MP_LOBBY)->show();
        sheet->getChild("DefaultBackground")->show();
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