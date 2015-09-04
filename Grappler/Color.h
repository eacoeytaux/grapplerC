//
//  Color.h
//  Climber
//
//  Created by Ethan Coeytaux on 6/28/15.
//  Copyright (c) 2015 Science Class Games. All rights reserved.
//

#ifndef __Climber__Color__
#define __Climber__Color__

#include <stdio.h>

class Color {
public:
    Color(int R, int G, int B, int A = 255) {
        RGBA = new int[4];
        RGBA[0] = R;
        RGBA[1] = G;
        RGBA[2] = B;
        RGBA[3] = A;
    }
    
    virtual ~Color() {
        delete RGBA;
    }
    
    int getRGBA(int index) const {
        if ((index < 0) || (index > 3)) {
            printf("Color Error: RGBA index out of bounds\n");
            return 0;
        }
        
        return RGBA[index];
    }
    
    void setRGBA(int value, int index) {
        if ((index < 0) || (index > 3)) {
            printf("Color Error: RGBA index out of bounds\n");
            return;
        } else if ((value < 0) || (value > 255)) {
            printf("Color Error: value out of bounds\n");
            return;
        }
        
        RGBA[index] = value;
    }
    
    void addToRGBA(int value, int index) {
        if ((index < 0) || (index > 3)) {
            printf("Color Error: RGBA index out of bounds\n");
            return;
        } else if ((value < -RGBA[index]) || (value > (255 - RGBA[index]))) {
            printf("Color Error: value out of bounds\n");
            return;
        }
        
        RGBA[index] += value;
    }
    
    Color * changeAlpha(int A) const {
        return new Color(RGBA[0], RGBA[1], RGBA[2], A);
    }
    
private:
    int * RGBA;
};

#endif /* defined(__Climber__Color__) */
