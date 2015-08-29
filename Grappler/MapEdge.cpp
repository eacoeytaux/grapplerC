//
//  MapEdge.cpp
//  Climber
//
//  Created by Ethan Coeytaux on 7/3/15.
//  Copyright (c) 2015 Science Class Games. All rights reserved.
//

#include "MapEdge.h"

#include "Constants.h"
#include "MapVertex.h"
#include "Player.h"

MapEdge::MapEdge(const Map * parent, MapVertex * vertexCW, MapVertex * vertexCCW, bool floor, bool facingUp, int strength) : MapElement(parent), line(vertexCW->getCoordinate(), vertexCCW->getCoordinate()), bumper(line.getPerpendicularLine(facingUp ? Player::trueRadius : -Player::trueRadius)), vertexCW(vertexCW), catchCW(vertexCW->getCoordinate(), vertexCW->getCoordinate() + Vector(line.getPerpendicularDx((Player::trueRadius + catchSize) * (facingUp ? 1 : -1)), line.getPerpendicularDy((Player::trueRadius + catchSize) * (facingUp ? 1 : -1)))), catchCCW(vertexCCW->getCoordinate(), vertexCCW->getCoordinate() + Vector(line.getPerpendicularDx((Player::trueRadius + catchSize) * (facingUp ? 1 : -1)), line.getPerpendicularDy((Player::trueRadius + catchSize) * (facingUp ? 1 : -1)))), vertexCCW(vertexCCW), floor(floor), facingUp(facingUp), strength(strength) {
    vertexCW->setEdgeCCW(this);
    vertexCCW->setEdgeCW(this);
}

MapEdge::~MapEdge() {
    if (vertexCW != nullptr)
        vertexCW->setEdgeCCW(nullptr);
    if (vertexCCW != nullptr)
        vertexCCW->setEdgeCW(nullptr);
}

Line MapEdge::getLine() const {
    return line;
}

bool MapEdge::isFloor() const {
    return floor;
}

int MapEdge::getStrength() const {
    return strength;
}

const MapVertex * MapEdge::getVertex(MapVertex * other) const {
    if (other == vertexCW)
        return vertexCCW;
    else if (other == vertexCCW)
        return vertexCW;
    else
        return nullptr;
}

const MapVertex * MapEdge::getVertexCW() const {
    return vertexCW;
}

const MapVertex * MapEdge::getVertexCCW() const {
    return vertexCCW;
}

Line MapEdge::getBumper() const {
    return bumper;
}

Line MapEdge::getCatchCW() const {
    return catchCW;
}

Line MapEdge::getCatchCCW() const {
    return catchCCW;
}

double MapEdge::getNormalForceAngle() const { //TODO this should be able to be improved
    double angle = line.getAngle() - M_PI_2;
    
    while (angle > M_PI)
        angle -= M_PI;
    while (angle < -M_PI)
        angle += M_PI;
    
    if (facingUp) {
        while (angle > 0)
            angle -= M_PI;
    } else {
        while (angle < 0)
            angle += M_PI;
    }
    
    return angle;
}

void MapEdge::setSortedCollectionNode(SortedCollection<MapEdge>::Node * node) {
    this->node = node;
}

bool MapEdge::adjustVelocity(Vector * vel) const {
    return vel->subtractAngle(getNormalForceAngle());
}

void MapEdge::draw(Camera * camera, bool drawBumper) const {
    line.draw(camera, floor ? constants::green : constants::red);
    
    if (drawBumper) {
        bumper.draw(camera, touchingPlayer ? constants::white : constants::cyan);
        catchCW.draw(camera, constants::yellow);
        catchCCW.draw(camera, constants::yellow);
    }
}

std::string MapEdge::toString() const {
    return "MapEdge(" + getVertexCCW()->getCoordinate().toString() + "," + getVertexCW()->getCoordinate().toString() + ")";
}

std::string MapEdge::toString(const MapEdge * edge) {
    return edge->toString();
}