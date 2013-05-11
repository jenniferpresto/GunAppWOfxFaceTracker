/*
 iAmmo app
 Jennifer G. Presto
 Humor & Code
 Parsons, Spring 2013
 
 Application measures the steadiness of your aim,
 then suggests how many bullets you should have in
 your magazine to make sure you can hit your mark.
 
 Face detection adapted from ofxFaceTracker empty example,
 available here:
 https://github.com/kylemcdonald/ofxFaceTracker
 
 ofxCenteredTrueTypeFont subclass is by armadillu, from oF addons,
 available here:
 https://github.com/armadillu/ofxCenteredTrueTypeFont
 
 Bullet images by The Noun Project Collection; target adapted from
 design by Laurent Patain from The Noun Project.
 
 */

#include "testApp.h"

using namespace ofxCv;

//-------------------------------------------------------------------

void testApp::setup() {
	ofSetVerticalSync(true);
	ofSetDrawBitmapMode(OF_BITMAPMODE_SIMPLE); // changed this to simple
	cam.initGrabber(640, 480); // note: inquire about changing aspect ratio
    
    ofSetFrameRate(30);
    
    ofEnableAlphaBlending(); // to maintain target transparency
    
    helvetica.loadFont("helvetica.otf", 12);
    
    titlePage.loadImage("iAmmoTitlePage.jpg");
	
	tracker.setup();
	tracker.setRescale(.5);
    
    ofSetLineWidth(1);
    ofSetColor(255);
    ofNoFill();
    
    target.loadImage("target.png");
    bullet.loadImage("bullet.png");
    
    appState = 0;
    
    pushTime = 0;
    measuring = false; // whether calculating steadiness
    
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
    cam.draw(-cam.width*0.25,0);
    // appState 0: Title page
    if(appState == 0){
        titlePage.draw(0, 0, 320, 480);
    }
    
    // appState 1: Measuring shakiness of aim
    if(appState == 1){
        // ofDrawBitmapString(ofToString((int) ofGetFrameRate()), 10, 20);
        pushToAim.display();
        if(!measuring){
            ofPushMatrix();
            ofTranslate(-cam.width*0.25, 0);
            ofRect(faceRect);
            ofPopMatrix();
            ofSetColor(0);
            helvetica.drawStringCentered("Push the button.", ofGetWidth()*0.5 - 1, 11);
            helvetica.drawStringCentered("The steadiness of your aim will", ofGetWidth()*0.5 - 1, 31);
            helvetica.drawStringCentered("be measured for three seconds.", ofGetWidth()*0.5 - 1, 51);
            ofSetColor(255);
            helvetica.drawStringCentered("Push the button.", ofGetWidth()*0.5, 10);
            helvetica.drawStringCentered("The steadiness of your aim will", ofGetWidth()*0.5, 30);
            helvetica.drawStringCentered("be measured for three seconds.", ofGetWidth()*0.5, 50);
        }
        
        if(measuring){
            ofPushMatrix();
            ofTranslate(-cam.width*0.25, 0);
            target.draw(faceCenter.x-targetW*0.5, faceCenter.y-targetH*0.5, targetW, targetW);
            ofPopMatrix();
        }
        collectPoints();
        measureAim();
    }
    
    
    // appState 2: reporting results
    if(appState == 2){
        recommendation = "Bullets recommended: " + ofToString(numBullets);
        ofSetColor(0);
        helvetica.drawStringCentered(recommendation, ofGetWidth() * 0.5-1, 51);
        helvetica.drawStringCentered("Click anywhere to test again.", ofGetWidth() * 0.5 - 1, 81);
        ofSetColor(255);
        helvetica.drawStringCentered(recommendation, ofGetWidth() * 0.5, 50);
        helvetica.drawStringCentered("Click anywhere to test again.", ofGetWidth() * 0.5, 80);

        
        for (int i = 0; i < numBullets; i++){
            if(i<5){
                bullet.draw(ofGetWidth() - 60, ofGetHeight() - 18 - (i * 18));
            }
            if(i >= 5 && i < 10){
                bullet.draw(ofGetWidth() - 110, ofGetHeight() - 18 - ((i-5) * 18));
            }
            if(i >= 10 && i < 15){
                bullet.draw(ofGetWidth() - 160, ofGetHeight() - 18 - ((i-10) * 18));
            }
            if(i >= 15 && i < 20){
                bullet.draw(ofGetWidth() - 210, ofGetHeight() - 18 - ((i-15) * 18));
            }
            if(i >= 20 && i < 25){
                bullet.draw(ofGetWidth() - 260, ofGetHeight() - 18 - ((i-20) * 18));
            }
            if(i >= 25){
                bullet.draw(ofGetWidth() - 310, ofGetHeight() - 18 - ((i-25) * 18));
            }
        }
    }
}


//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
    if (appState == 0) {
        appState = 1;
    }
    
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
        tracker.reset();
        appState = 1;
    }
}

//--------------------------------------------------------------
int testApp::round(float number){
    return number < 0.0 ? ceil(number - 0.5) : floor(number + 0.5);
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
        
        avgDist /= (targetX.size()-1);
        int roundedAvg = round(avgDist);
        
        numBullets = MAX(1, roundedAvg); // make sure it recommends at least one bullet
        numBullets = MIN(numBullets, 30); // recommending more than 30 is just silly
        // (according to Wikipedia, six states limit magazine capacity, ranging from 7 to 20 rounds)
        // see here: http://en.wikipedia.org/wiki/Magazine_(firearms)
        
        // debugging
        cout << "average distance = " << avgDist << endl;
        cout << "number of recommended bullets = " << numBullets << endl;
        
        calculating = false;
        
        targetX.erase(targetX.begin(), targetX.end());
        targetY.erase(targetY.begin(), targetY.end());
        
        appState = 2;
    }
}