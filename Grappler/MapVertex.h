//
//  MapVertex.h
//  Climber
//
//  Created by Ethan Coeytaux on 6/28/15.
//  Copyright (c) 2015 Science Class Games. All rights reserved.
//

#ifndef __Climber__MapVertex__
#define __Climber__MapVertex__

#include "MapElement.h"

#include "Coordinate.h"
#include "Circle.h"
#include "SortedCollection.h"

class Camera;
class Map;
class VertexNode;
class MapEdge;

class MapVertex : public MapElement {
    friend class Map;
    friend class MapEdge;
    
public:
    MapVertex(const Map * parent, Coordinate coor, bool grabbable = false);
    virtual ~MapVertex();
    
    Coordinate getCoordinate() const;
    bool isGrabbable() const;
    const MapEdge * getEdgeCW() const;
    const MapEdge * getEdgeCCW() const;
    Circle getBumper() const;
    
    virtual double getNormalForceAngle() const;
    
    bool adjustVelocity(Vector * vel) const;
    
    void draw(Camera * camera, bool drawBumper = false) const;
    
    std::string toString() const;
    static std::string toString(const MapVertex * vertex);
    
private:
    Coordinate coor;
    Circle bumper;
    MapEdge * edgeCW = nullptr, * edgeCCW = nullptr;
    
    SortedCollection<MapVertex>::Node * node = nullptr;
    
    void setEdgeCW(MapEdge * edge);
    void setEdgeCCW(MapEdge * edge);
    
    void setSortedCollectionNode(SortedCollection<MapVertex>::Node * node);
};

#endif /* defined(__Climber__MapVertex__) */
