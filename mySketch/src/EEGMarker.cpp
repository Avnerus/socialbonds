//
//  EEGSound.cpp
//  mySketch
//
//  Created by Avner Peled on 5/3/16.
//
//

#include <EEGMarker.h>


void EEGMarker::setup(ofApp* app) {
    _app = app;
    _x = 517;
    _width = 0;
}

void EEGMarker::draw() {
    ofSetColor(0,255,0);
    ofDrawRectangle(_x,0, 2, _app->HEIGHT);
    ofSetColor(0,255,0,50);
    ofDrawRectangle(_x + 2,0, _width, _app->HEIGHT);
}

void EEGMarker::update() {
    _width += 17;
}

