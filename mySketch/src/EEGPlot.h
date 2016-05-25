#pragma once
#include "ofMain.h"
#include "ofxHistoryPlot.h"
#include "ofApp.h"

struct Channel {
    ofxHistoryPlot* plot;
    int x;
    int y;
};

class ofApp;

class EEGPlot {
    public:
        void setup(ofApp* app);
        void update(int index, float value);
        void draw(int x, int y, int width, int height);
        void appendChannel(std::string name);
        void reset();

        ofMatrix4x4 getChannelMeshMatrix(int index, int x, int y, int width, int height);

        ofVboMesh* getChannelPlotMesh(int index);

        vector<Channel> channels;

        bool mouseIsPressed;
        float currentFrameRate;

    private:
        ofApp* _app;
};
