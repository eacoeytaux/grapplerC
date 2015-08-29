//
//  Map.h
//  Climber
//
//  Created by Ethan Coeytaux on 6/28/15.
//  Copyright (c) 2015 Science Class Games. All rights reserved.
//

#ifndef __Climber__Map__
#define __Climber__Map__

#include "Coordinate.h"
#include "Vector.h"
#include "SortedCollection.h"

class Camera;
class Hitbox;
class MapVertex;
class MapEdge;

class Map {
    friend class AbsLevel;
    
public:
    Map();
    virtual ~Map();
    
    void loadElements();
    
    void update(Coordinate mouseCoor); //TODO won't need mouseCoor for future use
    
    void drawElements(Camera * camera); //for debugging only
    void drawBackground(Camera * camera);
    void drawForeground(Camera * camera);
    
    static void flipShowBumpers();
    
private:
    static bool showBumpers;
    
    SortedCollection<MapVertex> vertices;
    SortedCollection<MapEdge> edges;
    int largestEdgeDx = 0;
    
    MapVertex * createVertex(int x, int y, bool grabbable = false);
    MapEdge * createEdge(MapVertex * rightVertex, MapVertex * leftVertex, bool floor = false, bool facingUp = true, int strength = 0);
};

#endif /* defined(__Climber__Map__) */
