//
//  Ball.hpp
//  FindCowrieShell
//
//  Created by Thomas Lawanson on 02/04/2021.
//

#ifndef Ball_h
#define Ball_h

#include <stdio.h>
#include "ofMain.h"
class Ball {
    public:
        Ball(float x = 0, float y= 0, float _r = 30);
        ofVec2f position;
        ofVec2f velocity;
        float r;
        void setup();
        void update();
        void draw();
};
#endif /* Ball_hpp */
