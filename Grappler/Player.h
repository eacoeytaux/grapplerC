//
//  Player.h
//  Climber
//
//  Created by Ethan Coeytaux on 6/27/15.
//  Copyright (c) 2015 Science Class Games. All rights reserved.
//

#ifndef __Climber__Player__
#define __Climber__Player__

#include "Coordinate.h"
#include "Vector.h"
#include "Path.h"
#include "SpriteSheet.h"
#include "Animation.h"
#include "Rope.h"

class AbsLevel;
class MapElement;

class Player {
    friend class Rope;
    
public:
    static const int trueRadius = 40;
    
    Player(Coordinate center);
    virtual ~Player();
    
    Coordinate getCenter();
    void setCenter(Coordinate center);
    Vector getVelocity();
    void setVelocity(Vector vel);
    void addVelocity(Vector vel);
    void addForce(Vector force);
    MapElement * getElement();
    void setElement(MapElement * element);
    
    void setJumping(bool b);
    void setMovingRight(bool b);
    void setMovingLeft(bool b);
    
    bool isGrappling() const;
    const Rope * getRope() const;
    
    MapElement * getMapElement();
    void setMapElement(MapElement * element);
    bool touchesElement(MapElement * element) const;
    
    bool update(AbsLevel * level);
    void draw(Camera * camera);
    
    static void setSpriteSheet(SpriteSheet * sheet);
    
private:
    const double jumpSpeed = -8, moveForceGround = 0.5, moveForceAir = 0.75;
    
    Coordinate center;
    Vector force;
    Vector nextForce;
    Vector vel;
    Path path;
    
    bool jumping = false;
    bool movingRight = false;
    bool movingLeft = false;
    
    double angle = M_PI_4;
    
    MapElement * element = nullptr;
    
    Animation * currentAnimation = nullptr;
    void setCurrentAnimation(Animation * animation);
    
    bool grappling;
    Rope rope;
    
    static SpriteSheet * sheet;
};

#endif /* defined(__Climber__Player__) */
