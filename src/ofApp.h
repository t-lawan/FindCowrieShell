#pragma once

#include "ofMain.h"
#include "ofxOpenCv.h"
#include "ofxNetwork.h"
#include "Cell.h"
#include "Vec2Key.h"
class State {
    public:
        float numOfCellsOccupied = 0.0;
        float numOfOnCowrieShell = 0.0;
};

class ofApp : public ofBaseApp{

	public:
        State state;
        bool isChanging;
        bool hasLearntBackground;
        bool bLearnBackground;
        ofVideoGrabber vidGrabber;
        ofxCvColorImage colorImg;
        ofxCvGrayscaleImage grayImage, grayBg, grayDiff;
        ofxCvGrayscaleImage grayImagePrev, motionDetectorDiff;
        ofxCvFloatImage diffFloat;        //Amplified difference images
        ofxCvFloatImage bufferFloat;    //Buffer image
        ofxCvContourFinder contourFinder;
    
        float width = 960;
        float height = 720;
        int gridSize = 4;
    
        int numOfCowrieShell;
    
        float rTarget;
        float gTarget;
        float bTarget;
        float colourThreshold;
        float cowrieDetectorThreshold;
        float motionDetectorThreshold;
        ofMesh mesh;
        ofEasyCam easyCam;
        vector<ofVec3f> offsets;
        ofJson json;
        
    
    
		void setup();
		void update();
		void draw();
    
        void updateMotionDetector();
        void updateCowrieDetector();
        bool areAllCowrieShellsPresent();
        
        vector<Cell> grid;
        void createGrid();
        void updateGrid();
        void drawGrid();
    
        ofxTCPClient client;
        void setupClient();
        void sendMessageToServer();
        int connectTime;
        int deltaTime;
    
        void getDivination();
    
        void sendStateToSimulation();
    
		void keyPressed(int key);
		void mousePressed(int x, int y, int button);

		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		
};
