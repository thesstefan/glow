#pragma once

#include "ofMain.h"
#include "ofImage.h"

class ofApp : public ofBaseApp {
    ofImage source;
    ofImage result;

    public:
    	void setup();
	void draw();
};
