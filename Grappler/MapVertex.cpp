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

/*void MapVertex::calculateBumper() {
    if ((edgeCW == nullptr) || (edgeCCW == nullptr)) {
        bumper = Arc(circle, 0, (M_PI_2));
    } else {
        double startAngle = edgeCW->getNormalForceAngle();
        double endAngle = edgeCCW->getNormalForceAngle();
        bumper = Arc(circle, endAngle, constants::findAngleDifference(endAngle, startAngle));
    }
}*/

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
    return edgeCCW;
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
    double angleToOrigin = constants::findAngle(coor, vel->getOrigin());
    
    if (vel->subtractAngle(constants::findAngle(coor, vel->getOrigin()))) { //(constants::findAngleDifference(vel->getAngle(), constants::findAngle(coor, vel->getOrigin())) >= M_PI_2)
        Arc arc(bumper, angleToOrigin, bumper.convertToRadians(vel->getMagnitude()) * ((constants::findAngleDifference(angleToOrigin, constants::findAngle(coor, vel->getDestination())) < 0) ? -1 : 1));
        vel->setDestination(arc.getEnd());
        
        return true;
    } else {
        return false;
    }
}

void MapVertex::draw(Camera * camera, bool drawBumper) const {
    camera->drawRectangle(coor.getX(), coor.getY(), 3, 3, constants::yellow);
    if (drawBumper)
        camera->drawCircle(bumper, (touchingPlayer ? constants::white : constants::cyan));
}

std::string MapVertex::toString() const {
    return "MapVertex(" + coor.toString() + ")";
}

std::string MapVertex::toString(const MapVertex * vertex) {
    return vertex->toString();
}