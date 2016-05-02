#pragma once
#include "SQLiteCpp.h"

const int FRAME_RATE = 60;
const int EYE_RATE = 500;
const int WIDTH = 1920;
const int HEIGHT = 1200;

#include "ofMain.h"

class ofApp : public ofBaseApp{

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
        ofVideoPlayer _player;
        bool _queryDone;
        SQLite::Statement* _query;
        SQLite::Database* _eyeDB;
        int _eyePerFrame;
        int _eyePerLastFrame;

        double _eyeX;
        double _eyeY;
		
};
