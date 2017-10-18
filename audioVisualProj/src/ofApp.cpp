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
    
    /* This is stuff you always need for Maximilian */
    ofSetFrameRate(60);
    sampleRate     = 44100; /* Sampling Rate */
    bufferSize    = 512; /* Buffer Size. you have to fill this buffer with sound using the for loop in the audioOut method */
    ofxMaxiSettings::setup(sampleRate, 2, bufferSize);
    ofSetVerticalSync(true);
    ofEnableAlphaBlending();
    ofEnableSmoothing();
    
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
    
    sound.load(ofToDataPath("russian-night.mp3"));

    // default drawing rectangles
    drawCircle = false;
    drawSquare = true;

    // set up line
    radius = 20;
    lineWeight = 5;
    line = TimedLine(lineWeight);
    waitTime = 5;
    
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
    
    ratioShown = 0;
    
    // Play the background noise, applying a low pass filter
    backgroundSfx[currentImg].play();
    backgroundSfx[currentImg].setVolume(MIN(.1 + ratioShown,1));
    
}

//--------------------------------------------------------------
void ofApp::audioOut(float * output, int bufferSize, int nChannels) {
    
    
//    for (int i = 0; i < bufferSize; i++){
//
//        maxiFilter myFilter;
//        double myFilteredOutput = myFilter.lopass(sound.play(),800);
//
////        output[i] = myFilteredOutput;
////        output[1] = output[0];
//
//
//
//        output[i*nChannels    ] = myFilteredOutput; /* You may end up with lots of outputs. add them here */
//        output[i*nChannels + 1] = myFilteredOutput;
//
//    }
    
    
}

//--------------------------------------------------------------
void ofApp::audioIn(float * input, int bufferSize, int nChannels){
    
}

//--------------------------------------------------------------
void ofApp::update(){
    
    // the volume of the background noise is proportional to
    // the amount of background that is shown
    // update the sound playing system
    backgroundSfx[currentImg].setVolume(MIN(.1 + ratioShown,1));
    ofSoundUpdate();
//    cout << "volume: " << backgroundSfx[currentImg].getVolume() << endl;
    
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

    // ratio of how much background shown vs foreground
    // convert FBO into pixels
    ofPixels fboPixels;
    fbo.readToPixels(fboPixels);
    
    // convert pixels into a texture
//    ofTexture tex;
//    tex.loadData(fboPixels);
//    tex.draw(0,0);

    // get pixels of background image
    ofPixels backgroundPixels = foregrounds[currentImg].getPixels();
    
    // RGB values are interlaced (0=R, 1=G, 2=B for 1st pixel)
    // if pixel in fbo is not (0,0,0) then it was drawn on/shows the background
    int numPixels = 0;  // number of pixels drawn on
    for (int i = 0; i < fboPixels.size(); i+=3) {

        if (fboPixels[i] != 0 ||
            fboPixels[i+1] != 0 ||
            fboPixels[i+2] != 0) {
            numPixels++;
//            cout << i/3 << endl;
        }
        
//        if (fboPixels.getColor(i) != ofColor(0,0,0,0)) {
//            pixSame++;
//            cout << i << " " << fboPixels.getColor(i) << endl;
//        }
//            cout << i << " " << fboPixels.getColor(i) << endl;
//        if (fboPixels.getColor(i) == backgroundPixels.getColor(i)) {}
    }
//    cout << "num pixels: " << fboPixels.size() << endl;
//    ofLog() << "numPixels: " << numPixels << endl;
    
    ratioShown = float(numPixels) / (fboPixels.size()/3);
//    cout << ratioShown << endl;
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
