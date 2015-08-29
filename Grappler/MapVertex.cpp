//
//  MapVertex.cpp
//  Climber
//
//  Created by Ethan Coeytaux on 6/28/15.
//  Copyright (c) 2015 Science Class Games. All rights reserved.
//

#include "MapVertex.h"

#include "Constants.h"
#include "Camera.h"
#include "MapEdge.h"
#include "Player.h"

MapVertex::MapVertex(const Map * parent, Coordinate coor, bool grabbable) : MapElement(parent), coor(coor), bumper(coor, Player::trueRadius) { }

MapVertex::~MapVertex() {
    if (edgeCW != nullptr)
        delete edgeCW;
    if (edgeCCW != nullptr)
        delete edgeCCW;
    node = nullptr;
}

Coordinate MapVertex::getCoordinate() const {
    return coor;
}

const MapEdge * MapVertex::getEdgeCW() const {
    return edgeCW;
}

void MapVertex::setEdgeCW(MapEdge * edge) {
    edgeCW = edge;
}

const MapEdge * MapVertex::getEdgeCCW() const {
    return edgeCW;
}

void MapVertex::setEdgeCCW(MapEdge * edge) {
    edgeCCW = edge;
}

Circle MapVertex::getBumper() const {
    return bumper;
}

void MapVertex::setSortedCollectionNode(SortedCollection<MapVertex>::Node * node) {
    this->node = node;
}

double MapVertex::getNormalForceAngle() const {
    return 0;
}

bool MapVertex::adjustVelocity(Vector * vel) const {
    return true;
}

void MapVertex::draw(Camera * camera, bool drawBumper) const {
    camera->drawRectangle(coor.getX(), coor.getY(), 5, 5, constants::yellow);
    if (drawBumper)
        camera->drawCircle(bumper, (touchingPlayer ? constants::white : constants::cyan));
}

std::string MapVertex::toString() const {
    return "MapVertex(" + coor.toString() + ")";
}

std::string MapVertex::toString(const MapVertex * vertex) {
    return vertex->toString();
}