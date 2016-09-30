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
    
    ofLogToFile("log.txt");
    ofEnableAlphaBlending();
    ofEnableSmoothing();
    ofSetBackgroundColor(0,0,0);
    //ofSetBackgroundAuto(false);
    ofSetVerticalSync(true);

//    ofSetLogLevel(OF_LOG_VERBOSE);
    ofSetFrameRate(FRAME_RATE);
    
    std::string eyeDBPath  = ofToDataPath("matti_eyedata.db", true);

    _xScale = (float)(WIDTH) / 1920.0;
    _yScale = (float)(HEIGHT) / 1200.0;

    try {

        _eyeDB = new SQLite::Database(eyeDBPath);
        
        // Open EyeLink Dataset
        // std::string eyeDB  = ofToDataPath("eyedata.db", true);
        
        // GET the first onset
        _query = std::make_shared<SQLite::Statement>(*(_eyeDB), "SELECT * from data LIMIT 1");
        _query->executeStep();
        _firstOnset = _query->getColumn(0);
        ofLogNotice() << "First onset of eye data " << _firstOnset << std::endl;

    }

    catch (std::exception& e) {
        ofLogError() << "exception: " << e.what();
    }

    _eyePerFrame = EYE_RATE / FRAME_RATE;
    _eyePerLastFrame = EYE_RATE / FRAME_RATE + (EYE_RATE % FRAME_RATE);

    ofLogNotice() << "EYE Samples per frame: " << _eyePerFrame << ". Last frame: " << _eyePerLastFrame << std::endl;

    //_player.load("/run/media/avnerus/66220716265174D3/Facebook Piot Experiment/Video/pilotexp.mp4");
    _player.load("matti_session_trimmed.mp4");
    _finder.setup();

    _appState = PREPARING;

    _startTime = 0;
}

//--------------------------------------------------------------
void ofApp::update(){


	if(_finder.isRunning() && _finder.doesServerFound()){
		_finder.close();
        ofLogNotice() << "Found server at " << _finder.getServerInfo().ip << ". Setting up client. " << std::endl;
		if(_client.setup(_finder.getServerInfo().ip, _finder.getServerInfo().port)){
			ofAddListener(_client.messageReceived, this, &ofApp::onMessageReceived);
            ofLogNotice() << "Client connected" << std::endl;
		} else {
			ofLogError() << "failed to start client";
		}
	}

    if (_appState == PREPARING) {
        if (_startTime > 0 && ofGetElapsedTimeMillis() >= _startTime) {
            ofLogNotice() << "START!" << std::endl;
            _appState = RUNNING;
            _player.play();
        }
    } 
    else if (_appState == RUNNING) {
        int numToRead = _eyePerFrame;

        for (int i = 0; i < numToRead && !_queryDone; i++) {
            try {
                if (_query->executeStep()) {
                    _eyeX = _query->getColumn(1);
                    _eyeY = _query->getColumn(2);
                } else {
                    _queryDone = true;
                }
            }
            catch (std::exception& e) {
                ofLogError() << "EYE DB EXCEPTIION OCCURED : " << e.what();
            }
        }
        try {
            _player.update();
        }
        catch (std::exception& e) {
            ofLogError() << "PLAYER EXCEPTIION OCCURED : " << e.what();
        }
    }

}

//--------------------------------------------------------------
void ofApp::draw(){

    if (_appState == PREPARING) {
        ofSetColor(255,255,255);
        ofDrawRectangle(WIDTH / 2 - 100, HEIGHT / 2 - 100, 200, 200);
    } else {
        ofSetColor(255,255,255); 
        _player.draw(0, 0, WIDTH, HEIGHT);
        ofSetColor(255,0,0, 150); 
        ofDrawCircle(_eyeX * _xScale, _eyeY * _yScale, 20 * _xScale);
    }
}

void ofApp::reset() {
    ofLogNotice() << "Performing RESET" << std::endl;
    _appState = PREPARING;
    _player.stop();
    std::string resetQuery  = "SELECT * from data WHERE onset >= " + ofToString(_firstOnset + _offsetSec * 1000); // Onset is in milliseconds
    ofLogNotice() << resetQuery << std::endl;
    _query = std::make_shared<SQLite::Statement>(*(_eyeDB), resetQuery);
    //ofLogNotice() << "Starting in frame " << (_offsetSec * FRAME_RATE) << std::endl;
    //_player.setFrame(_offsetSec * FRAME_RATE);
    ofLogNotice() << "Starting in frame 0" << (_offsetSec * FRAME_RATE) << std::endl;
    _queryDone = false;
}

void ofApp::onMessageReceived(string & message){

	const string messagePlay = "PLAY ";
	if(message.find(messagePlay) == 0 && _client.isCalibrated()){
        std::string args = message.substr(messagePlay.length());
        std::size_t commaPos = args.find(',');
        int time = ofToInt(args.substr(0,commaPos));
        _offsetSec = ofToInt(args.substr(commaPos + 1));
        _startTime =  ofGetElapsedTimeMillis() + (time - _client.getSyncedElapsedTimeMillis());

        ofLogNotice() << "Start time: " << _startTime << " Offset: " << _offsetSec << std::endl;

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
    ofLogToConsole();
    ofLogNotice() << "EXIT" << std::endl;
}

