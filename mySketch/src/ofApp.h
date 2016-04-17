#pragma once

#include "ofMain.h"
#include "SQLiteCpp.h"
#include "EEGPlot.h"

const int FRAME_RATE = 60;
const int EEG_RATE = 5000;
const int WIDTH = 1024;
const int HEIGHT = 768;

class ofApp : public ofBaseApp {

	public:
		void setup();
		void update();
		void draw();

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

    private:
        SQLite::Statement* _query;
        int _eegPerFrame;
        int _eegPerLastFrame;
        bool _queryDone;
        int _x =  WIDTH / -2;

        EEGPlot* _eegPlot;

		
};
