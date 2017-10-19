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
    bufferSize    = 512; /* Buffer Size. fill this buffer with sound using the for loop in the audioOut method */
    // ofxMaxim can only read wav and ogg files
    sound.load(ofToDataPath("sounds/steady-rain.wav"));

    
    ofxMaxiSettings::setup(sampleRate, 2, bufferSize);
    ofSoundStreamSetup(2, 0, sampleRate, bufferSize, 4);
    m_param_lp_cutoff = 1.0;
    
    ofSetVerticalSync(true);
    ofEnableAlphaBlending();
    ofEnableSmoothing();
    
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
    

    // set up line
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
//    backgroundSfx[currentImg].play();
//    backgroundSfx[currentImg].setVolume(MIN(.1 + ratioShown,1));
    
    
}

//--------------------------------------------------------------
void ofApp::update(){
    
    // the volume of the background noise is proportional to
    // the amount of background that is shown
    // update the sound playing system
//    backgroundSfx[currentImg].setVolume(MIN(.1 + ratioShown,1));
//    ofSoundUpdate();
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
        }

    }
    
    ratioShown = float(numPixels) / (fboPixels.size()/3);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    // Increasing/decreasing the size of the drawn shape
    if (key == ']') {
        lineWeight += 2;
        line = TimedLine(lineWeight);
    } else if (key == '[' && lineWeight > 1) {
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
    
    // Clearing the window
    else if (key == 'q') {
        if(m_param_lp_cutoff < 1.0)
            m_param_lp_cutoff += .05;
    }
    else if (key == 'a') {
        if(m_param_lp_cutoff > 0.0)
        m_param_lp_cutoff -= .05;
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
    
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    
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
void ofApp::audioOut(float * output, int bufferSize, int nChannels) {
    
    double sample;
    for (unsigned int i = 0; i < bufferSize; ++i ) {
        
        sample = sound.play();
  
        sample = m_filter_lp.lopass(sample, m_param_lp_cutoff);
          
        
        output[i * nChannels] = sample;
        output[i * nChannels + 1] = sample;
        
    }
    
    
    
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
void ofApp::exit() {
    
    ofSoundStreamClose();
    
}
