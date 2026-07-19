#include "handbrake/AutoCalibrator.h"

AutoCalibrator::AutoCalibrator(uint8_t buttonPin, uint8_t ledPin)
    : _buttonPin(buttonPin)
    , _ledPin(ledPin)
    , _calibrating(false)
    , _ledState(false)
    , _ledTimer(0)
    , _minValue(1023)
    , _maxValue(0)
    , _calStartTime(0)
    , _buttonPressTime(0)
    , _buttonPressed(false)
    , _config({DEFAULT_INPUT_MIN, DEFAULT_INPUT_MAX, DEFAULT_OUTPUT_MIN, DEFAULT_OUTPUT_MAX})
{
}

void AutoCalibrator::begin() {
    pinMode(_buttonPin, INPUT_PULLUP);
    pinMode(_ledPin, OUTPUT);
    digitalWrite(_ledPin, LOW);
    
    loadFromEEPROM();
}

void AutoCalibrator::update(int currentAdcValue) {
    bool buttonActive = (digitalRead(_buttonPin) == LOW);
    
    // Button just pressed
    if (buttonActive && !_buttonPressed) {
        _buttonPressed = true;
        _buttonPressTime = millis();
    }
    // Button released
    else if (!buttonActive && _buttonPressed) {
        _buttonPressed = false;
        
        // If not calibrating and held long enough, start calibration
        if (!_calibrating && (millis() - _buttonPressTime >= BUTTON_HOLD_MS)) {
            startCalibration();
        }
    }
    
    // Calibration mode
    if (_calibrating) {
        unsigned long elapsed = millis() - _calStartTime;
        
        // Update min/max with current ADC value
        if (currentAdcValue < _minValue) {
            _minValue = currentAdcValue;
        }
        if (currentAdcValue > _maxValue) {
            _maxValue = currentAdcValue;
        }
        
        // Finish calibration after duration
        if (elapsed >= CALIBRATION_MS) {
            finishCalibration();
        }
        
        updateLED();
    }
    // Button hold detection (slow blink while holding)
    else if (_buttonPressed) {
        unsigned long held = millis() - _buttonPressTime;
        if (held < BUTTON_HOLD_MS) {
            // Slow blink while holding
            if (millis() - _ledTimer >= LED_SLOW_BLINK) {
                _ledState = !_ledState;
                digitalWrite(_ledPin, _ledState);
                _ledTimer = millis();
            }
        }
    }
    else {
        // LED off when not calibrating
        digitalWrite(_ledPin, LOW);
    }
}

bool AutoCalibrator::isCalibrating() const {
    return _calibrating;
}

CalibrationConfig AutoCalibrator::getConfig() const {
    return _config;
}

void AutoCalibrator::reloadConfig() {
    loadFromEEPROM();
}

void AutoCalibrator::saveConfig(const CalibrationConfig& config) {
    _config = config;
    saveToEEPROM();
}

void AutoCalibrator::loadFromEEPROM() {
    CalibrationData data;
    EEPROM.get(0, data);
    
    if (data.magicByte == MAGIC_BYTE) {
        // Valid calibration found
        _config.inputMin = data.inputMin;
        _config.inputMax = data.inputMax;
        _config.outputMin = DEFAULT_OUTPUT_MIN;
        _config.outputMax = DEFAULT_OUTPUT_MAX;
    }
    else {
        // No valid calibration, use defaults
        _config = {DEFAULT_INPUT_MIN, DEFAULT_INPUT_MAX, DEFAULT_OUTPUT_MIN, DEFAULT_OUTPUT_MAX};
    }
}

void AutoCalibrator::saveToEEPROM() {
    CalibrationData data;
    data.inputMin = _config.inputMin;
    data.inputMax = _config.inputMax;
    data.magicByte = MAGIC_BYTE;
    
    EEPROM.put(0, data);
}

void AutoCalibrator::startCalibration() {
    _calibrating = true;
    _calStartTime = millis();
    _minValue = 1023;
    _maxValue = 0;
    _ledTimer = millis();
    _ledState = false;
    
    // LED on to indicate calibration started
    digitalWrite(_ledPin, HIGH);
}

void AutoCalibrator::finishCalibration() {
    _calibrating = false;
    
    // Validate range (minimum 50 ADC units difference)
    if (_maxValue - _minValue > 50) {
        _config.inputMin = _minValue;
        _config.inputMax = _maxValue;
        saveToEEPROM();
        
        // Quick flash to confirm save
        for (int i = 0; i < 3; i++) {
            digitalWrite(_ledPin, LOW);
            delay(100);
            digitalWrite(_ledPin, HIGH);
            delay(100);
        }
    }
    
    // LED off
    digitalWrite(_ledPin, LOW);
}

void AutoCalibrator::updateLED() {
    // Fast blink during calibration
    if (millis() - _ledTimer >= LED_FAST_BLINK) {
        _ledState = !_ledState;
        digitalWrite(_ledPin, _ledState);
        _ledTimer = millis();
    }
}