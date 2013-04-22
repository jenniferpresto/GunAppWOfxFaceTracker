/*
 iAmmo app
 Jennifer G. Presto
 Humor & Code
 Parsons, Spring 2013
 
 Face detection adapted from ofxFaceTracker empty example
 
*/

#pragma once

#include "ofMain.h"
#include "ofxCv.h"

#include "ofxFaceTracker.h"

class testApp : public ofBaseApp {
public:
	void setup();
	void update();
	void draw();
	void keyPressed(int key);
	
	ofVideoGrabber cam;
	ofxFaceTracker tracker;
    
    ofPolyline faceOutline;
    ofRectangle faceRect;
    ofPoint faceCenter;
    
    ofImage target;
    float targetW;
    float targetH;
    
    ofImage aimButton;
};
