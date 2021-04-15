//
//  Ball.cpp
//  FindCowrieShell
//
//  Created by Thomas Lawanson on 02/04/2021.
//

#include "Ball.h"

Ball::Ball(float x, float y, float _r) {
    position = ofVec2f(x,y);
    velocity = ofVec2f(ofRandom(3),ofRandom(3));
    r = ofRandom(_r * 0.8, _r);
}

void Ball::update(){
    position += velocity;
    if(position.x > ofGetWidth() || position.x < 0){
        velocity.x *= -1;
    }
    
    if(position.y > ofGetHeight() || position.y < 0){
        velocity.y *= -1;
    }
}


void Ball::draw(){
    ofNoFill();
//    ofSetColor(0);
    ofDrawEllipse(position.x, position.y, r * 2, r * 2);
}
