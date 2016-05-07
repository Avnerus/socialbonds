//
//  EEGSound.cpp
//  mySketch
//
//  Created by Avner Peled on 5/3/16.
//
//

#include <EEGMarker.h>


void EEGMarker::setup(ofApp* app) {
    _app = app;
    _vertexIndex = _app->getEEGPlot()->getChannelPlotMesh(0)->getNumVertices() - 1;
    _meshMatrix = _app->getEEGPlot()->getChannelMeshMatrix(0, 20, 0, _app->WIDTH, _app->HEIGHT);

    ofVec3f worldVertex   = _app->getEEGPlot()->getChannelPlotMesh(0)->getVertex(_vertexIndex);
    ofVec3f screenVertex  = _app->getCamera()->worldToScreen(worldVertex);
    screenVertex = screenVertex * _meshMatrix;
    _x = _lastX = screenVertex.x - 50;

    std::cout << "EEG Marker created!" << std::endl;

    finished = false;
}

void EEGMarker::draw() {
    ofSetColor(0,255,0);
    ofDrawRectangle(_x,0, 2, _app->HEIGHT);
    ofSetColor(0,255,0,50);
    ofDrawRectangle(_x + 2,0, _width, _app->HEIGHT);

    ofVec3f worldVertex   = _app->getEEGPlot()->getChannelPlotMesh(0)->getVertex(_vertexIndex);
    ofVec3f screenVertex  = _app->getCamera()->worldToScreen(worldVertex);
    screenVertex = screenVertex * _meshMatrix;

    _x = screenVertex.x;

    _vertexIndex -= 19;
    if (_vertexIndex < 0) {
        std::cout << "Vertex index is " << _vertexIndex << "!" << "I'm finished" << std::endl;
        finished = true;
    }
}

void EEGMarker::update(double value) {
    // Let's try to get the coordinate of the corrosponding vertex
    
    _width = _lastX - _x;

}


EEGMarker::~EEGMarker() {
    std::cout << "Marker destroyed!!" << std::endl;
}
