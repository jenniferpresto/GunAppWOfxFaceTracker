/*
 iAmmo app
 Jennifer G. Presto
 Humor & Code
 Parsons, Spring 2013
 
 Application measures the steadiness of your aim,
 then suggests how many bullets you should have in
 your magazine to make sure you can hit your mark.
 
 Face detection adapted from ofxFaceTracker empty example.
 ofxCenteredTrueTypeFont subclass is by armadillu, from oF addons.
 
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
    
    helvetica.loadFont("helvetica.otf", 12);
	
	tracker.setup();
	tracker.setRescale(.5);
    
    ofSetLineWidth(1);
    ofSetColor(255);
    ofNoFill();
    
    target.loadImage("target.png");
    bullet.loadImage("bullet.png");
    
    appState = 1;
    
    pushTime = 0;
    measuring = false;
    firstMeasure = true;
    
    pushToAim.buttonOff.loadImage("aimButtonOpen.png");
    pushToAim.buttonOn.loadImage("aimButtonPushed.png");
    
    pushToAim.xPos = ofGetWidth() * 0.5;
    pushToAim.yPos = ofGetHeight() * 0.85;
    pushToAim.w = 50;
    pushToAim.h = 50;
    
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
    
    if(!ofGetMousePressed()) pushToAim.pushed = false;
    
}

//-------------------------------------------------------------------

void testApp::draw() {
    cam.draw(0,0);
    // appState 1: Measuring shakiness of aim
    if(appState == 1){
        // ofDrawBitmapString(ofToString((int) ofGetFrameRate()), 10, 20);
        pushToAim.display();
        if(!measuring){
            // faceOutline.draw();
            ofRect(faceRect);
            // ofCircle(faceCenter.x, faceCenter.y, 10);
            helvetica.drawStringCentered("Push the button.", ofGetWidth()*0.5, 10);
            helvetica.drawStringCentered("The steadiness of your aim will", ofGetWidth()*0.5, 30);
            helvetica.drawStringCentered("be measured for three seconds.", ofGetWidth()*0.5, 50);
            
        }
        
        if(measuring){
            target.draw(faceCenter.x-targetW*0.5, faceCenter.y-targetH*0.5, targetW, targetW);
            // ofCircle(pushToAim.xPos, pushToAim.yPos, 10);
        }
        collectPoints();
        measureAim();
    }
    
    
    // appState 2: reporting results
    if(appState == 2){
        recommendation = "Bullets recommended: " + ofToString(numBullets);
        helvetica.drawStringCentered(recommendation, ofGetWidth() * 0.5, 50);
        helvetica.drawStringCentered("Click anywhere to test again:", ofGetWidth() * 0.5, 80);
        if(firstMeasure){
            for (int i = 0; i < numBullets; i++){
                magazine.push_back(bullet);
                firstMeasure = false;
            }
        }
        
        for (int i = 0; i < magazine.size(); i++){
            if(i<5){
                magazine[i].draw(ofGetWidth() - 50, ofGetHeight() - 18 - (i * 18));
            }
            if(i >= 5 && i < 10){
                magazine[i].draw(ofGetWidth() - 100, ofGetHeight() - 18 - ((i-5) * 18));
            }
            if(i >= 10 && i < 15){
                magazine[i].draw(ofGetWidth() - 150, ofGetHeight() - 18 - ((i-10) * 18));
            }
            if(i >= 15 && i < 20){
                magazine[i].draw(ofGetWidth() - 200, ofGetHeight() - 18 - ((i-15) * 18));
            }
            if(i >= 20 && i < 25){
                magazine[i].draw(ofGetWidth() - 250, ofGetHeight() - 18 - ((i-20) * 18));
            }
            if(i >= 25 && i < 30){
                magazine[i].draw(ofGetWidth() - 300, ofGetHeight() - 18 - ((i-25) * 18));
            }
            if(i >= 30 && i < 35){
                magazine[i].draw(ofGetWidth() - 350, ofGetHeight() - 18 - ((i-30) * 18));
            }
            if(i >= 35){
                magazine[i].draw(ofGetWidth() - 400, ofGetHeight() - 18 - ((i-35) * 18));
            }
        }
    }
}

//-------------------------------------------------------------------

void testApp::keyPressed(int key) {
	if(key == 'r') {
		tracker.reset();
	}
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
    if (appState == 1){
        if(x > pushToAim.xPos - pushToAim.w * 0.5 && x < pushToAim.xPos + pushToAim.w * 0.5 && y > pushToAim.yPos - pushToAim.h * 0.5 && y < pushToAim.yPos + pushToAim.h * 0.5){
            pushToAim.pushed = true;
            if(!measuring){
                // start time marker so measure for 3 seconds
                pushTime = ofGetElapsedTimeMillis();
                measuring = true;
            }
            
        } else {
            pushToAim.pushed = false;
        }
    }
    
    if(appState == 2){
        magazine.erase(magazine.begin(), magazine.end());
        firstMeasure = true;
        appState = 1;
    }
}

//--------------------------------------------------------------
void testApp::collectPoints(){
    if (cam.isFrameNew() && ofGetElapsedTimeMillis()-pushTime < 3000 && measuring){
        
        // for 3 seconds, get position of target and
        // add x- and y-positions from each frame to each vector
        targetX.push_back(faceCenter.x);
        targetY.push_back(faceCenter.y);
    }
    
    // stop measuring points after 3 seconds,
    // set boolean to start calculating the average
    if(ofGetElapsedTimeMillis()-pushTime > 3000 && measuring){
        measuring = false;
        calculating = true;
    }
}

//--------------------------------------------------------------
void testApp::measureAim(){
    if(calculating){
        float avgDist = 0;
        for(int i=0; i<targetX.size()-1; i++){
            avgDist += ofDist(targetX[i], targetY[i], targetX[i+1], targetY[i+1]);
            cout << "calculating avgDist: " << avgDist << endl;
        }
        cout << "total distance = " << avgDist << endl;
        cout << "number of points calculated = " << targetX.size() << endl;
        
        avgDist /= targetX.size()-1;
        
        numBullets = (int)MAX(1, avgDist); // make sure it recommends at least one bullet
        
        // debugging
        cout << "average distance = " << avgDist << endl;
        cout << "number of recommended bullets = " << numBullets << endl;
        
        calculating = false;
        
        targetX.erase(targetX.begin(), targetX.end());
        targetY.erase(targetY.begin(), targetY.end());
        
        appState = 2;
    }
}