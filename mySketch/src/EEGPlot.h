#pragma once
#include "ofMain.h"
#include "ofxHistoryPlot.h"

struct Channel {
    ofxHistoryPlot* plot;
    int x;
    int y;
};

class EEGPlot {
    public:
        void setup();
        void update(int index, float value);
        void draw();
        void appendChannel(int x, int y);

        vector<Channel> channels;

        bool mouseIsPressed;
        float currentFrameRate;
};
