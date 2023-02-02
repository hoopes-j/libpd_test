


#include "Delay.h"



bool Delay::setup(int sampleRate) {
    _sampleRate = sampleRate;
    _maxDelayTimeF = sampleRate*10;
    _bfr.resize(_maxDelayTimeF);
    return true;
}

float Delay::process(float x) {


    return x;
}

void Delay::setDelayTime(int delayTime) {
    _delayTimeF = delayTime/1000 * _sampleRate;
}