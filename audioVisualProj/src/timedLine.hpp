//
//  timedLine.hpp
//  audioVisualProj
//
//  Created by Uyen Uong on 10/12/17.
//
//

#ifndef timedLine_hpp
#define timedLine_hpp

#include <stdio.h>
#include "ofMain.h"
#include "ofxFatLine.h"

class TimedLine  {
public:
    TimedLine();
    TimedLine(float weight);
    TimedLine(const vector<ofPoint>& pts, float weight);
    TimedLine(const TimedLine& t);
    
    // ACCESSORS
    void draw();
    unsigned long size() const { return line.size(); }
    vector<ofPoint> getVertices() const { return line.getVertices(); }
    float getWeight() const { return lineWeight; }
    
    // MODIFIERS
    void addVertex(float x, float y);
    void addVertex(const ofPoint& p);
    int stripVertices(int sec);
    void clear() { line.clear(); timeCreated.clear(); }
    void changeWeight(float newWeight);
    
    
private:
    ofxFatLine line;
    vector<float> timeCreated;  // stores the time that each vertex in the line was created
    float lineWeight;
};

#endif /* timedLine_hpp */
