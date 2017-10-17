#pragma once

#include "ofMain.h"
#include "timedLine.hpp"
#include "ofxFatLine.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

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
    
        // Sound effects
    ofSoundPlayer chime;
    ofSoundPlayer magic;
    
    // Brush size, shape
    float radius;
    bool drawCircle;
    bool drawSquare;
    
    ofColor color;
    
    // All of the shapes to draw
    vector<ofRectangle> rects;
    
    // Storing all of the lines that we draw
    float lineWeight;
    TimedLine line;
    list<TimedLine> lines;
    
    int waitTime;  // the amount of time to wait for a vertex to be deleted
    
    // Store background sfx
    vector<ofSoundPlayer> backgroundSfx;
    
    // Store images to loop through
    vector<ofImage> backgrounds;
    vector<ofImage> foregrounds;
    int currentImg;
    
    // Frame buffers to create alpha mask
    ofFbo maskFbo;
    ofFbo fbo;
    
    // Shader to composite the images together using the mask
    ofShader shader;
    
};
