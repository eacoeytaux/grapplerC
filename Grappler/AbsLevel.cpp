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
#include "Arc.h"
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
    vel.setOrigin(&center);
    MapElement * element = player->getElement();
    
    const Rope * rope = player->getRope();
    bool taught = rope->isTaught();
    
    float percentage = 1.f;
    
    while (percentage > 0) {
        bool velAdjusted = false;
        
        if (element != nullptr) {
            if (!element->adjustVelocity(&vel))
                element = nullptr;
            else
                velAdjusted = true;
        }
        
        if (player->isGrappling()) {
            
            if (taught) {
                if (vel.subtractAngle(constants::findAngle(rope->getOrigin(), rope->getRoot()))) {
                    if (velAdjusted) {
                        vel.setDxDy(0, 0);
                    } else {
                        Coordinate velOrigin(0, 0);
                        vel.getOrigin(&velOrigin);
                        
                        Arc arc(Circle(rope->getRoot(), rope->getLength()), constants::findAngle(rope->getRoot(), velOrigin), (vel.getMagnitude() / rope->getLength()) * ((constants::findAngleDifference(atan2(vel.getDy(), vel.getDx()), constants::findAngle(rope->getRoot(), rope->getOrigin())) > 0) ? -1 : 1));
                        
                        //if ((element != nullptr) && (fabs(constants::findAngleDifference(constants::findAngle(arc.getStart(), arc.getEnd()), element->getNormalForceAngle())) >= M_PI_2));
                        vel.setDestination(arc.getEnd());
                        
                        /*Coordinate velDestination(0, 0);
                         if (vel.getDestination(&velDestination)) {
                         this->line = Line(rope->getRoot(), velDestination);
                         this->circle = Circle(rope->getRoot(), rope->getLength());
                         
                         Coordinate intersection(0, 0);
                         if (circle.doesIntersect(line, &intersection)) {
                         
                         printf("distance: %f\n", constants::findDistance(circle.getCenter(), intersection));
                         this->coor = intersection;
                         vel.getOrigin(&(this->coor2));
                         if (!vel.setDestination(intersection))
                         printf("WARNING\n");
                         
                         this->vec = vel;
                         }
                         //if (circle)
                         
                         
                         //double angle = constants::findAngle(velDestination, rope->getRoot());
                         //double distance = constants::findDistance(velDestination, rope->getRoot()) - rope->getLength();
                         
                         
                         //Vector ropeAdjustment(cos(angle) * distance, sin(angle) * distance);
                         //vel += ropeAdjustment;
                         }*/
                    }
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
        
        Coordinate nextCenter = center;
        Vector nextVel = adjustedVel;
        MapElement * nextElement = element;
        
        double distance = INFINITY;
        
        bool nextTaught = taught;
        
        if (player->isGrappling() && !taught) {
            Coordinate collision = center;
            Line adjustedVelLine = adjustedVel.toLine(center);
            
            if (Circle(rope->getRoot(), rope->getLength()).doesIntersect(adjustedVelLine, &collision)) {
                Vector collisionVel(center, collision);
                if (collisionVel.getMagnitude() < distance) {
                    nextCenter = collision;
                    nextVel = collisionVel;
                    
                    distance = collisionVel.getMagnitude();
                    nextTaught = true;
                }
            }
        }
        
        //check all vertices for collision
        for (SortedCollection<MapVertex>::Node * traversalNode = map.vertices.getNodeByValue(lowerBound - Player::trueRadius); (traversalNode != NULL) ? (traversalNode->value <= upperBound + Player::trueRadius) : false; traversalNode = traversalNode->next) {
            MapVertex * vertex = traversalNode->element;
            
            Coordinate collision = center;
            Line adjustedVelLine = adjustedVel.toLine(center);
            
            if (!(element == (MapElement*)vertex)) {
                if (vertex->getBumper().doesIntersect(adjustedVelLine, &collision)) {
                    Vector collisionVel(center, collision);
                    if (collisionVel.getMagnitude() < distance) {
                        nextCenter = collision;
                        nextVel = collisionVel;
                        nextElement = (MapElement*)vertex;
                        
                        distance = collisionVel.getMagnitude();
                        nextTaught = taught;
                    }
                }
            }/* else {
              if (adjustedVelLine.doesIntersect(edge->getCatchCW(), &collision)) {
              Vector collisionVel(center, collision);
              if (collisionVel.getMagnitude() < distance) {
              nextCenter = collision;
              nextVel = collisionVel;
              nextElement = (MapElement*)edge->getVertexCW();
              
              distance = collisionVel.getMagnitude();
              }
              } else if (adjustedVelLine.doesIntersect(edge->getCatchCCW(), &collision)) {
              Vector collisionVel(center, collision);
              if (collisionVel.getMagnitude() < distance) {
              nextCenter = collision;
              nextVel = collisionVel;
              nextElement = (MapElement*)edge->getVertexCCW();
              
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
                        nextCenter = collision;
                        nextVel = collisionVel;
                        nextElement = (MapElement*)edge;
                        
                        distance = collisionVel.getMagnitude();
                        nextTaught = taught;
                    }
                }
            } else {
                if (edge->getCatchCW().doesIntersect(adjustedVelLine, &collision)) {
                    Vector collisionVel(center, collision);
                    if (collisionVel.getMagnitude() < distance) {
                        nextCenter = collision;
                        nextVel = collisionVel;
                        nextElement = (MapElement*)edge->getVertexCW();
                        
                        distance = collisionVel.getMagnitude();
                        nextTaught = taught;
                    }
                } else if (edge->getCatchCCW().doesIntersect(adjustedVelLine, &collision)) {
                    Vector collisionVel(center, collision);
                    if (collisionVel.getMagnitude() < distance) {
                        nextCenter = collision;
                        nextVel = collisionVel;
                        nextElement = (MapElement*)edge->getVertexCCW();
                        
                        distance = collisionVel.getMagnitude();
                        nextTaught = taught;
                    }
                }
            }
        }
        
        //check all map elements touching player for boundaries
        /*if (adjustedVel.isDxPositive()) {
         if (hitbox.getBottomVertex() != nullptr) {
         double distanceToEdge = adjustedVel.getDx() > (hitbox.getBottomVertex()->getCoordinate().getX() - (hitbox.getCenter().getX() - hitbox.getHalfWidth()));
         if ((distanceToEdge <= adjustedVel.getMagnitude()) && (distanceToEdge < distance)) {
         nextHitbox.clear();
         nextHitbox.setBottomLeftEdge(hitbox.getBottomVertex()->getEdgeCW());
         nextVel.setDxDy(distanceToEdge, 0);
         }
         }
         }*/
        
        //finished checking collisions
        
        float percentageUsed = nextVel.getMagnitude() / adjustedVel.getMagnitude();
        
        float prevPercentage = percentage;
        percentage *= (1 - percentageUsed);
        if (percentage == prevPercentage)
            percentage = 0;
        //printf("percentage: %f\n", percentage);
        
        center = nextCenter;
        //vel = nextVel; //TODO is this required? cause it creates an issue where the adjustedVel length is used in adjusted the velocity
        element = nextElement;
        taught = nextTaught;
        
        path.addVector(nextVel);
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