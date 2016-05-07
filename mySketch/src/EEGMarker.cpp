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
    _x = 517;
    _width = 0;
    _vertexIndex = 5999;

    _meshMatrix = _app->getEEGPlot()->getChannelMeshMatrix(0, 20, 0, _app->WIDTH, _app->HEIGHT);
    std::cout << "Mesh Matrix: " << _meshMatrix << std::endl;
}

void EEGMarker::draw() {
    ofSetColor(0,255,0);
    ofDrawRectangle(_x,0, 2, _app->HEIGHT);
    ofSetColor(0,255,0,50);
    ofDrawRectangle(_x + 2,0, _width, _app->HEIGHT);
}

void EEGMarker::update() {
    // Let's try to get the coordinate of the corrosponding vertex
    
    ofVec3f worldVertex   = _app->getEEGPlot()->getChannelPlotMesh(0)->getVertex(_vertexIndex);
    ofVec3f screenVertex  = _app->getCamera()->worldToScreen(worldVertex);

    screenVertex = screenVertex * _meshMatrix;

    _x = screenVertex.x;
    _vertexIndex -= 19;
    _width += 1;
}

