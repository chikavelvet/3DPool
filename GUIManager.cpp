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

GUIManager::GUIManager(ThreeDPool* _game) : game(_game)
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
}

GUIManager::~GUIManager() {
}

void GUIManager::hideAllScreens()
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

void GUIManager::createMainMenu()
{
    CEGUI::WindowManager& wmgr = CEGUI::WindowManager::getSingleton();
    CEGUI::GUIContext& context = CEGUI::System::getSingleton().getDefaultGUIContext();
    CEGUI::Window* sheet = context.getRootWindow();
    
    if (!this->screens[MAIN_MENU]) {
        hideAllScreens();
        
        //----Main Menu Screen----//
        CEGUI::Window* mainMenu = wmgr.createWindow("DefaultWindow", MAIN_MENU);
        mainMenu->setSize(CEGUI::USize(CEGUI::UDim(1, 0), CEGUI::UDim(1, 0)));
        mainMenu->setPosition(CEGUI::UVector2(CEGUI::UDim(0, 0), CEGUI::UDim(0, 0)));
        mainMenu->setAlwaysOnTop(true);
        
        sheet->addChild(mainMenu);
        
        this->screens[MAIN_MENU] = mainMenu;
        
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
        quit->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&ThreeDPool::quit, game));

        mainMenu->addChild(quit);

        //----Single Player----//
        CEGUI::Window *singlePlayer = wmgr.createWindow("TaharezLook/Button", "StartSinglePlayerButton");
        singlePlayer->setText("Single Player");    

        // In UDim, only set one of the two params, the other should be 0
        singlePlayer->setSize(CEGUI::USize(CEGUI::UDim(0.15, 0), CEGUI::UDim(0.05, 0)));
        singlePlayer->setPosition(CEGUI::UVector2(CEGUI::UDim(0.425, 0), CEGUI::UDim(0.74, 0)));
        singlePlayer->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&ThreeDPool::createScene, game));

        mainMenu->addChild(singlePlayer);

        //----Multi Player----//
        CEGUI::Window *multiPlayer = wmgr.createWindow("TaharezLook/Button", "StartMultiPlayerButton");
        multiPlayer->setText("Multi Player");    

        // In UDim, only set one of the two params, the other should be 0
        multiPlayer->setSize(CEGUI::USize(CEGUI::UDim(0.15, 0), CEGUI::UDim(0.05, 0)));
        multiPlayer->setPosition(CEGUI::UVector2(CEGUI::UDim(0.425, 0), CEGUI::UDim(0.80, 0)));
        multiPlayer->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&ThreeDPool::createMPLobby, game));

        mainMenu->addChild(multiPlayer);
    } else {
        hideAllScreens();
        sheet->getChild(MAIN_MENU)->show();
        sheet->getChild("DefaultBackground")->show();
    }
}