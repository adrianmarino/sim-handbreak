#pragma once
#include <Arduino.h>

/**
 * @class AnalogSensor
 * @brief Class for reading analog sensors.
 * 
 * Encapsulates Arduino analog pin reading, providing a simple
 * and reusable interface for sensors that return values in the
 * ADC range (0-1023).
 * 
 * @note Requires calling begin() before using read()
 * 
 * @code
 * AnalogSensor sensor(A3);
 * sensor.begin();
 * int value = sensor.read();
 * @endcode
 */
class AnalogSensor {
public:
    /**
     * @brief AnalogSensor constructor.
     * @param pin Analog pin number (e.g.: A0, A1, A3)
     */
    AnalogSensor(uint8_t pin);

    /**
     * @brief Initializes the sensor by configuring the pin as input.
     * @note Must be called in setup() before read()
     */
    void begin();

    /**
     * @brief Reads the current analog sensor value.
     * @return ADC value in range [0, 1023]
     */
    int read();

private:
    uint8_t _pin; ///< Analog pin associated with the sensor
};