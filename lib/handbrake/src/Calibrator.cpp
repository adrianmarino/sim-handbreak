#include "handbrake/Calibrator.h"

Calibrator::Calibrator(CalibrationConfig config) : _config(config) {}

int Calibrator::map(int value) {
    int mapped = ::map(value, _config.inputMin, _config.inputMax, 
                      _config.outputMin, _config.outputMax);
    return constrain(mapped, _config.outputMin, _config.outputMax);
}

void Calibrator::setConfig(CalibrationConfig config) {
    _config = config;
}

CalibrationConfig Calibrator::getConfig() const {
    return _config;
}