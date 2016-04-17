#include "EEGPlot.h"

void EEGPlot::setup() {
	mouseIsPressed = false;
	int numSamples = 15000;
	

	plot = new ofxHistoryPlot( NULL, "Soujanyaa's Brain", numSamples, false);	
	//plot->setLowerRange(0); //set only the lowest part of the range upper is adaptative to curve
	plot->setAutoRangeShrinksBack(true); //plot scale can shrink back after growing if plot curves requires it
	plot->setColor( ofColor(50,0,255) );
	plot->setShowNumericalInfo(true);
	plot->setRespectBorders(true);
	plot->setLineWidth(1);

	plot->setShowSmoothedCurve(true); //plot a smoothed version of the values, but alos the original in lesser alpha
	plot->setSmoothFilter(0.1); //smooth filter strength
	plot->setCropToRect(true);
	plot->update(0);
}

void EEGPlot::update(float value) {
    plot->update(value);	//manually add values to the plot
}

void EEGPlot::draw() {
	plot->draw(10, 10, 1024, 240);
}
