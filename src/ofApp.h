#pragma once

#include "ofMain.h"
#include "ofxOpenCv.h"
#include "Cell.h"
#include "Vec2Key.h"

class ofApp : public ofBaseApp{

	public:
        bool hasChanged;
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
        float threshold;
    
        ofMesh mesh;
        ofEasyCam easyCam;
        vector<ofVec3f> offsets;

        // These variables will let us store the polar coordinates of each vertex
        vector<float> distances;
        vector<float> angles;
        ofVec3f meshCentroid;
    
		void setup();
		void update();
		void draw();
    
        void updateMotionDetector();
        
        vector<Cell> grid;
        void createGrid();
        void updateGrid();
        void drawGrid();
        
    
        void getDivination();
    
		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		
};
