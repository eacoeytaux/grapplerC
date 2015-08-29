//
//  Player.cpp
//  Climber
//
//  Created by Ethan Coeytaux on 6/27/15.
//  Copyright (c) 2015 Science Class Games. All rights reserved.
//

#include "Player.h"

#include "Camera.h"
#include "AbsLevel.h"
#include "MapElement.h"

Player::Player(Coordinate center) : center(center), vel(0, 0, &(this->center)), force(0, 0, &(this->center)), nextForce(0, 0), path(center), jumping(false), movingRight(false), movingLeft(false), grappling(true), rope(this, new Coordinate(400, 200), 300) {
    //currentAnimation = &standingAnimation;
}

Player::~Player() { }

Coordinate Player::getCenter() {
    return center;
}

void Player::setCenter(Coordinate center) {
    this->center = center;
    rope.updateParabola();//&a, &b, &c, *origin, *root, length);
}

Vector Player::getVelocity() {
    return vel;
}

void Player::setVelocity(Vector vel) {
    this->vel.setDxDy(vel.getDx(), vel.getDy());
}

void Player::addVelocity(Vector vel) {
    this->vel += vel;
}

void Player::addForce(Vector force) {
    nextForce += force;
}

MapElement * Player::getElement() {
    return element;
}

void Player::setElement(MapElement * element) {
    if (this->element != nullptr)
        this->element->setTouchingPlayer(false);
    
    if (element != nullptr)
        element->setTouchingPlayer(true);
    
    this->element = element;
}

void Player::setJumping(bool b) {
    jumping = b;
}

void Player::setMovingRight(bool b) {
    movingRight = b;
}

void Player::setMovingLeft(bool b) {
    movingLeft = b;
}

bool Player::isGrappling() const {
    return grappling;
}

const Rope * Player::getRope() const {
    return &rope;
}

MapElement * Player::getMapElement() {
    return element;
}

void Player::setMapElement(MapElement * element) {
    this->element = element;
}

bool Player::touchesElement(MapElement * element) {
    return (this->element == element);
}

bool Player::update(AbsLevel * level) {
    force.setDxDy(0, 0);
    
    force += constants::gravity;
    
    /*if (!hitbox.touchesFloor())
        force += constants::gravity;
    else {
        if (jumping) {
            force.setDy(jumpSpeed);
        }
    }
    
    if (movingRight) {
        if (hitbox.getBottomRightEdge() != nullptr) {
            if (hitbox.getBottomRightEdge()->isFloor()) {
                Line line = hitbox.getBottomRightEdge()->getLine();
                force += Vector(moveForceGround * line.getXAdjustment(), moveForceGround * line.getYAdjustment());
            }
        } else if (hitbox.getBottomVertex() != nullptr){
            force += Vector(moveForceGround, 0);
        } else if (hitbox.getBottomLeftEdge() != nullptr) {
            if (hitbox.getBottomLeftEdge()->isFloor()) {
                Line line = hitbox.getBottomLeftEdge()->getLine();
                force += Vector(moveForceGround * line.getXAdjustment(), moveForceGround * line.getYAdjustment());
            }
        } else {
            force += Vector(moveForceAir, 0);
        }
    }
    if (movingLeft) {
        if (hitbox.getBottomLeftEdge() != nullptr) {
            if (hitbox.getBottomLeftEdge()->isFloor()) {
                Line line = hitbox.getBottomLeftEdge()->getLine();
                force += Vector(-moveForceGround * line.getXAdjustment(), -moveForceGround * line.getYAdjustment());
            }
        } else if (hitbox.getBottomVertex() != nullptr){
            force += Vector(-moveForceGround, 0);
        } else if (hitbox.getBottomRightEdge() != nullptr) {
            if (hitbox.getBottomRightEdge()->isFloor()) {
                Line line = hitbox.getBottomRightEdge()->getLine();
                force += Vector(-moveForceGround * line.getXAdjustment(), -moveForceGround * line.getYAdjustment());
            }
        } else {
            force += Vector(-moveForceAir, 0);
        }
    }
    
    vel += force;
    if (jumping) {
        jumping = false;
        if (hitbox.touchesFloor() && (vel.getDy() > jumpSpeed))
            vel.setDy(jumpSpeed);
    }*/
    
    if (movingRight)
        force += Vector(moveForceGround, 0);
    if (movingLeft)
        force += Vector(-moveForceGround, 0);
    
    if (jumping && (element != nullptr))
        force += Vector(0, jumpSpeed);
    jumping = false;
    
    printf("force dx: %f, dy: %f\n", force.getDx(), force.getDy());
    
    //if (element != nullptr)
        //printf(force.subtractAngle(element->getNormalForceAngle()) ? "true\n" : "false\n");
    
    if (grappling)
        rope.update();
    
    force += nextForce;
    nextForce.setDxDy(0, 0);
    
    Vector dVel = force;
    vel += dVel;
    
    Coordinate prevCenter = center;
    level->generatePath(this);
    
    angle += (center.getX() - prevCenter.getX()) / trueRadius;
    while (angle > M_PI)
        angle -= M_PI;
    while (angle < -M_PI)
        angle += M_PI;
    
    printf("final force dx: %f, dy: %f\n\n", (force + nextForce).getDx(), (force + nextForce).getDy());
    
    //vel.setDx(vel.getDx() * constants::frictionNorm);
    //vel.setDy(vel.getDy() * (hitbox.touchesFloor() ? constants::frictionNorm : constants::airResistance));
    
    return true;
}

void Player::draw(Camera * camera) {
    camera->drawCircle(center, trueRadius, constants::magenta);
    
    double sinAngle = sin(angle) * trueRadius;
    double cosAngle = cos(angle) * trueRadius;
    camera->drawLine(center - Vector(cosAngle, sinAngle), center + Vector(cosAngle, sinAngle));
    camera->drawLine(center - Vector(-sinAngle, cosAngle), center + Vector(-sinAngle, cosAngle));
    //camera->drawLine(center - Vector(sinAngle, cosAngle), center + Vector(sinAngle, cosAngle));
    
    vel.draw(camera, constants::yellow, 5);
    force.draw(camera, constants::red, 50);
    
    if (grappling)
        rope.draw(camera, constants::white);
}

void Player::setSpriteSheet(SpriteSheet * sheet) {
    Player::sheet = sheet;
}

SpriteSheet * Player::sheet = nullptr;