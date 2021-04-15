#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    bLearnBackground = false;
    vidGrabber.setVerbose(true);
    vidGrabber.listDevices();
    vidGrabber.setDeviceID(0);
    vidGrabber.initGrabber(width, height);
    numOfCowrieShell = 8;
    rTarget = 203;
    gTarget = 242;
    bTarget = 250;
    threshold = 20;

    colorImg.allocate(width, height);
    grayImage.allocate(width, height);
    grayBg.allocate(width, height);
    grayDiff.allocate(width, height);
    createGrid();
}

//--------------------------------------------------------------
void ofApp::update(){
    vidGrabber.update();
    //do we have a new frame?
    if (vidGrabber.isFrameNew()){
        colorImg.setFromPixels(vidGrabber.getPixels());
        grayImage = colorImg; // convert our color image to a grayscale image
        if (bLearnBackground == true) {
            grayBg = grayImage; // update the background image
            bLearnBackground = false;
        }
        grayDiff.absDiff(grayBg, grayImage);
        float threshold = ofMap(ofGetMouseX(), 0, ofGetWidth(), 1, 100); //26
        grayDiff.threshold(23);
        contourFinder.findContours(grayDiff, 5, (width * height)/3, numOfCowrieShell, false, true);
        
    }
    updateMotionDetector();
    updateGrid();
}

void ofApp::updateMotionDetector(){
    if(grayImage.bAllocated) {
        grayImagePrev = grayImage;
    }
    
    if(grayImagePrev.bAllocated){
        motionDetectorDiff.absDiff(grayImage, grayImagePrev);
        
        diffFloat = motionDetectorDiff;
        diffFloat *= 5.0;
        
        if(!bufferFloat.bAllocated){
            bufferFloat = diffFloat;
        } else {
            bufferFloat *= 0.85;
            bufferFloat += diffFloat;
        }
        
    }
}
//--------------------------------------------------------------
void ofApp::draw(){
    
    ofSetHexColor(0xffffff);
    colorImg.draw(0, 0, width, height);
    ofColor  c = ofColor::red;
    
    ofImage img;
    img.grabScreen(0, 0, width, height);
    ofColor co = img.getColor(ofGetMouseX(), ofGetMouseY());
    ofSetColor(co);
    ofDrawRectangle(ofGetMouseX(), ofGetMouseY(), 10, 10);
//    for(int i = 0; i < contourFinder.nBlobs; i++) {
//        ofVec3f r = contourFinder.blobs.at(i).centroid;
//        contourFinder.blobs.at(i).draw();
//        ofSetColor(c);
//        ofDrawEllipse(r.x, r.y, 20, 20);
//    }
//    drawGrid();
    
//    if(diffFloat.bAllocated){
//        int w = grayImage.width;
//        int h = grayImage.height;
//        float *pixels = bufferFloat.getPixelsAsFloats();
//        for (int y=0; y<h; y++) {
//            for (int x=0; x<w; x++) {
//                //Get the pixel value
//                float value = pixels[ x + w * y ];
//                //If value exceed threshold, then draw pixel
//                // between 0.3 and 2.0
//                if ( value >= 0.3 ) {
//                    ofSetColor(ofColor::blue);
//                    ofDrawRectangle( x, y, 1, 1 );
//                    //Rectangle with size 1x1 means pixel
//                    //Note, this is slow function,
//                    //we use it here just for simplicity
//                }
//            }
//        }
//    }

}




void ofApp::createGrid(){
    for(int x = 0; x < gridSize; x++) {
        for(int y = 0; y < gridSize; y++) {
            float xVal =(float) x/gridSize;
            float yVal =(float) y/gridSize;

            Vec2Key coordinates(xVal * width, yVal * width);
            Cell cell(xVal * width, yVal * height,width/gridSize,  height/gridSize);
            grid.push_back(cell);
        }
    }
}

void ofApp::updateGrid() {
    for(int x = 0; x < grid.size(); x++) {
        bool isOccupied = false;
        for(int i = 0; i < contourFinder.nBlobs; i++) {
            ofVec2f blobPos(contourFinder.blobs[i].centroid.x, contourFinder.blobs[i].centroid.y);
            

            if(grid[x].isWithinBounds(blobPos)) {
                isOccupied = true;
            }
        }
//        ofVec2f pos(ofGetMouseX(), ofGetMouseY());
//        isOccupied = grid[i].isWithinBounds(pos);
        grid[x].setIsOccupied(isOccupied);
    }
}


void ofApp::drawGrid(){
    ofPushStyle();
    ofNoFill();
    ofColor c(0);
    
    for(auto cell: grid) {
        if(cell.isOccupied) {
            c = ofColor::red;
        } else {
            c = ofColor::blue;
        }
        
        ofSetColor(c);
        ofDrawRectangle(cell.x, cell.y, cell.width, cell.height);
    }
    
    ofPopStyle();
}

void ofApp::getDivination(){
    int numOfCellsOccupied = 0;
    for(auto cell: grid) {
        if(cell.isOccupied) {
            numOfCellsOccupied++;
        }
    }
    
    int numOfOnCowrieShell = 0;
    for(int i = 0; i < contourFinder.nBlobs; i++) {
        
        
        // Get Rect
        ofRectangle rect = contourFinder.blobs[i].boundingRect;
        bool hasGreenSpot = false;
        
        // Get image of Blob
        ofImage img;
        img.grabScreen(rect.x, rect.y, rect.width, rect.height);
        cout << "WIDTH: " << img.getWidth() << ", HEIGHT: " << img.getHeight() << endl;
        // Loop through image and find color
        for(float x = 0; x < img.getWidth();  x++) {
            for(float y = 0; y < img.getHeight();  y++ ) {
                ofColor colorAtXY = img.getColor(x, y);
                float rAtXY = colorAtXY.r;
                float gAtXY = colorAtXY.g;
                float bAtXY = colorAtXY.b;
                
                float colorDistance = ofDist(rAtXY, gAtXY, bAtXY, rTarget, gTarget, bTarget);
                cout << "COLOUR: " << colorDistance << endl;
                if(colorDistance < threshold){
                    
                    hasGreenSpot = true;
                }
            }
        }
        
        if(hasGreenSpot){
            numOfOnCowrieShell++;
        }
    }
    
    cout << "NUM OF CELLS OCCUPIED: " << numOfCellsOccupied << endl;
    cout << "NUM OF ON COWRIE SHELL: " << numOfOnCowrieShell << endl;
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    bLearnBackground = true;
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
//    cout << "X : " << x << ", Y: " << y << endl;
    getDivination();
//    ofImage img;
//    img.grabScreen(0, 0, width, height);
//    ofColor co = img.getColor(x, y);
//    cout << "R : " << co.r << ", G: " << co.g << ", B: " << co.b << endl;
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
