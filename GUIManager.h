/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   GUIManager.h
 * Author: treyr3
 *
 * Created on November 19, 2017, 1:37 PM
 */

#ifndef GUIMANAGER_H
#define GUIMANAGER_H

#include <string>
#include <map>

const std::string MAIN_MENU = "MainMenuScreen";
const std::string MP_LOBBY = "MPLobbyScreen";
const std::string BACKGROUND = "DefaultBackground";
    
class ThreeDPool;

namespace CEGUI {
    class OgreRenderer;
    class WindowManager;
    class GUIContext;
    class Window;
    class EventArgs;
}

class GUIManager {
protected:
    ThreeDPool* game;
    CEGUI::OgreRenderer* mRenderer;
    std::map<std::string, CEGUI::Window*> screens;
    
    CEGUI::WindowManager* wmgr;
    CEGUI::GUIContext* context;
    CEGUI::Window* sheet;
    
    void hideAllScreens(void);
    void createMPLobby(void);
    void showEnterIPWindow(void);
    void hideEnterIPWindow(void);
    void onIPEnterBoxKeyPressed(const CEGUI::EventArgs& e);
public:
    GUIManager(ThreeDPool* _game);
    GUIManager(const GUIManager& orig);
    virtual ~GUIManager();
    
    void createMainMenu(void);
private:

};

#endif /* GUIMANAGER_H */

