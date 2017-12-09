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
    static const std::string lookNFeelClass;
    
    ThreeDPool* game;
    CEGUI::OgreRenderer* mRenderer;
    std::map<std::string, CEGUI::Window*> screens;
    
    CEGUI::WindowManager* wmgr;
    CEGUI::GUIContext* context;
    CEGUI::Window* sheet;
    
    static const float PROGRESS_MAX_ALPHA;
    float progressAlpha;

    void hideAllScreens(void);
    void createMPLobby(void);
    void showEnterIPWindow(void);
    void hideEnterIPWindow(void);
    void onIPEnterBoxKeyPressed(const CEGUI::EventArgs& e);
    
    void makeBackground(const std::string& filename);
    
    void makeScreen(const std::string& name);
    
    CEGUI::Window* makeWindow(const std::string& parentScreen,
        const std::string& type, const std::string& name,
        float xRelSize, float yRelSize, 
        float xRelPos, float yRelPos,
        bool useLookNFeel = true);
    
public:
    static const std::string MAIN_MENU,
                             MP_LOBBY,
                             BACKGROUND,
                             GAME_SCREEN;
    
    GUIManager(ThreeDPool* _game);
    GUIManager(const GUIManager& orig);
    virtual ~GUIManager();
    
    void createMainMenu(void);
    void setUpGUI(void);
    
    void setPowerBar(float progress);
    void fadeInPowerBar(void);
    void fadeOutPowerBar(void);
private:

};

#endif /* GUIMANAGER_H */

