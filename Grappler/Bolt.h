//
//  Bolt.h
//  Grappler
//
//  Created by Ethan Coeytaux on 9/3/15.
//  Copyright (c) 2015 Firework Gamelabs. All rights reserved.
//

#ifndef __Grappler__Bolt__
#define __Grappler__Bolt__

#include "Constants.h"
#include "Camera.h"
#include "Coordinate.h"
#include "Line.h"
#include "Path.h"

class Bolt {
public:
    Bolt(Coordinate origin = Coordinate(0, 0));
    virtual ~Bolt();
    
    void update();
    void draw(Camera * camera, const Color * color = constants::white);
    
private:
    Path path;
    int boltCount, displayLength;
    bool struck, flashed = false;
};

#endif /* defined(__Grappler__Bolt__) */
