#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    // load shader
#ifdef TARGET_OPENGLES
    shader.load("shadersES2/shader");
#else
    if(ofIsGLProgrammableRenderer()){
        shader.load("shadersGL3/shader");
    }else{
        shader.load("shadersGL2/shader");
    }
#endif
    
    // load in sounds:
    chime.load("sounds/chimes_short.mp3");
    magic.load("sounds/sfx-magic.wav");
    
    ofSoundPlayer sfx;
    sfx.load("sounds/russian-rain.mp3");
    sfx.setLoop(true);
    backgroundSfx.push_back(sfx);
    sfx.load("sounds/steady-rain.wav");
    sfx.setLoop(true);
    backgroundSfx.push_back(sfx);
    sfx.load("sounds/rainy-night.mp3");
    sfx.setLoop(true);
    backgroundSfx.push_back(sfx);

    // default drawing rectangles
    drawCircle = false;
    drawSquare = true;

    // set up line
    radius = 20;
    lineWeight = 5;
    line = TimedLine(lineWeight);
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
    
    // allocate space for the frame buffers
    maskFbo.allocate(ofGetWidth(), ofGetHeight());
    fbo.allocate(ofGetWidth(), ofGetHeight());
    
    // Clear the FBO's
    // otherwise it will bring some junk with it from the memory
    maskFbo.begin();
    ofClear(0,0,0,255);
    maskFbo.end();
    
    fbo.begin();
    ofClear(0,0,0,255);
    fbo.end();
    
    // Play the background noise
    backgroundSfx[currentImg].play();
    
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
            waitTime += 0.5;
        }
        
        if (itr->size() == 0) {
            itr = lines.erase(itr);
            itr--;
        }
    }

}

//--------------------------------------------------------------
void ofApp::draw(){
    
    // Draw all the rectangles we've placed
//    for (int i = 0; i < rects.size(); i++) {
//        ofSetColor(ofColor::azure);
//        ofDrawRectangle(rects[i]);
//    }
    
    
    //----------------------------------------------------------
    // this is our alpha mask which we draw into.
    maskFbo.begin();
    
    // Clear mask each time draw on it, so that the line disappears
    ofClear(0,0,0,255);
    
    // Draw all the lines we've placed
    line.draw();
    for (list<TimedLine>::iterator itr = lines.begin(); itr != lines.end(); itr++) {
        itr->draw();
    }
    
    maskFbo.end();
    
    //----------------------------------------------------------
    // HERE the shader-masking happens
    fbo.begin();
    // Cleaning everthing with alpha mask on 0 in order to make it transparent by default
    ofClear(0, 0, 0, 0);
    
    shader.begin();
    // here is where the fbo is passed to the shader
    shader.setUniformTexture("maskTex", maskFbo.getTexture(), 1 );
    
    // Draw the background
    backgrounds[currentImg].draw(0,0);
    
    shader.end();
    fbo.end();
    
    //----------------------------------------------------------
    // FIRST draw the foreground image
    foregrounds[currentImg].draw(0,0);
    
    // THEN draw the masked fbo on top
    fbo.draw(0,0);

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    // Switching between drawing circles and squares
    if (key == 'c' || key == 'C') {
        drawCircle = true;
        drawSquare = false;
    } else if (key == 'r' || key == 'R') {
        drawCircle = false;
        drawSquare = true;
    }
    
    // Increasing/decreasing the size of the drawn shape
    else if (key == ']') {
//        radius += 5;
        lineWeight += 2;
        line = TimedLine(lineWeight);
    } else if (key == '[' && lineWeight > 1) {
//        radius -= 5;
        lineWeight -= 2;
        line = TimedLine(lineWeight);
    }
    
    // Changing the background
    else if (key == 'b' || key == 'B') {
        currentImg = (currentImg + 1) % backgrounds.size();
        backgroundSfx[currentImg].play();
    }
    
    // Clearing the window
    else if (key == ' ') {
        maskFbo.begin();
        ofClear(0,0,0,255);
        maskFbo.end();
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
    
//    ofRectangle r = ofRectangle(x, y, radius, radius);
//    rects.push_back(r);
    
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    // draw rectangle onto screen
//    ofRectangle r = ofRectangle(x, y, radius, radius);
//    rects.push_back(r);

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    // once released, play sound effect
    // use radius for volume of the samples:
    chime.play();
    chime.setVolume(MIN(radius/100.0f,1));
    
    // Store the line we just made into our vector
    lines.push_back(line);
    line = TimedLine(lineWeight);
    
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
