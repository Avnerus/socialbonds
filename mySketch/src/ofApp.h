#pragma once

#include "ofMain.h"
#include "SQLiteCpp.h"
#include "EEGPlot.h"
#include "EEGSound.h"

#include "ofxVideoRecorder.h"

const int FRAME_RATE = 60;
const int EEG_RATE = 500;
const int EEG_CHANNELS = 10;
const int WIDTH = 1920;
const int HEIGHT = 1080;
const int REC_SAMPLE_RATE  = 44100;
const int REC_CHANNELS = 2;


class ofApp : public ofBaseApp {

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

        void recordingComplete(ofxVideoRecorderOutputFileCompleteEventArgs& args);

    private:
        SQLite::Statement* _query;
        SQLite::Database* _database;
        int _eegPerFrame;
        int _eegPerLastFrame;
        bool _queryDone;
        int _x =  WIDTH / -2;

        EEGPlot* _eegPlot;
        EEGSound* _eegSound;

        ofxVideoRecorder vidRecorder;
        ofFbo _rgbFbo;
        ofPixels _pix;

        float _eyeX;
        float _eyeY;

        bool _nowRecording;
};
