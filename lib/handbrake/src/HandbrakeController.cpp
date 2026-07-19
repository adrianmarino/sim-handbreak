#include "handbrake/HandbrakeController.h"

HandbrakeController::HandbrakeController(
    AnalogSensor* sensor,
    MovingAverageFilter* filter,
    Calibrator* calibrator,
    JoystickSender* sender,
    AutoCalibrator* autoCal
) : _sensor(sensor), _filter(filter), _calibrator(calibrator), _sender(sender), _autoCal(autoCal) {}

void HandbrakeController::begin() {
    _sensor->begin();
    _filter->begin();
    _sender->begin();
    _autoCal->begin();
    
    // Apply saved calibration from EEPROM
    _calibrator->setConfig(_autoCal->getConfig());
}

void HandbrakeController::update() {
    int rawValue = _sensor->read();
    int filteredValue = _filter->process(rawValue);
    
    // Update auto-calibrator (handles button detection and calibration)
    _autoCal->update(filteredValue);
    
    // If calibrating, skip normal handbrake output
    if (_autoCal->isCalibrating()) {
        return;
    }
    
    // Normal operation: map and send
    int mappedValue = _calibrator->map(filteredValue);
    _sender->send(mappedValue);
}

bool HandbrakeController::isCalibrating() const {
    return _autoCal->isCalibrating();
}