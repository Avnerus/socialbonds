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
    EEGMarker();
    ~EEGMarker();

    void setup(ofApp* app, double lpp);
    void draw();
    void update(double lpp);

    bool finished;
    
private:
    ofApp* _app;
    float _x;
    float _width;
    int _firstVertexIndex;
    int _lastVertexIndex;
    float _lastX;
    ofMatrix4x4 _meshMatrix;
    double _money;
    float _labelHeight;
    int _labelAlpha;

    ofColor _markerColor;
    ofColor _markerFilling;
};

    
