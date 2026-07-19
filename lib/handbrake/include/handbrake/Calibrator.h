#pragma once
#include <Arduino.h>

/**
 * @struct CalibrationConfig
 * @brief Calibration configuration for value mapping.
 * 
 * Defines input and output ranges for mapping analog sensor
 * values to the USB joystick range.
 */
struct CalibrationConfig {
    int inputMin;   ///< Minimum input value (ADC at rest)
    int inputMax;   ///< Maximum input value (ADC at full scale)
    int outputMin;  ///< Minimum output value (joystick)
    int outputMax;  ///< Maximum output value (joystick)
};

/**
 * @class Calibrator
 * @brief Class for value mapping and calibration.
 * 
 * Converts values from ADC range (0-1023) to the desired joystick
 * range, applying constraints to prevent out-of-range values.
 * Allows custom calibration based on the potentiometer.
 * 
 * @code
 * CalibrationConfig config = {
 *     .inputMin = 945,
 *     .inputMax = 735,
 *     .outputMin = 0,
 *     .outputMax = 1023
 * };
 * 
 * Calibrator calibrator(config);
 * int mapped = calibrator.map(rawValue);
 * @endcode
 * 
 * @see MovingAverageFilter for pre-mapping filtering
 */
class Calibrator {
public:
    /**
     * @brief Calibrator constructor.
     * @param config Calibration configuration with input/output ranges
     */
    Calibrator(CalibrationConfig config);

    /**
     * @brief Maps a value from input range to output range.
     * @param value Value to map (typically from ADC)
     * @return Mapped and constrained value to output range
     */
    int map(int value);

private:
    CalibrationConfig _config; ///< Stored calibration configuration
};