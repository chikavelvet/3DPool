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

#include <CEGUI/CEGUI.h>
#include <CEGUI/RendererModules/Ogre/Renderer.h>

class GUIManager {
protected:
    CEGUI::OgreRenderer* mRenderer;
public:
    GUIManager();
    GUIManager(const GUIManager& orig);
    virtual ~GUIManager();
private:

};

#endif /* GUIMANAGER_H */

