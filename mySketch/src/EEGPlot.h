#pragma once
#include "ofMain.h"
#include "ofxHistoryPlot.h"

class EEGPlot {
    public:
        void setup();
        void update(float value);
        void draw();

        ofxHistoryPlot * plot; //manual

        bool mouseIsPressed;
        float currentFrameRate;
};
