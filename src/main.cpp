#include <Arduino.h>
#include "handbrake/AnalogSensor.h"
#include "handbrake/MovingAverageFilter.h"
#include "handbrake/Calibrator.h"
#include "handbrake/JoystickSender.h"
#include "handbrake/AutoCalibrator.h"
#include "handbrake/HandbrakeController.h"

// Hardware pins
const int POT_PIN = A3;
const int CALIBRATION_BUTTON_PIN = 2;
const int CALIBRATION_LED_PIN = 13;

// Filter configuration
const int FILTER_SAMPLES = 8;

// Default calibration values (used if EEPROM is empty)
const int ADC_REPOSO = 945;
const int ADC_A_FONDO = 735;

AnalogSensor sensor(POT_PIN);
MovingAverageFilter filter(FILTER_SAMPLES);
Calibrator calibrator({ADC_REPOSO, ADC_A_FONDO, 0, 1023});
JoystickSender sender;
AutoCalibrator autoCal(CALIBRATION_BUTTON_PIN, CALIBRATION_LED_PIN);
HandbrakeController handbrake(&sensor, &filter, &calibrator, &sender, &autoCal);

void setup() {
    handbrake.begin();
}

void loop() {
    handbrake.update();
    delay(10);
}