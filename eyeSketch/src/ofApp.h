#pragma once
#include "SQLiteCpp.h"
#include "ofxVideoRecorder.h"

const int FRAME_RATE = 25;
const int EYE_RATE = 500;
const int WIDTH = 1920;
const int HEIGHT = 1200;

#include "ofMain.h"

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

        void recordingComplete(ofxVideoRecorderOutputFileCompleteEventArgs& args);

    private:
        ofVideoPlayer _player;
        ofxVideoRecorder vidRecorder;
        bool _nowRecording;
        ofFbo _rgbFbo;
        ofPixels _pix;

        bool _queryDone;
        SQLite::Statement* _query;
        SQLite::Database* _eyeDB;
        int _eyePerFrame;
        int _eyePerLastFrame;

        double _eyeX;
        double _eyeY;
		
};
