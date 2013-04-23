//
//  aimButton.h
//  GunApp
//
//  Created by Jennifer Presto on 4/23/13.
//
//

#ifndef __GunApp__aimButton__
#define __GunApp__aimButton__

#include <iostream>
#include "ofMain.h"

class aimButton{

public:
    
    // variables
    float xPos;
    float yPos;
    float w;
    float h;
    
    ofImage buttonOff;
    ofImage buttonOn;
    
    // method
    void display();
    
    // constructor
    aimButton(bool pushed);
};

#endif /* defined(__GunApp__aimButton__) */
