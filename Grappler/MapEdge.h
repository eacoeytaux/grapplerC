//
//  MapEdge.h
//  Climber
//
//  Created by Ethan Coeytaux on 7/3/15.
//  Copyright (c) 2015 Science Class Games. All rights reserved.
//

#ifndef __Climber__MapEdge__
#define __Climber__MapEdge__

#include "MapElement.h"

#include "Line.h"
#include "SortedCollection.h"

class Map;
class MapVertex;

class MapEdge : public MapElement {
    friend class Map;
    friend class MapVertex;
    
public:
    MapEdge(const Map * parent, MapVertex * vertexCW, MapVertex * vertexCCW, bool floor = false, bool facingUp = true, int strength = 0);
    virtual ~MapEdge();
    
    Line getLine() const;
    bool isFloor() const;
    int getStrength() const;
    const MapVertex * getVertex(MapVertex * other) const;
    const MapVertex * getVertexCW() const;
    const MapVertex * getVertexCCW() const;
    Line getBumper() const;
    Line getCatchCW() const;
    Line getCatchCCW() const;
    double getNormalForceAngle() const;
    
    bool adjustVelocity(Vector * vel) const;
    
    void draw(Camera * camera, bool drawBumper = false) const;
    
    std::string toString() const;
    static std::string toString(const MapEdge * edge);
    
private:
    static const int catchSize = 4;
    
    Line line, bumper, catchCW, catchCCW;
    bool floor, facingUp; //for vertical edges, facingUp means facing right
    int strength;
    MapVertex * vertexCW = nullptr, * vertexCCW = nullptr;
    
    SortedCollection<MapEdge>::Node * node = nullptr;
    
    void setSortedCollectionNode(SortedCollection<MapEdge>::Node * node);
};

#endif /* defined(__Climber__MapEdge__) */
