#include "handbrake/JoystickSender.h"

JoystickSender::JoystickSender() {}

void JoystickSender::begin() {
    _joystick.begin(false);
    _joystick.setThrottleRange(0, 1023);
}

void JoystickSender::send(int value) {
    _joystick.setRyAxis(value);
    _joystick.sendState();
}