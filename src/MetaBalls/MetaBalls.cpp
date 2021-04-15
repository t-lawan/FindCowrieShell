//
//  MetaBalls.cpp
//  FindCowrieShell
//
//  Created by Thomas Lawanson on 02/04/2021.
//

#include "MetaBalls.h"



void MetaBalls::setup(ofColor _colour, ofColor _lerpColour, float _radius){
    ofBackground(0);
    colour = _colour;
    lerpColour = _lerpColour;
    ofBackground(51);
    numOfBalls = 6;
    setupBalls(_radius);
    texture.allocate(ofGetWidth(), ofGetHeight(), OF_PIXELS_RGBA);
//    ofEnableBlendMode(OF_BLENDMODE_MULTIPLY);
}

void MetaBalls::update(){
    
        img.grabScreen(0, 0, ofGetWidth(), ofGetHeight());
        ofPixels pixels = img.getPixels();
    
        for(int x = 0; x < ofGetWidth(); x++){
            for(int y = 0; y < ofGetHeight(); y++){
                int index = x + y * ofGetWidth();
                float sum = 0;
                for(auto & ball : balls){
                    float d =  ofDist(x, y, ball.position.x, ball.position.y);
                    sum += 200 * balls[0].r / d;
                }
                float colorValue = ofClamp(sum, 0, 150);
                ofColor c = colour;
                c.lerp(lerpColour, ofMap(colorValue, 0, 150, 1, 0));
                c.a = colorValue;
//                ofColor(255, 0, 0, colorValue);
                
                pixels.setColor(x, y, c);
            }
        }
        texture.loadData(pixels);
        for(auto & ball : balls){
            ball.update();
        }
}

//--------------------------------------------------------------
void MetaBalls::draw(){
    ofClear(0);
//    ofSetColor();
    texture.draw(0,0);
}

void MetaBalls::setupBalls(float radius){
    for(int i=0; i < numOfBalls; i++) {
        balls.push_back(Ball(ofRandomWidth(), ofRandomHeight(), radius));
    }
}
