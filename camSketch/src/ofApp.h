#pragma once
#include "ofxNetworkSync.h"

const int FRAME_RATE = 8;
const int WIDTH = 1280;
const int HEIGHT = 960;

#include "ofMain.h"

enum State {
    RUNNING,
    PREPARING
};

class ofApp : public ofBaseApp{

    public:
        void setup();
        void update();
        void draw();

        void exit();

        void keyPressed(int key);
        void keyReleased(int key);
        void mouseMoved(int x, int y );
        void mouseDragged(int x, int y, int button);
        void mousePressed(int x, int y, int button);
        void mouseReleased(int x, int y, int button);
        void mouseEntered(int x, int y);
        void mouseExited(int x, int y);
        void windowResized(int w, int h);
        void dragEvent(ofDragInfo dragInfo);
        void gotMessage(ofMessage msg);

        void onMessageReceived(string & message);
        void onServerFound(IpAndPort & info);

        void reset();

    private:

        ofVideoPlayer _player;

        ofxNetworkSyncServerFinder _finder;
        ofxNetworkSyncClient _client;

        State _appState;
        uint64_t _startTime;
        int _offsetSec;
        
};
