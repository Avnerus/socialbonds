#include "EEGPlot.h"

void EEGPlot::setup() {
	mouseIsPressed = false;

}

void EEGPlot::appendChannel(int x, int y) {
    int numSamples = 25000;
    ofxHistoryPlot* plot = new ofxHistoryPlot( NULL, "", numSamples, false);	
    //plot->setLowerRange(0); //set only the lowest part of the range upper is adaptative to curve
    plot->setAutoRangeShrinksBack(true); //plot scale can shrink back after growing if plot curves requires it
    plot->setColor( ofColor(255,255,255) );
    plot->setShowNumericalInfo(true);
    plot->setRespectBorders(true);
    plot->setLineWidth(1);

    plot->setShowSmoothedCurve(true); //plot a smoothed version of the values, but alos the original in lesser alpha
    plot->setSmoothFilter(0.1); //smooth filter strength
    plot->setCropToRect(true);
    plot->update(0);

    Channel newChannel;
    newChannel.plot = plot;
    newChannel.x = x;
    newChannel.y = y;

    channels.push_back(newChannel);
}

void EEGPlot::update(int index, float value) {
    channels[index].plot->update(value);	//manually add values to the plot
}

void EEGPlot::draw() {
    for (int i=0; i < channels.size(); i++) {
        channels[i].plot->draw(channels[i].x, channels[i].y, 1920, 40);
    }
}
