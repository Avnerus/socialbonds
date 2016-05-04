//
//  EEGSound.h
//  mySketch
//
//  Created by Avner Peled on 5/3/16.
//
//
#pragma once
#include "ofMain.h"
#include "ofApp.h"

class ofApp;

class EEGMarker {
    
public:
    void setup(ofApp* app);
    void draw();
    void update();
    
private:
    ofApp* _app;
    int _x;
    int _width;
};

    
