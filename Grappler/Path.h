//
//  Path.h
//  Climber
//
//  Created by Ethan Coeytaux on 7/23/15.
//  Copyright (c) 2015 Science Class Games. All rights reserved.
//

#ifndef Climber_Path_h
#define Climber_Path_h

#include "LinkedList.h"

class Path {
public:
    Path(Coordinate origin) : endPoint(origin), path() {
        addCoordinate(origin);
    }
    
    virtual ~Path() { }
    
    int getCount() {
        return path.getSize();
    }
    
    bool addCoordinate(Coordinate coor) {
        endPoint = coor;
        return path.addElement(coor);
    }
    
    bool addVector(Vector vec) {
        return addCoordinate(endPoint + vec);
    }
    
    bool clear() {
        return path.clear();
    }
    
    Coordinate getEndPoint() {
        return endPoint;
    }
    
private:
    Coordinate endPoint;
    LinkedList<Coordinate> path;
};


#endif
