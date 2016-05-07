#include "ofApp.h"

bool markerFinished(std::shared_ptr<EEGMarker>& marker) {
    return marker->finished;
}

//--------------------------------------------------------------
void ofApp::setup(){

    ofEnableAlphaBlending();
    ofEnableSmoothing();
    ofSetBackgroundColor(0,0,0);
    ofSetColor(255,255,255); 
    ofSetVerticalSync(true);

    ofSetFrameRate(FRAME_RATE);

    _cam.setupPerspective();

    //ofSetLogLevel(OF_LOG_VERBOSE);

    _eegPerFrame = EEG_RATE / FRAME_RATE;
    _eegPerLastFrame = EEG_RATE / FRAME_RATE + (EEG_RATE % FRAME_RATE);

    std::cout << "EEG Samples per frame: " << _eegPerFrame << ". Last frame: " << _eegPerLastFrame << endl;

    try {

        // Open EEG Dataset
        std::string eegDB = ofToDataPath("pouyan_eeg.db", true);

        _database = new SQLite::Database(eegDB);

        _query = new SQLite::Statement(*(_database), "SELECT * from data");
        _queryDone = false;

    }

    catch (std::exception& e) {
        ofLogError() << "exception: " << e.what();
    }

    _eegPlot = new EEGPlot();
    _eegPlot->setup();
    
    _eegSound = new EEGSound();
    _eegSound->setup();

    _liveMarker = nullptr;


    std::vector<std::string> channelNames = {"C3","Cz","C4","CP1","CP2","CP5","CP6","P3","Pz"};
    for (int i = 0; i < EEG_CHANNELS; i++) {
        _eegPlot->appendChannel(channelNames[i]);
    }

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
        vidRecorder.setup("testeeg.mov", WIDTH, HEIGHT, 30);
            
        // Start recording
        vidRecorder.start();
    }

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
    int numToRead = _eegPerFrame;

    if (ofGetFrameNum() % 59 == 0) {
        numToRead = _eegPerLastFrame;
    }

    for (int i = 0; i < numToRead && !_queryDone; i++) {
        if (_query->executeStep()) {
            for (int channel = 0; channel < EEG_CHANNELS; channel++) {
                double value = _query->getColumn(channel);
                _eegPlot->update(channel,value * 5000.0);
                _eegSound->update(channel,value * 5000.0);
            }
            double lpp = _query->getColumn(10); // Last column is LPP
            std::cout << lpp << std::endl;
            if (lpp > LPP_THRESHOLD) {
                // Passed the threshold
                _eegSound->updateLPP(1.0);
                if (_liveMarker) {
                    _liveMarker->update(lpp);
                } else {
                    // Create a new marker
                    std::shared_ptr<EEGMarker> marker(new EEGMarker());
                    marker->setup(this);
                    marker->update(lpp);
                    _markers.push_back(marker);
                    _liveMarker = marker;
                }
            } else {
                _eegSound->updateLPP(0.0);
                if (_liveMarker) {
                    std::cout << "No more live marker" << std::endl;
                    _liveMarker = nullptr;
                }
            }
        } else {
            _queryDone = true;
        }
    }


    _markers.erase( std::remove_if(_markers.begin(), _markers.end(), markerFinished), _markers.end() );

    _cam.setPosition(WIDTH / 2, HEIGHT / 2, _cam.getPosition().z);
}

//--------------------------------------------------------------
void ofApp::draw(){


    _cam.begin();
    _eegPlot->draw(20, 0, WIDTH, HEIGHT);
    _cam.end();

    for (auto & marker : _markers) {
        if (!marker->finished) {
            marker->draw();
        } else {

        }
    }

    if (_nowRecording) {
        _rgbFbo.end();
        _rgbFbo.readToPixels(_pix);
        _pix.mirror(true, false);
        vidRecorder.addFrame(_pix);
        _rgbFbo.draw(0,0);
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

EEGPlot* ofApp::getEEGPlot() {
    return _eegPlot;
}

ofCamera* ofApp::getCamera() {
    return &_cam;
}
