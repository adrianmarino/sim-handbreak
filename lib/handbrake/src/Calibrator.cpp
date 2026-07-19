#include "handbrake/Calibrator.h"

Calibrator::Calibrator(CalibrationConfig config) : _config(config) {}

int Calibrator::map(int value) {
    int mapped = ::map(value, _config.inputMin, _config.inputMax, 
                      _config.outputMin, _config.outputMax);
    return constrain(mapped, _config.outputMin, _config.outputMax);
}