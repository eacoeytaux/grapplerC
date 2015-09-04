//
//  Map.cpp
//  Climber
//
//  Created by Ethan Coeytaux on 6/28/15.
//  Copyright (c) 2015 Science Class Games. All rights reserved.
//

#include "Map.h"

#include <math.h>

#include "LinkedList.h"
#include "MapVertex.h"
#include "MapEdge.h"
#include "Player.h"

bool Map::showBumpers = false;

Map::Map() : vertices(), edges() {
    loadElements();
}

Map::~Map() { }

void Map::loadElements() {
    MapVertex * v0 = createVertex(100, 0);
    MapVertex * v1 = createVertex(10, 300);
    MapVertex * v2 = createVertex(175, 420);
    //MapVertex * v3 = createVertex(250, 420);
    MapVertex * v4 = createVertex(400, 350);
    MapVertex * v5 = createVertex(450, 375);
    MapVertex * v6 = createVertex(500, 350);
    MapVertex * v7 = createVertex(800, 400);
    MapVertex * v8 = createVertex(750, 0);
    
    createEdge(v1, v0, false, false, 1);
    createEdge(v2, v1, true, 2);
    //createEdge(v3, v2, true, 2);
    createEdge(v4, v2, true, 2);
    createEdge(v5, v4, true, 2);
    createEdge(v6, v5, true, 2);
    createEdge(v7, v6, true, 2);
    createEdge(v8, v7, false, false, 1);
}

void Map::update(Coordinate mouseCoor) {
    
}

void Map::drawElements(Camera * camera) {
    for (SortedCollection<MapEdge>::Node * traversalNode = edges.getHeadNode(); traversalNode != nullptr; traversalNode = traversalNode->next) {
        traversalNode->element->draw(camera, showBumpers);
    }
    
    for (SortedCollection<MapVertex>::Node * traversalNode = vertices.getHeadNode(); traversalNode != nullptr; traversalNode = traversalNode->next) {
        traversalNode->element->draw(camera, showBumpers);
    }
}

void Map::drawBackground(Camera * camera) {
    
}

void Map::drawForeground(Camera * camera) {
    
}

MapVertex * Map::createVertex(int x, int y, bool grabbable) {
    MapVertex * vertex = new MapVertex(this, Coordinate(x, y), grabbable);
    
    vertex->setSortedCollectionNode(vertices.addElement(vertex, vertex->getCoordinate().getX(), &MapVertex::toString));
    
    return vertex;
}

MapEdge * Map::createEdge(MapVertex * vertexCW, MapVertex * vertexCCW, bool floor, bool facingUp, int strength) {
    MapEdge * edge = new MapEdge(this, vertexCW, vertexCCW, floor, facingUp, strength);
    
    edge->setSortedCollectionNode(edges.addElement(edge, edge->line.getCoorLow().getX(), &MapEdge::toString));
    
    int edgeDx = fabs(vertexCW->getCoordinate().getX() - vertexCCW->getCoordinate().getX());
    if (edgeDx > largestEdgeDx)
        largestEdgeDx = edgeDx;
    
    printf("normal force angle: %f, facing up: %s", edge->getNormalForceAngle(), edge->facingUp ? "true\n" : "false\n");// * 57.2957795);
    
    return edge;
}
