#include "handbrake/MovingAverageFilter.h"

MovingAverageFilter::MovingAverageFilter(int samples) 
    : _samples(samples), _readIndex(0), _total(0) {
    _readings = new int[_samples];
}

MovingAverageFilter::~MovingAverageFilter() {
    delete[] _readings;
}

void MovingAverageFilter::begin() {
    for (int i = 0; i < _samples; i++) {
        _readings[i] = 0;
    }
}

int MovingAverageFilter::process(int newValue) {
    _total -= _readings[_readIndex];
    _readings[_readIndex] = newValue;
    _total += newValue;
    _readIndex = (_readIndex + 1) % _samples;
    return _total / _samples;
}