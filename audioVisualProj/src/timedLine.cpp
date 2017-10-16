//
//  timedLine.cpp
//  audioVisualProj
//
//  Created by Uyen Uong on 10/12/17.
//
//

#include "timedLine.hpp"

TimedLine::TimedLine(const vector<ofPoint>& pts, float weight) {
    lineWeight = weight;
    
    for (int i = 0; i < pts.size(); i++) {
        line.add(pts[i], ofColor::white, lineWeight);
        timeCreated.push_back(ofGetElapsedTimef());
    }
}

TimedLine::TimedLine(const TimedLine& t) {
    line = t.line;
    timeCreated = t.timeCreated;
    lineWeight = t.lineWeight;
}

void TimedLine::draw() {
    line.draw();
}

void TimedLine::addVertex(float x, float y) {
    ofPoint pt;
    pt.set(x,y);
    line.add(pt, ofColor::white, lineWeight);
    timeCreated.push_back(ofGetElapsedTimef());
}

void TimedLine::addVertex(const ofPoint& p) {
    line.add(p, ofColor::white, lineWeight);
    timeCreated.push_back(ofGetElapsedTimef());
}

// Gets rid of the vertices in the point that have been alive
// longer than the given wait time
// Returns the number of vertices that were removed
int TimedLine::stripVertices(int waitTime) {
    ofxFatLine tmpLine;
    vector<float> tmpTimes;
    
    ofLog() << "lineweight: " << lineWeight << endl;
    
    // Copy the vertices that will not be removed
    for (int i = 0; i < timeCreated.size(); i++) {
        if (ofGetElapsedTimef() - timeCreated[i] < waitTime) {
            tmpLine.add(line.getVertices()[i], ofColor::white, lineWeight);
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

// Changes the weight of the line, need to change for all vertices in line
void TimedLine::changeWeight(float newWeight) {
    for (int i = 0; i < line.size(); i++) {
        line.updateWeight(i, newWeight);
    }
    line.update();
}
