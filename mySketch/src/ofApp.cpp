#include "ofApp.h"

bool markerFinished(std::shared_ptr<EEGMarker>& marker) {
    return marker->finished;
}

ofApp::ofApp() {
    bool parsingSuccessful = _config.open("config.json");
    if (parsingSuccessful) {
        ofLogNotice("ofApp::setup") << _config.getRawString() << std::endl;
        HEIGHT = _config["height"].asInt();
        WIDTH = _config["width"].asInt();
        START_OFFSET_SEC = _config["startOffsetSec"].asInt();
        SERVER_PORT = _config["serverPort"].asInt();
        FULLSCREEN = _config["fullscreen"].asInt();
        LPP_THRESHOLD = _config["lppThreshold"].asDouble();
        PLOT_MARGIN = _config["plotMargin"].asInt();
        PLOT_RIGHT_MARGIN = _config["plotRightMargin"].asInt();
        MONEY_FONT_SIZE = _config["moneyFontSize"].asInt();
        PLOT_FONT_SIZE = _config["plotFontSize"].asInt();
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
    ofSetColor(255,255,255); 
    ofSetVerticalSync(true);

//    ofSetLogLevel(OF_LOG_VERBOSE);

    ofSetFrameRate(FRAME_RATE);

    _font.load("Verdana.ttf", MONEY_FONT_SIZE);

    _appState = PREPARING;

    // This is the network server
    bool result = _server.setup(9540);

    ofLogNotice() << "Network server startup result: " << result << std::endl;

    _cam.setupPerspective();
    _blink = false;

    //ofSetLogLevel(OF_LOG_VERBOSE);

    _eegPerFrame = EEG_RATE / FRAME_RATE;
    _eegPerLastFrame = EEG_RATE / FRAME_RATE + (EEG_RATE % FRAME_RATE);

    _eegRestartFrame = EEG_RATE * RESTART_TIME_SEC;

    ofLogNotice() << "EEG Samples per frame: " << _eegPerFrame << ". Last frame: " << _eegPerLastFrame << endl;

    try {

        // Open EEG Dataset
        std::string eegDB = ofToDataPath("matti_eeg.db", true);

        _database = new SQLite::Database(eegDB);
    }

    catch (std::exception& e) {
        ofLogError() << "exception: " << e.what();
    }

    _eegPlot = new EEGPlot();
    _eegPlot->setup(this);
    
    _eegSound = new EEGSound();
    _eegSound->setup();

    _liveMarker = nullptr;


    std::vector<std::string> channelNames = {"C3","Cz","C4","CP1","CP2","CP5","CP6","P3","Pz"};
    for (int i = 0; i < EEG_CHANNELS; i++) {
        _eegPlot->appendChannel(channelNames[i]);
    }

    _startTime = 0;

}

//--------------------------------------------------------------
void ofApp::update(){
    
    try {
        if(_server.isConnected()){
            _server.update();
        }
        if (_appState == PREPARING) {
            if (_startTime > 0 && ofGetElapsedTimeMillis() >= _startTime) {
                _appState = RUNNING;
            }
        }
        if (_appState == RUNNING) {
            int numToRead = _eegPerFrame;
            
            if (ofGetFrameNum() % 59 == 0) {
                numToRead = _eegPerLastFrame;
            }
            
            bool thresholdPassed = false;
            double biggestValue = 0;
            
            for (int i = 0; i < numToRead && !_queryDone; i++) {
                if (_query->executeStep()) {
                    for (int channel = 0; channel < EEG_CHANNELS; channel++) {
                        double value = _query->getColumn(channel + 1);
                        _eegPlot->update(channel,value * 5000.0);
                        _eegSound->update(channel,value * 5000.0);
                    }
                    double lpp = _query->getColumn(11); // Last column is LPP
                    //std::cout << lpp << std::endl;
                    if (lpp > LPP_THRESHOLD) {
                        thresholdPassed = true;
                        if (lpp > biggestValue) {
                            biggestValue = lpp;
                        }
                    }
                } else {
                    _queryDone = true;
                    restart();
                }
            }
            
            if (thresholdPassed) {
                // Passed the threshold
                if (_liveMarker) {
                    _liveMarker->update(biggestValue);
                } else {
                    // Create a new marker
                    std::shared_ptr<EEGMarker> marker(new EEGMarker());
                    marker->setup(this, biggestValue);
                    _markers.push_back(marker);
                    _liveMarker = marker;
                }
                if (!_blink) {
                    _blink = true;
                    ofSetBackgroundColor(255,255,255);
                    _eegSound->updateLPP(1.0);
                } else {
                    ofSetBackgroundColor(0,0,0);
                    _blink = false;
                }
            } else {
                ofSetBackgroundColor(0,0,0);
                _blink = false;
                if (_liveMarker) {
                    //std::cout << "No more live marker" << std::endl;
                    _liveMarker = nullptr;
                }
            }
            
            for (auto & marker : _markers) {
                if (marker != _liveMarker) {
                    marker->update(0);
                }
            }
            
            _markers.erase( std::remove_if(_markers.begin(), _markers.end(), markerFinished), _markers.end() );
        
            //std::cout << "Markers: " << _markers.size() << std::endl;
            
            int lastId = _query->getColumn(0);
            if (lastId >= _eegRestartFrame) {
                restart();
            }
        }
        
        
        _cam.setPosition(WIDTH / 2, HEIGHT / 2, _cam.getPosition().z);

    }
    catch (std::exception& e) {
        ofLogError() << "RESTARTING DUE TO EXCEPTION (Update): " << e.what() << std::endl;
        restart();
    }
}

int ofApp::getNumberOfSamplesPerFrame() {
    return _eegPerFrame;
}

ofTrueTypeFont* ofApp::getFont() {
    return &_font;
}

//--------------------------------------------------------------
void ofApp::draw(){
    try {
        if (_appState == PREPARING) {
            ofSetColor(255,255,255);
            ofDrawRectangle(WIDTH / 2 - 100, HEIGHT / 2 - 100, 200, 200);
        } else {
            ofSetColor(0,0,0);
            ofDrawRectangle(WIDTH - PLOT_RIGHT_MARGIN, 0, PLOT_RIGHT_MARGIN, HEIGHT);
            
            _cam.begin();
            _eegPlot->draw(0, 0, WIDTH, HEIGHT);
            _cam.end();
            
            for (auto & marker : _markers) {
                if (!marker->finished) {
                    marker->draw();
                } 
            }
            
        }
    }
    catch (std::exception& e) {
        ofLogError() << "RESTARTING DUE TO EXCEPTION (Draw): " << e.what() << std::endl;
        restart();
    }
}

void ofApp::reset() {
    ofLogNotice() << "Performing RESET" << std::endl;
    _markers.clear();
    _liveMarker = nullptr;
    _eegPlot->reset();

    std::string resetQuery  = "SELECT * from data WHERE id >= " + ofToString(START_OFFSET_SEC * EEG_RATE); // id is 500 in a sec
    ofLogNotice() << resetQuery << std::endl;
    _query = std::make_shared<SQLite::Statement>(*(_database), resetQuery);
    _queryDone = false;
}

void ofApp::restart() {
    _startTime = ofGetElapsedTimeMillis() + 5000;
    _appState = PREPARING;
    reset();

    ofLogNotice() << "Sending PLAY message. Start time: " << _startTime << std::endl;
    // Start presentation
    for (auto & client : _server.getClients()) {
        ofLogNotice() << client->isCalibrated() << std::endl;
        if(client->isCalibrated()){
            client->send("PLAY "+ofToString(_startTime) + "," + ofToString(START_OFFSET_SEC));
            ofLogNotice() << "Send to client " << client->getClientID() << std::endl;
        }
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    if (key == 'p') {
        restart();
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
    ofLogToConsole();
    ofLogNotice() << "EXIT" << std::endl;
}


EEGPlot* ofApp::getEEGPlot() {
    return _eegPlot;
}

ofCamera* ofApp::getCamera() {
    return &_cam;
}
