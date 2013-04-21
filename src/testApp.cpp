#include "testApp.h"

using namespace ofxCv;

void testApp::setup() {
	ofSetVerticalSync(true);
	ofSetDrawBitmapMode(OF_BITMAPMODE_SIMPLE); // changed this to simple
	cam.initGrabber(640, 480); // changed screen size for phone (changed back; squeezed picture)
	
	tracker.setup();
	tracker.setRescale(.5);
    
    ofSetLineWidth(1);
    ofSetColor(255);
    ofNoFill();
}

void testApp::update() {
	cam.update();
	if(cam.isFrameNew()) {
		tracker.update(toCv(cam));
	}

    // declared variable in header file (unlike example)
	faceOutline = tracker.getImageFeature(ofxFaceTracker::FACE_OUTLINE);
    
    // calculate point at center of face using a bounding box
    // the bounding box is an ofRectangle object
    faceRect = faceOutline.getBoundingBox();
    faceCenter = faceRect.getCenter();


}

void testApp::draw() {
	cam.draw(0, 0);
	ofDrawBitmapString(ofToString((int) ofGetFrameRate()), 10, 20);
	faceOutline.draw();
    ofRect(faceRect);
    
}

void testApp::keyPressed(int key) {
	if(key == 'r') {
		tracker.reset();
	}
}