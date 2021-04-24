//
//  ObjectDetector.hpp
//  FindCowrieShell
//
//  Created by Thomas Lawanson on 23/04/2021.
//

#ifndef ObjectDetector_h
#define ObjectDetector_h

#include <stdio.h>
#include "ofMain.h"
#include "ofxOpenCv.h"

class ObjectDetector {
    public:
        ofMesh mesh;
        ofEasyCam easyCam;
        vector<ofVec3f> offsets;
        bool bLearnBackground;
        ofxCvColorImage colorImg;
        ofxCvGrayscaleImage grayImage, grayBg, grayDiff;
        float width = 960;
        float height = 720;
        int gridSize = 4;
        int numOfCowrieShell;
        float rTarget;
        float gTarget;
        float bTarget;
        float threshold;
        void setup();
        void update();
        void draw();
};
#endif /* ObjectDetector_hpp */
