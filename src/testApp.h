#pragma once

#include "ofMain.h"
#include "ofxCv.h"
using namespace ofxCv;
using namespace cv;

#include "ofxFaceTracker.h"

class testApp : public ofBaseApp {
public:
	void setup();
	void update();
	void draw();
	void keyPressed(int key);
	
    ofVideoGrabber cam;
    ofxFaceTracker tracker;
    ofxFaceTracker imgTracker;
    ofImage faceImage;
    ofImage faceImages[4];
    
    ofEasyCam easyCam;
    ofSoundPlayer inaiinai;
    ofSoundPlayer baa;
    Boolean inaiinaiFlag = true;
    Boolean baaFlag = false;
};
