/*
 iAmmo app
 Jennifer G. Presto
 Humor & Code
 Parsons, Spring 2013
 
 Face detection adapted from ofxFaceTracker empty example
 
 */

#include "testApp.h"

using namespace ofxCv;

//-------------------------------------------------------------------

void testApp::setup() {
	ofSetVerticalSync(true);
	ofSetDrawBitmapMode(OF_BITMAPMODE_SIMPLE); // changed this to simple
	cam.initGrabber(640, 480); // note: how to change aspect ratio
    
    ofSetFrameRate(30);
    
    ofEnableAlphaBlending(); // to maintain target transparency
    
	
	tracker.setup();
	tracker.setRescale(.5);
    
    ofSetLineWidth(1);
    ofSetColor(255);
    ofNoFill();
    
    target.loadImage("target.png");
    
    button.buttonOff.loadImage("aimButtonOpen.png");
    button.buttonOn.loadImage("aimButtonPushed.png");
    
}

//-------------------------------------------------------------------

void testApp::update() {
	cam.update();
	if(cam.isFrameNew()) {
		tracker.update(toCv(cam));
	}

    // declared variable in header file (unlike example)
	faceOutline = tracker.getImageFeature(ofxFaceTracker::FACE_OUTLINE);
    
    // calculate point at center of face using a bounding box
    // the bounding box is an ofRectangle object
    // Note: tried to get centroid of Polyline in code commented below, didn't work
    // faceCenter = faceOutline.getCentroid2D();

    faceRect = faceOutline.getBoundingBox();
    faceCenter = faceRect.getCenter();
    // numbers to scale target to face size
    targetW = faceRect.getWidth();
    targetH = faceRect.getHeight();
    
}

//-------------------------------------------------------------------

void testApp::draw() {
	cam.draw(0, 0);
	ofDrawBitmapString(ofToString((int) ofGetFrameRate()), 10, 20);
//	faceOutline.draw();
//    ofRect(faceRect);
//    ofCircle(faceCenter.x, faceCenter.y, 10);
    target.draw(faceCenter.x-targetW*0.5, faceCenter.y-targetH*0.5, targetW, targetW);
}

//-------------------------------------------------------------------

void testApp::keyPressed(int key) {
	if(key == 'r') {
		tracker.reset();
	}
}