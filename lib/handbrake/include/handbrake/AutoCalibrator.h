#pragma once
#include <Arduino.h>
#include <EEPROM.h>
#include "Calibrator.h"

/**
 * @struct CalibrationData
 * @brief Data structure for EEPROM storage.
 */
struct CalibrationData {
    uint16_t inputMin;   ///< ADC value at full pull
    uint16_t inputMax;   ///< ADC value at rest
    uint8_t magicByte;   ///< 0xA5 = valid calibration
};

/**
 * @class AutoCalibrator
 * @brief Automatic calibration system with EEPROM persistence.
 * 
 * Manages calibration using an external button and LED feedback.
 * On button press (3 seconds), enters calibration mode for 3 seconds,
 * then saves the detected ADC range to EEPROM.
 * 
 * If EEPROM has no valid calibration, default values are used.
 * 
 * @code
 * AutoCalibrator autoCal(D2, 13);
 * autoCal.begin();
 * 
 * // In loop:
 * autoCal.update(analogRead(A3));
 * if (autoCal.isCalibrating()) {
 *     // Skip normal handbrake update
 *     return;
 * }
 * @endcode
 * 
 * @see Calibrator for value mapping
 * @see HandbrakeController for integration
 */
class AutoCalibrator {
public:
    /**
     * @brief AutoCalibrator constructor.
     * @param buttonPin Digital pin for calibration button (INPUT_PULLUP)
     * @param ledPin Digital pin for LED feedback (13 = built-in LED)
     */
    AutoCalibrator(uint8_t buttonPin, uint8_t ledPin);

    /**
     * @brief Initializes auto-calibrator, loads EEPROM if valid.
     */
    void begin();

    /**
     * @brief Main update loop - call every iteration.
     * @param currentAdcValue Current ADC reading from potentiometer
     */
    void update(int currentAdcValue);

    /**
     * @brief Check if currently in calibration mode.
     * @return true if calibrating, false otherwise
     */
    bool isCalibrating() const;

    /**
     * @brief Get the current calibration configuration.
     * @return CalibrationConfig with detected or default values
     */
    CalibrationConfig getConfig() const;

    /**
     * @brief Force reload configuration from EEPROM.
     */
    void reloadConfig();

    /**
     * @brief Save current config to EEPROM.
     * @param config Configuration to save
     */
    void saveConfig(const CalibrationConfig& config);

private:
    void loadFromEEPROM();
    void saveToEEPROM();
    void startCalibration();
    void finishCalibration();
    void updateLED();

    uint8_t _buttonPin;
    uint8_t _ledPin;
    
    bool _calibrating;        ///< True during calibration mode
    bool _ledState;           ///< Current LED state
    unsigned long _ledTimer;  ///< LED blink timer
    
    int _minValue;            ///< Detected minimum ADC
    int _maxValue;            ///< Detected maximum ADC
    unsigned long _calStartTime;  ///< Calibration start time
    unsigned long _buttonPressTime; ///< Button press start time
    bool _buttonPressed;      ///< Button currently pressed
    
    CalibrationConfig _config; ///< Current calibration config
    
    static const uint16_t MAGIC_BYTE = 0xA5;  ///< EEPROM valid marker
    static const unsigned long BUTTON_HOLD_MS = 3000;  ///< Button hold time
    static const unsigned long CALIBRATION_MS = 3000;  ///< Calibration duration
    static const unsigned long LED_FAST_BLINK = 200;   ///< Fast blink interval
    static const unsigned long LED_SLOW_BLINK = 500;   ///< Slow blink interval
    
    /// Default calibration values (current hardcoded values)
    static const int DEFAULT_INPUT_MIN = 945;
    static const int DEFAULT_INPUT_MAX = 735;
    static const int DEFAULT_OUTPUT_MIN = 0;
    static const int DEFAULT_OUTPUT_MAX = 1023;
};