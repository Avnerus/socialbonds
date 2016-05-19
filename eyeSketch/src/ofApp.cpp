#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

    ofEnableAlphaBlending();
    ofEnableSmoothing();
    ofSetBackgroundColor(0,0,0);
    //ofSetBackgroundAuto(false);
    ofSetVerticalSync(true);

//    ofSetLogLevel(OF_LOG_VERBOSE);
    ofSetFrameRate(FRAME_RATE);
    
    std::string eyeDBPath  = ofToDataPath("matti_eyedata.db", true);

    try {

        _eyeDB = new SQLite::Database(eyeDBPath);
        
        // Open EyeLink Dataset
        // std::string eyeDB  = ofToDataPath("eyedata.db", true);
        
        // GET the first onset
        _query = std::make_shared<SQLite::Statement>(*(_eyeDB), "SELECT * from data LIMIT 1");
        _query->executeStep();
        _firstOnset = _query->getColumn(0);
        std::cout << "First onset of eye data " << _firstOnset << std::endl;

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
    _finder.setup();

    _appState = PREPARING;

    _startTime = 0;
    

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
        _player.update();
    }

}

//--------------------------------------------------------------
void ofApp::draw(){

    if (_appState == PREPARING) {
        ofSetColor(255,255,255);
        ofDrawRectangle(WIDTH / 2 - 100, HEIGHT / 2 - 100, 200, 200);
    } else {
        ofSetColor(255,255,255); 
        _player.draw(0, 0);
        ofSetColor(255,0,0, 150); 
        ofDrawCircle(_eyeX,_eyeY, 20);
    }

    if (_nowRecording) {
        _rgbFbo.end();
        _rgbFbo.readToPixels(_pix);
        _pix.mirror(true, false);
        vidRecorder.addFrame(_pix);
     //   _rgbFbo.draw(0,0);
    }
}

void ofApp::reset() {
    std::cout << "Performing RESET" << std::endl;
    _appState = PREPARING;
    _player.setPaused(true);
    std::string resetQuery  = "SELECT * from data WHERE onset >= " + ofToString(_firstOnset + _offsetSec * 1000); // Onset is in milliseconds
    std::cout << resetQuery << std::endl;
    _query = std::make_shared<SQLite::Statement>(*(_eyeDB), resetQuery);
    _player.setFrame(_offsetSec * FRAME_RATE);
    _queryDone = false;
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
