//
//  EEGSound.cpp
//  mySketch
//
//  Created by Avner Peled on 5/3/16.
//
//

#include <EEGSound.h>


void EEGSound::setup() {
    _sender.setup("localhost", 57120); // Supercollider
}

void EEGSound::update(int channel, float value) {
    _message.clear();
    _message.setAddress("/eeg");
    _message.addFloatArg(value);
    _sender.sendMessage(_message);
}

void EEGSound::updateLPP(float value) {
    _message.clear();
    _message.setAddress("/lpp");
    _message.addFloatArg(value);
    _sender.sendMessage(_message);
}

