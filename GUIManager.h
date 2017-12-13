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

#include <iostream>     // std::cout
#include <sstream>      // std::stringstream, std::stringbuf

class ThreeDPool;

namespace CEGUI {
    class OgreRenderer;
    class WindowManager;
    class GUIContext;
    class Window;
    class EventArgs;
}

class Player;

class GUIManager {
protected:
    static const std::string lookNFeelClass;
    
    ThreeDPool* game;
    CEGUI::OgreRenderer* mRenderer;
    std::map<std::string, CEGUI::Window*> screens;
    
    CEGUI::WindowManager* wmgr;
    CEGUI::GUIContext* context;
    CEGUI::Window* sheet;
    
    int p1Type, p2Type, p1Diff, p2Diff;
    bool justCheckedBox;
    bool justCheckedBox2;



    static const float PROGRESS_MAX_ALPHA;
    float progressAlpha;

    void restartGame(void);
    void hideAllScreens(void);
    void createMPLobby(void);
    void showEnterIPWindow(void);
    void hideEnterIPWindow(void);
    void onIPEnterBoxKeyPressed(const CEGUI::EventArgs& e);
    void spawnBallImages();
    void addBallImage(std::string num, float xScale, float yScale, float xpos, float ypos);
    void makeBackground(const std::string& filename);    
    void makeScreen(const std::string& name);

    void player1SelectManual();
    void player2SelectManual();
    void player1SelectAI();
    void player2SelectAI();
    void player1SelectEasy();
    void player1SelectMedium();
    void player1SelectHard();
    void player2SelectEasy();
    void player2SelectMedium();
    void player2SelectHard();
    
    CEGUI::Window* makeWindow(const std::string& parentScreen,
        const std::string& type, const std::string& name,
        float xRelSize, float yRelSize, 
        float xRelPos, float yRelPos,
        bool useLookNFeel = true);
    
public:
    static const std::string MAIN_MENU,
                             PLAYER_SELECT,
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
    
    void createPlayersLobby();
    void decrementRemainingBallCount(bool redBall);
    void endCurrentTurn(void);
    void playerWon(Player* winning);

private:

};

#endif /* GUIMANAGER_H */

