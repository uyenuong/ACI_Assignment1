//
//  shape.hpp
//  audioVisualProj
//
//  Created by Uyen Uong on 10/3/17.
//
//

#ifndef shape_hpp
#define shape_hpp

#include "ofMain.h"

class Shape {
    
public:
    Shape();
    Shape(ofColor c, ofVec3f s, ofVec3f p);
    
    void draw();
    
    ofColor color;
    ofVec3f scale;
    ofVec3f position;
};

#endif /* shape_hpp */
