#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

    ofEnableAlphaBlending();
    ofEnableSmoothing();
    ofSetBackgroundColor(0,0,0);
    //ofSetBackgroundAuto(false);
    ofSetVerticalSync(true);

    ofSetFrameRate(FRAME_RATE);
    
    std::string eyeDBPath  = ofToDataPath("pouyan_eyedata.db", true);

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

  //  _player.load("/run/media/avnerus/66220716265174D3/Facebook Piot Experiment/Video/pilotexp.mp4");
    _player.load("/Volumes/Store/Avner/POUYAN-BRAIN/pouyan_cropped.mp4");
    _player.play();

}

//--------------------------------------------------------------
void ofApp::update(){


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
        }
    }

    // std::cout << "( " << _eyeX << ", " << _eyeY << ")" <<  std::endl;
    

    _player.update();
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofSetColor(255,255,255); 
    _player.draw(0, 0);
    ofSetColor(255,0,0, 150); 
    ofDrawCircle(_eyeX,_eyeY, 20);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

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
