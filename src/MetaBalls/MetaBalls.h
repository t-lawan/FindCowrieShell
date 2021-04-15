//
//  MetaBalls.hpp
//  FindCowrieShell
//
//  Created by Thomas Lawanson on 02/04/2021.
//

#ifndef MetaBalls_h
#define MetaBalls_h

#include <stdio.h>
#include "ofMain.h"
#include "Ball.h"

class MetaBalls{
    public:
        float lerpValue = 0;
        int factor = 1;
        ofColor colour;
        ofColor lerpColour;
    
        ofImage img;
        int numOfBalls;
        vector<Ball> balls;
        ofTexture texture;
        void setup(ofColor _colour = ofColor::blue, ofColor _lerpColour = ofColor::red, float _radius = 40.0);
        void setupBalls(float radius = 40.0);
        void update();
        void draw();
};
#endif /* MetaBalls_h */
