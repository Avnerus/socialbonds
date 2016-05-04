#include "EEGPlot.h"

void EEGPlot::setup() {
	mouseIsPressed = false;
}

void EEGPlot::appendChannel(std::string name) {
    int numSamples = 6000;
    ofxHistoryPlot* plot = new ofxHistoryPlot( NULL, name, numSamples, false);	
    //plot->setLowerRange(0); //set only the lowest part of the range upper is adaptative to curve
    plot->setAutoRangeShrinksBack(true); //plot scale can shrink back after growing if plot curves requires it
    plot->setColor( ofColor(255,255,255) );
    plot->setShowNumericalInfo(false);
    plot->setRespectBorders(true);
    plot->setLineWidth(1);
    plot->setRange(-1.0, 1.0);

    plot->setShowSmoothedCurve(true); //plot a smoothed version of the values, but alos the original in lesser alpha
    plot->setSmoothFilter(0.1); //smooth filter strength
    plot->setCropToRect(true);
    plot->update(0);

    Channel newChannel;
    newChannel.plot = plot;

    channels.push_back(newChannel);
}

void EEGPlot::update(int index, float value) {
    channels[index].plot->update(value);	//manually add values to the plot
}

void EEGPlot::draw(int x, int y, int width, int height) {
    int yOffset = y + 20;
    int heightPerChannel = (height - 20 * channels.size()) / (channels.size()) ;
    for (int i=0; i < channels.size(); i++) {
        channels[i].plot->draw(x, yOffset, width - 60, heightPerChannel);
        yOffset += heightPerChannel + 20;
    }
}

