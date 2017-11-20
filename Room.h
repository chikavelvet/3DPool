#ifndef __Room_h_
#define __Room_h_

#include "GameObject.h"
#include <OgreMeshManager.h>
#include "Wall.h"
#include <vector>

class Room : public GameObject {
protected:
    static const float OFFSET;
    
    std::vector<Wall*> physicalWalls;
    std::vector<Wall*> visualWalls;
    
public:
    Room(Ogre::SceneManager* _sceneMgr, Simulator* _simulator);
};

#endif