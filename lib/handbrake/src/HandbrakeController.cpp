#include "handbrake/HandbrakeController.h"

HandbrakeController::HandbrakeController(
    AnalogSensor* sensor,
    MovingAverageFilter* filter,
    Calibrator* calibrator,
    JoystickSender* sender
) : _sensor(sensor), _filter(filter), _calibrator(calibrator), _sender(sender) {}

void HandbrakeController::begin() {
    _sensor->begin();
    _filter->begin();
    _sender->begin();
}

void HandbrakeController::update() {
    int rawValue = _sensor->read();
    int filteredValue = _filter->process(rawValue);
    int mappedValue = _calibrator->map(filteredValue);
    _sender->send(mappedValue);
}