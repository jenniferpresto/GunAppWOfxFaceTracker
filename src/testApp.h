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
#include "aimButton.h"

class testApp : public ofBaseApp {
public:
	void setup();
	void update();
	void draw();
	void keyPressed(int key);
    void mousePressed(int x, int y, int button);

	// variables for cam and faceTracker
	ofVideoGrabber cam;
	ofxFaceTracker tracker;
    
    // variables created by faceTracker
    ofPolyline faceOutline;
    ofRectangle faceRect;
    ofPoint faceCenter;
    
    // variables controlling on-screen target
    ofImage target;
    float targetW;
    float targetH;
    
    // button to push to start measurement
    aimButton pushToAim;
    
    // variables regarding actual measurement
    void collectPoints();
    bool measuring;
    bool calculating;
    void measureAim();
    int pushTime; // time button is pushed
    vector<float> targetX; // all target x-positions during the measured time
    vector<float> targetY; // all target y-positions during the measured time
};
