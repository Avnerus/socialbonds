#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

    ofEnableAlphaBlending();
    ofEnableSmoothing();
    ofSetBackgroundColor(0,0,0);
    //ofSetBackgroundAuto(false);
    ofSetVerticalSync(true);

    ofSetFrameRate(FRAME_RATE);
    
    std::string eyeDBPath  = ofToDataPath("matti_eyedata.db", true);

    try {

        _eyeDB = new SQLite::Database(eyeDBPath);
        
        // Open EyeLink Dataset
        // std::string eyeDB  = ofToDataPath("eyedata.db", true);


        _query = new SQLite::Statement(*(_eyeDB), "SELECT * from data");
        _queryDone = false;

    }

    catch (std::exception& e) {
        ofLogError() << "exception: " << e.what();
    }

    _eyePerFrame = EYE_RATE / FRAME_RATE;
    _eyePerLastFrame = EYE_RATE / FRAME_RATE + (EYE_RATE % FRAME_RATE);

    std::cout << "EYE Samples per frame: " << _eyePerFrame << ". Last frame: " << _eyePerLastFrame << endl;

    _nowRecording = false;

    if(_nowRecording) {
        
        _rgbFbo.allocate(WIDTH, HEIGHT, GL_RGB32F_ARB); 
        _rgbFbo.begin();
            ofClear(0,0,0);
        _rgbFbo.end();
        _pix.allocate(WIDTH,HEIGHT,OF_PIXELS_RGB);
        
        
        ofAddListener(vidRecorder.outputFileCompleteEvent, this, &ofApp::recordingComplete);

        vidRecorder.setVideoCodec("h264"); 
        vidRecorder.setVideoBitrate("4000k");
        // vidRecorder.setPixelFormat("yuv420p");
        vidRecorder.setup("pouyan_eye.mov", WIDTH, HEIGHT, 25);
            
        // Start recording
        vidRecorder.start();
    }
    //_player.load("/run/media/avnerus/66220716265174D3/Facebook Piot Experiment/Video/pilotexp.mp4");
    _player.load("/Volumes/Store/Avner/MATTI-BRAIN/matti_session_cropped.mp4");
    
    _player.play();

}

//--------------------------------------------------------------
void ofApp::update(){

    if(_nowRecording){
        _rgbFbo.begin();
        // Check if the video recorder encountered any error while writing video frame or audio smaples.
        if (vidRecorder.hasVideoError()) {
            ofLogWarning("The video recorder failed to write some frames!");
        }
        
        if (vidRecorder.hasAudioError()) {
            ofLogWarning("The video recorder failed to write some audio samples!");
        }
    }


    std::stringstream strm;
    strm << "fps: " << ofGetFrameRate();
    ofSetWindowTitle(strm.str());
    int numToRead = _eyePerFrame;

    if (ofGetFrameNum() % 59 == 0) {
        numToRead = _eyePerLastFrame;
    }


    for (int i = 0; i < numToRead && !_queryDone; i++) {
        if (_query->executeStep()) {
            _eyeX = _query->getColumn(1);
            _eyeY = _query->getColumn(2);
        } else {
            _queryDone = true;
            vidRecorder.close();
            _nowRecording = false;
        }
    }

    // std::cout << "( " << _eyeX << ", " << _eyeY << ")" <<  std::endl;
    //

    _player.update();
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofSetColor(255,255,255); 
    _player.draw(0, 0);
    ofSetColor(255,0,0, 150); 
    ofDrawCircle(_eyeX,_eyeY, 20);

    if (_nowRecording) {
        _rgbFbo.end();
        _rgbFbo.readToPixels(_pix);
        _pix.mirror(true, false);
        vidRecorder.addFrame(_pix);
     //   _rgbFbo.draw(0,0);
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    if (key == 'v') {
        vidRecorder.close();
        _nowRecording = false;
    } 
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

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
