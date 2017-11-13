/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Component.h
 * Author: treyr3
 *
 * Created on October 24, 2017, 5:55 PM
 */

#ifndef COMPONENT_H
#define COMPONENT_H

class GameObject;

class Component {
protected:
    Component(GameObject* _owner);
    
    GameObject* owner;
};

#endif /* COMPONENT_H */

