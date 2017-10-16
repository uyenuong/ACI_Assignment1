#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofBackground(0, 0, 0);
    
    // load in sounds:
    chime.load("sounds/chimes_short.mp3");
    magic.load("sounds/sfx-magic.wav");
    
//    color = ofColor::azure;

    // default drawing rectangles
    drawCircle = false;
    drawSquare = true;
    
    radius = 20;
    ofSetLineWidth(10);
    waitTime = 2;
    
    // load in images
    ofImage img = ofImage("images/city_night.jpg");
    backgrounds.push_back(img);
    img = ofImage("images/vietnam.jpg");
    backgrounds.push_back(img);
    img = ofImage("images/budapest.jpg");
    backgrounds.push_back(img);
    
    img = ofImage("images/city_night_blurry.jpg");
    foregrounds.push_back(img);
    img = ofImage("images/vietnam_blurred.jpg");
    foregrounds.push_back(img);
    img = ofImage("images/budapest_blurred.jpg");
    foregrounds.push_back(img);
    
    currentImg = 0;
    
    
}

//--------------------------------------------------------------
void ofApp::update(){
    
    // update the sound playing system:
    ofSoundUpdate();
    
    // get rid of vertices in lines that have been present for a certain amount of time
    // if a line has no more vertices, remove it from the list
    for (list<TimedLine>::iterator itr = lines.begin(); itr != lines.end(); itr++) {
        int ptsDeleted = itr->stripVertices(waitTime);
        
        if (ptsDeleted > 0) {
            waitTime += 0.75;
        }
        
        if (itr->size() == 0) {
            itr = lines.erase(itr);
            itr--;
        }
    }

}

//--------------------------------------------------------------
void ofApp::draw(){
    // Draw the background
    backgrounds[currentImg].draw(0,0);
    
    // Draw all the rectangles we've placed
    for (int i = 0; i < rects.size(); i++) {
//        ofSetColor(ofColor::azure);
        ofDrawRectangle(rects[i]);
    }
//    ofRectang
    
    // Draw all the lines we've placed
    line.draw();
    thickLine.draw();
    for (list<TimedLine>::iterator itr = lines.begin(); itr != lines.end(); itr++) {
        itr->draw();
    }

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    // Switching between drawing circles and squares
    if (key == 'c') {
        drawCircle = true;
        drawSquare = false;
    } else if (key == 'r') {
        drawCircle = false;
        drawSquare = true;
    }
    
    // Increasing/decreasing the size of the drawn shape
    else if (key == ']') {
        radius += 5;
    } else if (key == '[' && radius > 10) {
        radius -= 5;
        
    }
    
    // Changing the background
    else if (key == 'b') {
        currentImg = (currentImg + 1) % backgrounds.size();
    }

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    // Adding points to the line
    ofPoint pt;
    pt.set(x,y);
    line.addVertex(pt);
    l.addVertex(pt);
    
//    ofRectangle r = ofRectangle(x, y, radius, radius);
//    rects.push_back(r);
    
    // If crosses itself, play a noise
//    if (line.size() > 0 && line.getVertices()[0] == pt) {
//        magic.play();
//    }
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    // draw rectangle onto screen
    ofRectangle r = ofRectangle(x, y, radius, radius);
    rects.push_back(r);

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    // once released, play sound effect
    // use radius for volume of the samples:
    chime.play();
    chime.setVolume(MIN(radius/100.0f,1));
    
    // Store the line we just made into our vector
    lines.push_back(line);
    line = TimedLine();
    thickLine.setFromPolyline(l);
    
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
