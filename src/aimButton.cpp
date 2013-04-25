//
//  aimButton.cpp
//  GunApp
//
//  Created by Jennifer Presto on 4/23/13.
//
//

#include "aimButton.h"

// ----------------------------------
// constructor
aimButton::aimButton(bool _pushed) {
    xPos = 0;
    yPos = 0;
    w = 0;
    h = 0;
    pushed = _pushed;
}

// -----------------------------------
void aimButton::display(){
    if(!pushed){
        buttonOff.draw(xPos-w*0.5, yPos-h*0.5, w, h);
    }
    if(pushed){
        buttonOn.draw(xPos-w*0.5, yPos-h*0.5, w, h);
    }
}