//
//  MapElement.cpp
//  Grappler
//
//  Created by Ethan Coeytaux on 8/23/15.
//  Copyright (c) 2015 Firework Gamelabs. All rights reserved.
//

#include "MapElement.h"

MapElement::MapElement(const Map * parent) : parent(parent) { }

MapElement::~MapElement() { }

const Map * MapElement::getParent() {
    return parent;
}

bool MapElement::isTouchingPlayer() const {
    return touchingPlayer;
}

void MapElement::setTouchingPlayer(bool touchingPlayer) {
    this->touchingPlayer = touchingPlayer;
}