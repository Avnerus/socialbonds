//
//  EEGSound.h
//  mySketch
//
//  Created by Avner Peled on 5/3/16.
//
//

#include "ofMain.h"
#include "ofxOsc.h"

#pragma once


class EEGSound {
    
public:
    void setup();
    void update(int channel, float value);
    
private:
    ofxOscSender _sender;
    ofxOscMessage _message;
};

    
