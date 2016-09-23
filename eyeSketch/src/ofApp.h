#pragma once
#include "SQLiteCpp.h"
#include "ofxNetworkSync.h"
#include "ofxJSON.h"


#include "ofMain.h"

const int FRAME_RATE = 25;
const int EYE_RATE = 500;

enum State {
    RUNNING,
    PREPARING
};

class ofApp : public ofBaseApp{

	public:
        int WIDTH = 1920;
        int HEIGHT = 1200;
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
        ofFbo _rgbFbo;
        ofPixels _pix;

        bool _queryDone;
        std::shared_ptr<SQLite::Statement> _query;
        SQLite::Database* _eyeDB;
        int _eyePerFrame;
        int _eyePerLastFrame;

        double _eyeX;
        double _eyeY;
        int _firstOnset;

        ofxNetworkSyncServerFinder _finder;
        ofxNetworkSyncClient _client;

        float _xScale;
        float _yScale;

        State _appState;
        uint64_t _startTime;
        int _offsetSec;
		
};
