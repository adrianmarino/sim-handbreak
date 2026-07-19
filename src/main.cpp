#include <Arduino.h>
#include "handbrake/AnalogSensor.h"
#include "handbrake/MovingAverageFilter.h"
#include "handbrake/Calibrator.h"
#include "handbrake/JoystickSender.h"
#include "handbrake/HandbrakeController.h"

const int POT_PIN = A3;
const int FILTER_SAMPLES = 8;
const int ADC_REPOSO = 945;
const int ADC_A_FONDO = 735;

AnalogSensor sensor(POT_PIN);
MovingAverageFilter filter(FILTER_SAMPLES);
Calibrator calibrator({ADC_REPOSO, ADC_A_FONDO, 0, 1023});
JoystickSender sender;
HandbrakeController handbrake(&sensor, &filter, &calibrator, &sender);

void setup() {
    handbrake.begin();
}

void loop() {
    handbrake.update();
    delay(10);
}