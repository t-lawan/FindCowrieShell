#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetFrameRate(2);
    bLearnBackground = false;
    hasLearntBackground = false;
    
    vidGrabber.setVerbose(true);
    vidGrabber.listDevices();
    vidGrabber.setDeviceID(0);
    vidGrabber.initGrabber(width, height);
    
    numOfCowrieShell = 8;
    rTarget = 203;
    gTarget = 242;
    bTarget = 250;
    
    cowrieDetectorThreshold = 23;
    motionDetectorThreshold = 0.2;
    colourThreshold = 20;
    isChanging = false;

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
        updateCowrieDetector();
        updateMotionDetector();
        updateGrid();
    }
    

}

void ofApp::updateCowrieDetector(){
    colorImg.setFromPixels(vidGrabber.getPixels());
    grayImage = colorImg; // convert our color image to a grayscale image
    if (bLearnBackground == true) {
        grayBg = grayImage; // update the background image
        bLearnBackground = false;
        hasLearntBackground = true;
    }
    grayDiff.absDiff(grayBg, grayImage);
//    cowrieDetectorThreshold = ofMap(ofGetMouseX(), 0, ofGetWidth(), 1, 100); //26
    grayDiff.threshold(cowrieDetectorThreshold);
    contourFinder.findContours(grayDiff, 5, (width * height)/3, numOfCowrieShell, false, true);
}


void ofApp::updateMotionDetector(){
    if(hasLearntBackground && !isChanging && areAllCowrieShellsPresent()){
        if(grayImagePrev.bAllocated){
            motionDetectorDiff.absDiff(grayImage, grayImagePrev);
            ofPixels p = motionDetectorDiff.getPixels();
            unsigned char* pixelData = p.getData();
            float cValue = 0.0;
            for(int i = 0; i < p.size(); i+=20){
                cValue = cValue + pixelData[i];
            }
            cValue = cValue/ p.size();
//            cout << cValue << endl;
            if(cValue > motionDetectorThreshold){
                cout << "CHANGED" << endl;
                isChanging = true;
                getDivination();
                isChanging = false;
            }
        }
    }

    if(grayImage.bAllocated) {
        grayImagePrev = grayImage;
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

}

void ofApp::setupClient(){
    ofxTCPSettings settings("127.0.0.1", 11324);
    client.setup(settings);
}

void ofApp::sendMessageToServer(){
    setupClient();
    if(client.isConnected()){
        // we are connected - lets try to receive from the server
        string str = client.receive();
        client.send(json.dump());

    }
    
    client.close();
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
        grid[x].setIsOccupied(isOccupied);
    }
}

bool ofApp::areAllCowrieShellsPresent(){
    bool r = contourFinder.nBlobs == numOfCowrieShell;
    cout << r << endl;
    return r;
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
        // Loop through image and find color
        for(float x = 0; x < img.getWidth();  x++) {
            for(float y = 0; y < img.getHeight();  y++ ) {
                ofColor colorAtXY = img.getColor(x, y);
                float rAtXY = colorAtXY.r;
                float gAtXY = colorAtXY.g;
                float bAtXY = colorAtXY.b;
                
                float colorDistance = ofDist(rAtXY, gAtXY, bAtXY, rTarget, gTarget, bTarget);
                if(colorDistance < colourThreshold){
                    hasGreenSpot = true;
                }
            }
        }
        
        if(hasGreenSpot){
            numOfOnCowrieShell++;
        }
    }
    
    state.numOfCellsOccupied = (float) numOfCellsOccupied/(gridSize* gridSize);
    state.numOfOnCowrieShell = (float) numOfOnCowrieShell/numOfCowrieShell;
    sendStateToSimulation();
}

void ofApp::sendStateToSimulation(){
    json["cells_occupied"] = to_string(state.numOfCellsOccupied);
    json["cowrie_shell"] = to_string(state.numOfOnCowrieShell);
    sendMessageToServer();
    cout << json.dump() << endl;
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    bLearnBackground = true;
}


//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    getDivination();
}


//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
