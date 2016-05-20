#include "ofApp.h"

ofApp::ofApp() {
    bool parsingSuccessful = _config.open("config.json");
    if (parsingSuccessful) {
        ofLogNotice("ofApp::setup") << _config.getRawString() << std::endl;
        HEIGHT = _config["height"].asInt();
        WIDTH = _config["width"].asInt();
        FULLSCREEN = _config["fullscreen"].asInt();
    } else {
        ofLogError("ofApp::setup") << "Failed to parse JSON config" << std::endl;
    }
}
//--------------------------------------------------------------
void ofApp::setup(){

    ofEnableAlphaBlending();
    ofEnableSmoothing();
    ofSetBackgroundColor(0,0,0);
    //ofSetBackgroundAuto(false);
    ofSetVerticalSync(true);

//    ofSetLogLevel(OF_LOG_VERBOSE);
    //ofSetFrameRate(FRAME_RATE);
    
    _player.load("matti_cam_cropped.mp4");
    _player.setSpeed(0.8);
    _finder.setup();

    _appState = PREPARING;

    _startTime = 0;
}

//--------------------------------------------------------------
void ofApp::update(){

    if(_finder.isRunning() && _finder.doesServerFound()){
        _finder.close();
        std::cout << "Found server at " << _finder.getServerInfo().ip << ". Setting up client. " << std::endl;
        if(_client.setup(_finder.getServerInfo().ip, _finder.getServerInfo().port)){
            ofAddListener(_client.messageReceived, this, &ofApp::onMessageReceived);
            std::cout << "Client connected" << std::endl;
        } else {
            ofLogError() << "failed to start client";
        }
    }

    if (_appState == PREPARING) {
        if (_startTime > 0 && ofGetElapsedTimeMillis() >= _startTime) {
            std::cout << "START!" << std::endl;
            _appState = RUNNING;
            _player.play();
        }
    } 
    if (_appState == RUNNING) {
        _player.setSpeed(0.81);
        _player.update();
    }

    //std::cout << _player.getSpeed() << std::endl;

}

//--------------------------------------------------------------
void ofApp::draw(){

    if (_appState == PREPARING) {
        ofSetColor(255,255,255);
        ofDrawRectangle(WIDTH / 2 - 100, HEIGHT / 2 - 100, 200, 200);
    } else {
        _player.draw(0, 0, WIDTH, HEIGHT);
    }
}

void ofApp::reset() {
    std::cout << "Performing RESET" << std::endl;
    _appState = PREPARING;
    _player.setPaused(true);
    // some delay
    _player.setFrame((_offsetSec - 11)  * FRAME_RATE);
    //float pct = (float)(_offsetSec) / 2552.0;
    //std::cout << "Set position " << pct << std::endl;
    //_player.setPosition(pct);
    

}

void ofApp::onMessageReceived(string & message){

    const string messagePlay = "PLAY ";
    if(message.find(messagePlay) == 0 && _client.isCalibrated()){
        std::string args = message.substr(messagePlay.length());
        std::cout << args << std::endl;
        std::size_t commaPos = args.find(',');
        int time = ofToInt(args.substr(0,commaPos));
        _offsetSec = ofToInt(args.substr(commaPos + 1));
        _startTime =  ofGetElapsedTimeMillis() + (time - _client.getSyncedElapsedTimeMillis());

        std::cout << "Start time: " << _startTime << " Offset: " << _offsetSec << std::endl;

        reset();
    }
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

void ofApp::exit(){
    std::cout << "EXIT" << std::endl;
}
