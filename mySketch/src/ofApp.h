#pragma once

#include "ofMain.h"
#include "SQLiteCpp.h"
#include "EEGPlot.h"
#include "EEGSound.h"
#include "EEGMarker.h"
#include "ofxVideoRecorder.h"
#include "ofxNetworkSync.h"
#include "ofxJSON.h"

class EEGMarker;
class EEGPlot;

enum State {
    RUNNING,
    PREPARING
};


class ofApp : public ofBaseApp {

	public:
        const int FRAME_RATE = 25;
        const int EEG_RATE = 500;
        const int EEG_CHANNELS = 9;
        //const int WIDTH = 5120;
        //const int HEIGHT = 2880;
        int SERVER_PORT = 9540;
        int START_OFFSET_SEC = 10;
        int RESTART_TIME_SEC = 3005;

        int PLOT_MARGIN = 20;
        int PLOT_RIGHT_MARGIN = 50;

        int MONEY_FONT_SIZE = 36;
        int PLOT_FONT_SIZE = 20;
        
        int WIDTH = 1920;
        int HEIGHT = 1080;

        int FULLSCREEN = 0;
        
        const int REC_SAMPLE_RATE  = 44100;
        const int REC_CHANNELS = 2;

        //const float LPP_THRESHOLD = 0.000710264; POUYAN
        //const float LPP_THRESHOLD = 0.00096785915213;
        double LPP_THRESHOLD = 0.00029921558638;

        //const float LPP_THRESHOLD = 0.000564972;
        //

        ofApp();

		void setup();
		void update();
		void draw();
        void exit();

        void reset();
        void restart();

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

        void recordingComplete(ofxVideoRecorderOutputFileCompleteEventArgs& args);

        int getNumberOfSamplesPerFrame();
        ofTrueTypeFont* getFont();

        EEGPlot* getEEGPlot();

        ofCamera* getCamera();
        ofxNetworkSyncServer _server;
        ofxJSONElement _config;

    private:
        std::shared_ptr<SQLite::Statement> _query;
        SQLite::Database* _database;
        int _eegPerFrame;
        int _eegPerLastFrame;
        bool _queryDone;
        int _x =  WIDTH / -2;

        int _eegRestartFrame;

        EEGPlot* _eegPlot;
        EEGSound* _eegSound;

        std::vector< std::shared_ptr<EEGMarker> > _markers;
        std::shared_ptr<EEGMarker> _liveMarker;

        ofxVideoRecorder vidRecorder;
        ofFbo _rgbFbo;
        ofPixels _pix;

        ofEasyCam _cam;
        ofTrueTypeFont _font;

        float _eyeX;
        float _eyeY;

        bool _nowRecording;

        bool _blink;

        State _appState;
        uint64_t _startTime;
};
