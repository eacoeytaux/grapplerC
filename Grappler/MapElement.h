//
//  MapElement.h
//  Climber
//
//  Created by Ethan Coeytaux on 7/19/15.
//  Copyright (c) 2015 Science Class Games. All rights reserved.
//

#ifndef __Climber__MapElement__
#define __Climber__MapElement__

class Camera;
class Vector;
class Map;

class MapElement {
public:
    MapElement(const Map * parent);
    virtual ~MapElement();
    
    const Map * getParent();
    
    bool isTouchingPlayer() const;
    void setTouchingPlayer(bool touchingPlayer);
    
    virtual double getNormalForceAngle() const = 0;
    
    virtual bool adjustVelocity(Vector * vel) const = 0;
    
    virtual void draw(Camera * camera, bool drawBumper = false) const = 0;
    
protected:
    const Map * parent;
    
    bool touchingPlayer = false;
};

#endif /* defined(__Climber__MapElement__) */
