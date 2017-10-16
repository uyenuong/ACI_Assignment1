//
//  shape.cpp
//  audioVisualProj
//
//  Created by Uyen Uong on 10/3/17.
//
//

#include "shape.hpp"

Shape::Shape() {
    Shape(ofColor::azure,
             ofVec3f(1,1,1),
             ofVec3f(ofGetWidth()/2, ofGetHeight()/2));
}

Shape::Shape(ofColor c, ofVec3f s, ofVec3f p) {
    color = c;
    scale = s;
    position = p;
}

void Shape::draw() {
    ofTranslate(position);
    //rotate
    ofSetColor(color);
}
