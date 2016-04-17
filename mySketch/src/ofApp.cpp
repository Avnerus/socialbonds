#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

    ofEnableAlphaBlending();
    ofEnableSmoothing();
    ofSetBackgroundColor(0,0,0);
    ofSetColor(255,255,255); 
    ofSetBackgroundAuto(false);
    ofSetVerticalSync(true);

    ofSetFrameRate(FRAME_RATE);

    ofSetLogLevel(OF_LOG_VERBOSE);

    _eegPerFrame = EEG_RATE / FRAME_RATE;
    _eegPerLastFrame = EEG_RATE / FRAME_RATE + (EEG_RATE % FRAME_RATE);

    std::cout << "EEG Samples per frame: " << _eegPerFrame << ". Last frame: " << _eegPerLastFrame << endl;

    try {

        // Open EEG Dataset
        std::string eegDB = ofToDataPath("eegdata.db", true);

        SQLite::Database db(eegDB);

        _query = new SQLite::Statement(db, "SELECT * from data");
        _queryDone = false;

    }

    catch (std::exception& e) {
        ofLogError() << "exception: " << e.what();
    }

    _eegPlot = new EEGPlot();
    _eegPlot->setup();


    _nowRecording = false;

    if(_nowRecording) {
        
        _rgbFbo.allocate(WIDTH, HEIGHT, GL_RGB32F_ARB); 
        _rgbFbo.begin();
            ofClear(0,0,0);
        _rgbFbo.end();
        _pix.allocate(WIDTH,HEIGHT,OF_PIXELS_RGB);
        
        
        ofAddListener(vidRecorder.outputFileCompleteEvent, this, &ofApp::recordingComplete);

        //vidRecorder.setup("testeeg.mov", WIDTH, HEIGHT, 30);
            
        // Start recording
        //vidRecorder.start();
    }

}

//--------------------------------------------------------------
void ofApp::update(){
    /*

    if(_nowRecording){
        _rgbFbo.begin();
        // Check if the video recorder encountered any error while writing video frame or audio smaples.
        if (vidRecorder.hasVideoError()) {
            ofLogWarning("The video recorder failed to write some frames!");
        }
        
        if (vidRecorder.hasAudioError()) {
            ofLogWarning("The video recorder failed to write some audio samples!");
        }
    }*/
    

    std::stringstream strm;
	strm << "fps: " << ofGetFrameRate();
	ofSetWindowTitle(strm.str());

    for (int i = 0; i < _eegPerFrame && !_queryDone; i++) {
        if (_query->executeStep()) {
            double value = _query->getColumn(1);
            value *= 5000.0;
            _eegPlot->update(value);
        } else {
            _queryDone = true;
        }
    }

}

//--------------------------------------------------------------
void ofApp::draw(){


    _eegPlot->draw();
/*
    if (_nowRecording) {
        _rgbFbo.end();
        _rgbFbo.draw(0,0);
        _rgbFbo.readToPixels(_pix);
        vidRecorder.addFrame(_pix);
    }*/
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    vidRecorder.close();
    _nowRecording = false;

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    _eegPlot->mouseIsPressed = true;
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    _eegPlot->mouseIsPressed = false;
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}

void ofApp::exit(){
    std::cout << "EXIT" << std::endl;
    ofRemoveListener(vidRecorder.outputFileCompleteEvent, this, &ofApp::recordingComplete);
    vidRecorder.close();
}

void ofApp::recordingComplete(ofxVideoRecorderOutputFileCompleteEventArgs& args){
    std::cout << "The recoded video file is now complete." << std::endl;
}
