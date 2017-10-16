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

class TimedLine  {
public:
    TimedLine();
    TimedLine(const vector<ofPoint>& pts);
    TimedLine(const TimedLine& t);
    
    // ACCESSORS
    void draw() const;
    unsigned long size() const { return line.size(); }
    vector<ofPoint> getVertices() const { return line.getVertices(); }
    
    // MODIFIERS
    void addVertex(float x, float y);
    void addVertex(const ofPoint& p);
    int stripVertices(int sec);
    void clear() { line.clear(); }
    
    
private:
    ofPolyline line;
    vector<float> timeCreated;  // stores the time that each vertex in the line was created
//    int waitTime;  // the amount of time to wait for a vertex to be deleted
};

#endif /* timedLine_hpp */
