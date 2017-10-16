//
//  timedLine.cpp
//  audioVisualProj
//
//  Created by Uyen Uong on 10/12/17.
//
//

#include "timedLine.hpp"

TimedLine::TimedLine() {
}

TimedLine::TimedLine(const vector<ofPoint>& pts) {
    for (int i = 0; i < pts.size(); i++) {
        line.addVertex(pts[i]);
        timeCreated.push_back(ofGetElapsedTimef());
    }
}

TimedLine::TimedLine(const TimedLine& t) {
    line = t.line;
    timeCreated = t.timeCreated;
}

void TimedLine::draw() const {
    line.draw();
}

void TimedLine::addVertex(float x, float y) {
    line.addVertex(x,y);
    timeCreated.push_back(ofGetElapsedTimef());
}

void TimedLine::addVertex(const ofPoint& p) {
    line.addVertex(p);
    timeCreated.push_back(ofGetElapsedTimef());
}

// Gets rid of the vertices in the point that have been alive
// longer than the given wait time
// Returns the number of vertices that were removed
int TimedLine::stripVertices(int waitTime) {
    ofPolyline tmpLine;
    vector<float> tmpTimes;
    
    // Copy the vertices that will not be removed
    for (int i = 0; i < timeCreated.size(); i++) {
        if (ofGetElapsedTimef() - timeCreated[i] < waitTime) {
            tmpLine.addVertex(line.getVertices()[i]);
            tmpTimes.push_back(timeCreated[i]);
            
        }
    }
    
    int numDeleted = (int)line.size() - (int)tmpLine.size();
    if (numDeleted > 0) {
        line = tmpLine;
        timeCreated = tmpTimes;
        waitTime += 0.5;
    }
    
    return numDeleted;
}

