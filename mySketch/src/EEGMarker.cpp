//
//  EEGSound.cpp
//  mySketch
//
//  Created by Avner Peled on 5/3/16.
//
//

#include <EEGMarker.h>


EEGMarker::EEGMarker():_markerColor(0, 0, 255),_markerFilling(0, 0, 255, 50) {

}

void EEGMarker::setup(ofApp* app, double value) {
    _app = app;
    _lastVertexIndex = _app->getEEGPlot()->getChannelPlotMesh(0)->getNumVertices() - 1;
    _firstVertexIndex = _lastVertexIndex;

    _meshMatrix = _app->getEEGPlot()->getChannelMeshMatrix(0, 0, 0, _app->WIDTH, _app->HEIGHT);

    ofVec3f worldVertex   = _app->getEEGPlot()->getChannelPlotMesh(0)->getVertex(_firstVertexIndex);
    ofVec3f screenVertex  = _app->getCamera()->worldToScreen(worldVertex);
    screenVertex = screenVertex * _meshMatrix;
    _x = _lastX = screenVertex.x - 50;

    std::cout << "EEG Marker created!" << std::endl;

    _money = value * 1000;
    _labelHeight = ofRandom(50,_app->HEIGHT - 50);
    finished = false;
}

void EEGMarker::draw() {
    if (_x > 0) {
        ofSetColor(_markerColor);
        ofDrawRectangle(_x,0, 2, _app->HEIGHT);
    }
    ofSetColor(_markerFilling);
    ofDrawRectangle(_x + 2,0, _width, _app->HEIGHT);

    // Draw money
    std::string label = std::string("$") + ofToString(_money, 5);
    ofSetColor(255,255,255);
    _app->getFont()->drawString(label, _x + (_width / 2), _labelHeight);
}

void EEGMarker::update(double value) {

    _money += value * 1000;
    
    // Go back vertexes according to the number of samples per frame
    _firstVertexIndex -= (_app->getNumberOfSamplesPerFrame() - 1);
    if (value == 0) {
        _lastVertexIndex -= (_app->getNumberOfSamplesPerFrame() - 1);
    }

    if (_firstVertexIndex < 0) {
        _x = 0;
        if (_lastVertexIndex >= 0) {
            ofVec3f worldVertex   = _app->getEEGPlot()->getChannelPlotMesh(0)->getVertex(_lastVertexIndex);
            ofVec3f screenVertex  = _app->getCamera()->worldToScreen(worldVertex);
            screenVertex = screenVertex * _meshMatrix;
            _width = screenVertex.x - 50;
            // We went out the screen, but can still decrease width
        } else {
            std::cout << "Last Vertex index is " << _lastVertexIndex << "!" << "I'm finished" << std::endl;
            finished = true;
        }
    } else {
        // Move the start vertex backwards
        ofVec3f worldVertex   = _app->getEEGPlot()->getChannelPlotMesh(0)->getVertex(_firstVertexIndex);
        ofVec3f screenVertex  = _app->getCamera()->worldToScreen(worldVertex);
        screenVertex = screenVertex * _meshMatrix;
        _x = screenVertex.x;

        // If this is still active, the width keeps increasing
        if (value > 0) {
            _width = _lastX - _x;
        }
    }
}


EEGMarker::~EEGMarker() {
    std::cout << "Marker destroyed!!" << std::endl;
}
