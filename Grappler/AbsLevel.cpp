//
//  AbsLevel.cpp
//  Climber
//
//  Created by Ethan Coeytaux on 6/28/15.
//  Copyright (c) 2015 Science Class Games. All rights reserved.
//

#include "AbsLevel.h"

#include "Camera.h"
#include "Line.h"
#include "Circle.h"
#include "MapVertex.h"
#include "MapEdge.h"

AbsLevel::AbsLevel(SDL_Renderer * renderer) : map(), player(Coordinate(600, 50)) {
    loadResources();
}

AbsLevel::~AbsLevel() { }

void AbsLevel::update(Coordinate mouseCoor, float dt) { //TODO remove dt?
    if (dt == 0)
        return;
    
    map.update(mouseCoor);
    player.update(this);
    
    //for debugging
    /*for (SortedCollection<MapVertex>::Node * traversalNode = map.vertices.getHeadNode(); traversalNode != nullptr; traversalNode = traversalNode->next) {
        traversalNode->element->setTouchingPlayer(false);//player.touchesElement(traversalNode->element));
    }
    for (SortedCollection<MapEdge>::Node * traversalNode = map.edges.getHeadNode(); traversalNode != nullptr; traversalNode = traversalNode->next) {
        traversalNode->element->setTouchingPlayer(false);//player.touchesElement(traversalNode->element));
    }*/
}

void AbsLevel::draw(Camera * camera) {
    map.drawBackground(camera);
    map.drawElements(camera);
    
    //draw entities
    player.draw(camera);
    
    map.drawForeground(camera);
    
    //Line(lowerBound, 0, lowerBound, 640).draw(camera, constants::white.changeAlpha(127));
    //Line(upperBound, 0, upperBound, 640).draw(camera, constants::white.changeAlpha(127));
    
    Arc arc(Circle(Coordinate(200, 200), 50), -1.05, 3.9);
    camera->drawArc(arc, constants::yellow);
    
    Coordinate intersection(0, 0);
    Coordinate intersection2(0, 0);
    
    Line line(150, 290, 275, 160);
    
    camera->drawLine(line, constants::magenta);
    
    if (arc.doesIntersect(line, &intersection, &intersection2)) {
        camera->drawRectangle(intersection, 3, 3, constants::white);
        camera->drawRectangle(intersection2, 3, 3, constants::cyan);
        printf("x: %f, y: %f\n", intersection.getX(), intersection.getY());
    }
}

//const Map * AbsLevel::getMap() {
//    return &map;
//}

//Path AbsLevel::generatePath(Hitbox hitbox, Vector vel) {
//Path path(hitbox);
//}

void AbsLevel::generatePath(Player * player) {
    Path path(player->getCenter());
    
    //Hitbox hitbox = object->hitbox;
    Coordinate center = player->getCenter();
    Vector vel = player->getVelocity();
    MapElement * element = player->getElement();
    
    float percentage = 1.f;
    
    while (percentage > 0) {
        if (element != nullptr) {
            if (!element->adjustVelocity(&vel))
                element = nullptr;
        }
        
        if (player->isGrappling()) {
            const Rope * rope = player->getRope();
            
            if (rope->isTaught()) {
                vel.subtractAngle(constants::findAngle(rope->getOrigin(), rope->getRoot()));
                
                Coordinate velDestination(0, 0);
                if (vel.getDestination(&velDestination)) {
                    Line line(rope->getRoot(), velDestination);
                    Circle circle(rope->getRoot(), rope->getLength());
                    
                    Coordinate intersection(0, 0);
                    if (circle.doesIntersect(line, &intersection)) {
                        //vel.setDestination(intersection);
                    }
                    //if (circle)
                    
                    
                    //double angle = constants::findAngle(velDestination, rope->getRoot());
                    //double distance = constants::findDistance(velDestination, rope->getRoot()) - rope->getLength();
                    
                    
                    //Vector ropeAdjustment(cos(angle) * distance, sin(angle) * distance);
                    //vel += ropeAdjustment;
                }
            }
        }
        
        Vector adjustedVel = vel * percentage;
        
        if (adjustedVel.getMagnitude() == 0) {
            path.addVector(adjustedVel);
            break;
        }
        
        int lowerBound, upperBound;
        lowerBound = upperBound = center.getX();
        
        (adjustedVel.isDxPositive() ? upperBound : lowerBound) += adjustedVel.getDx();
        //printf("vel angle: %f\n", atan2(adjustedVel.getDy(), adjustedVel.getDx()));
        
        this->lowerBound = lowerBound;
        this->upperBound = upperBound;
        
        Coordinate tempCenter = center;
        Vector tempVel = adjustedVel;
        MapElement * tempElement = element;
        
        double distance = INFINITY;
        
        //check all vertices for collision
        for (SortedCollection<MapVertex>::Node * traversalNode = map.vertices.getNodeByValue(lowerBound - Player::trueRadius); (traversalNode != NULL) ? (traversalNode->value <= upperBound + Player::trueRadius) : false; traversalNode = traversalNode->next) {
            MapVertex * vertex = traversalNode->element;
            
            Coordinate collision = center;
            Line adjustedVelLine = adjustedVel.toLine(center);
            
            if (!(element == (MapElement*)vertex)) {
                if (vertex->getBumper().doesIntersect(adjustedVelLine, &collision)) {
                    Vector collisionVel(center, collision);
                    if (collisionVel.getMagnitude() < distance) {
                        tempCenter = collision;
                        tempVel = collisionVel;
                        tempElement = (MapElement*)vertex;
                        
                        distance = collisionVel.getMagnitude();
                    }
                }
            }/* else {
                if (adjustedVelLine.doesIntersect(edge->getCatchCW(), &collision)) {
                    Vector collisionVel(center, collision);
                    if (collisionVel.getMagnitude() < distance) {
                        tempCenter = collision;
                        tempVel = collisionVel;
                        tempElement = (MapElement*)edge->getVertexCW();
                        
                        distance = collisionVel.getMagnitude();
                    }
                } else if (adjustedVelLine.doesIntersect(edge->getCatchCCW(), &collision)) {
                    Vector collisionVel(center, collision);
                    if (collisionVel.getMagnitude() < distance) {
                        tempCenter = collision;
                        tempVel = collisionVel;
                        tempElement = (MapElement*)edge->getVertexCCW();
                        
                        distance = collisionVel.getMagnitude();
                    }
                }
            }*/
        }
        
        //check all edges for collision
        for (SortedCollection<MapEdge>::Node * traversalNode = map.edges.getNodeByValue(lowerBound - map.largestEdgeDx - Player::trueRadius); (traversalNode != NULL) ? (traversalNode->value <= upperBound + Player::trueRadius) : false; traversalNode = traversalNode->next) {
            MapEdge * edge = traversalNode->element;
            
            Coordinate collision = center;
            Line adjustedVelLine = adjustedVel.toLine(center);
            
            if (!(element == (MapElement*)edge)) {
                if (edge->getBumper().doesIntersect(adjustedVelLine, &collision)) {
                    Vector collisionVel(center, collision);
                    if (collisionVel.getMagnitude() < distance) {
                        tempCenter = collision;
                        tempVel = collisionVel;
                        tempElement = (MapElement*)edge;
                        
                        distance = collisionVel.getMagnitude();
                    }
                }
            } else {
                if (edge->getCatchCW().doesIntersect(adjustedVelLine, &collision)) {
                    Vector collisionVel(center, collision);
                    if (collisionVel.getMagnitude() < distance) {
                        tempCenter = collision;
                        tempVel = collisionVel;
                        tempElement = (MapElement*)edge->getVertexCW();
                        
                        distance = collisionVel.getMagnitude();
                    }
                } else if (edge->getCatchCCW().doesIntersect(adjustedVelLine, &collision)) {
                    Vector collisionVel(center, collision);
                    if (collisionVel.getMagnitude() < distance) {
                        tempCenter = collision;
                        tempVel = collisionVel;
                        tempElement = (MapElement*)edge->getVertexCCW();
                        
                        distance = collisionVel.getMagnitude();
                    }
                }
            }
        }
        
        //check all map elements touching player for boundaries
        /*if (adjustedVel.isDxPositive()) {
            if (hitbox.getBottomVertex() != nullptr) {
                double distanceToEdge = adjustedVel.getDx() > (hitbox.getBottomVertex()->getCoordinate().getX() - (hitbox.getCenter().getX() - hitbox.getHalfWidth()));
                if ((distanceToEdge <= adjustedVel.getMagnitude()) && (distanceToEdge < distance)) {
                    tempHitbox.clear();
                    tempHitbox.setBottomLeftEdge(hitbox.getBottomVertex()->getEdgeCW());
                    tempVel.setDxDy(distanceToEdge, 0);
                }
            }
        }*/
        
        //finished checking collisions
        
        float percentageUsed = tempVel.getMagnitude() / adjustedVel.getMagnitude();
        
        float prevPercentage = percentage;
        percentage *= (1 - percentageUsed);
        if (percentage == prevPercentage)
            percentage = 0;
        //printf("percentage: %f\n", percentage);
        
        center = tempCenter;
        //vel = tempVel; //TODO is this required? cause it creates an error...
        element = tempElement;
        
        path.addVector(tempVel);
    }
    
    /*hitbox.setCenter(path.getEndPoint());
    object->hitbox = hitbox;*/
    
    player->setCenter(path.getEndPoint());
    player->setVelocity(vel);
    player->setElement(element);
}

Player * AbsLevel::getPlayer() {
    return &player;
}

void AbsLevel::loadResources() {
    loadImages();
    loadSounds();
}

void AbsLevel::loadImages() {
    
}

void AbsLevel::loadSounds() {
    
}