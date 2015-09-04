//
//  Bolt.cpp
//  Grappler
//
//  Created by Ethan Coeytaux on 9/3/15.
//  Copyright (c) 2015 Firework Gamelabs. All rights reserved.
//

#include "Bolt.h"

#include "Vector.h"

void alterColor(Color * color); //function stub

//TODO use variables for screen size

Bolt::Bolt(Coordinate origin) : path(origin), boltCount(0), struck(false), displayLength(constants::getRand(15, 40)) {
    int dx1, dy1, dx2, dy2;
    
    while (path.getEndPoint().getY() < 640) {
        dx1 = constants::getRand(-4, 4);
        dy1 = constants::getRand(4, 12);
        dx2 = constants::getRand(-5, 5);
        dy2 = constants::getRand(-5, 5);
        path.addVector(Vector(dx1 + dx2, dy1 + dy2));
        boltCount++;
    }
    
    struck = true;
}

Bolt::~Bolt() { }

void Bolt::update() {
    /*if ((boltCount > 0) || (path.getEndPoint().getY() < 640)) {
        int dx1, dy1, dx2, dy2;
        for (int i = 0; i < 5; i++) {
            dx1 = constants::getRand(-4, 4);
            dy1 = constants::getRand(4, 12);
            dx2 = constants::getRand(-5, 5);
            dy2 = constants::getRand(-5, 5);
            path.addVector(Vector(dx1 + dx2, dy1 + dy2));
        }
        boltCount--;
    } else {
        struck = true;
    }*/
}

void Bolt::draw(Camera * camera, const Color * color) {
    if (struck) {
        if (!flashed) {
            flashed = true;
            camera->drawRectangle(Coordinate(480, 320), 960, 640, constants::white);
            return;
        }
        
        if (displayLength-- > 0) {
            if (constants::getRandBool())
                path.draw(camera, color);
        }
    }/* else {
      Coordinate base = path.getEndPoint();
      camera->drawRectangle(base, 2, 2, color);
      for (int i = constants::getRand(0, 3); i > 0; i--) {
      Vector vec1(constants::getRand(-3, 3), constants::getRand(-3, 3));
      Vector vec2(constants::getRand(-3, 3), constants::getRand(-3, 3));
      
      camera->drawLine(base + vec1, base + vec2, color);
      }
      }*/
}