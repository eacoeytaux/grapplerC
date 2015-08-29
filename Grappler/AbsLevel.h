//
//  AbsLevel.h
//  Climber
//
//  Created by Ethan Coeytaux on 6/28/15.
//  Copyright (c) 2015 Science Class Games. All rights reserved.
//

#ifndef __Climber__AbsLevel__
#define __Climber__AbsLevel__

#include "LinkedList.h"
#include "SortedCollection.h"
#include "Map.h"
#include "Player.h"

class SDL_Renderer;

class MapObject;

class AbsLevel {
public:
    AbsLevel(SDL_Renderer * renderer);
    virtual ~AbsLevel();
    
    void update(Coordinate mouseCoor, float dt); //TODO remove dt?
    void draw(Camera * camera);
    
    void generatePath(Player * player);
    
    Player * getPlayer();
    
private:
    Map map;
    Player player;
    
    void loadResources();
    void loadImages();
    void loadSounds();
    
    template<typename T>
    static void cleanUpCollection(LinkedList<MapObject*> * objectsToDelete, SortedCollection<T> * collection) {
        for (LinkedList<MapObject*>::Node * traversalNode = objectsToDelete->getHeadNode(); traversalNode != nullptr; traversalNode = traversalNode->next) {
            collection->removeElement((T*)traversalNode->element);
        }
        objectsToDelete->clear();
    }
    
    //for debugging
    double lowerBound = 0;
    double upperBound = 0;
};

#endif /* defined(__Climber__AbsLevel__) */
