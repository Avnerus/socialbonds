#pragma once
#include "ofxNetworkSync.h"
#include "ofxJSON.h"

const int FRAME_RATE = 8;

#include "ofMain.h"

enum State {
    RUNNING,
    PREPARING
};

class ofApp : public ofBaseApp{

    public:
        int WIDTH = 1280;
        int HEIGHT = 960;
        int FULLSCREEN = 0;

        ofApp();

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

        ofxJSONElement _config;

        ofVideoPlayer _player;

        ofxNetworkSyncServerFinder _finder;
        ofxNetworkSyncClient _client;

        State _appState;
        uint64_t _startTime;
        int _offsetSec;
        
};
