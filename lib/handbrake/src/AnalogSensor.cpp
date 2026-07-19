#include "handbrake/AnalogSensor.h"

AnalogSensor::AnalogSensor(uint8_t pin) : _pin(pin) {}

void AnalogSensor::begin() {
    pinMode(_pin, INPUT);
}

int AnalogSensor::read() {
    return analogRead(_pin);
}