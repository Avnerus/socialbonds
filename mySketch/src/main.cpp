#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main( ){

    ofApp* myApp = new ofApp();
	ofSetupOpenGL(myApp->WIDTH,myApp->HEIGHT,(ofWindowMode)myApp->FULLSCREEN);			// <-------- setup the GL context

	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp(myApp);

}
