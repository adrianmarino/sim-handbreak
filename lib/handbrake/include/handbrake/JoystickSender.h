#pragma once
#include <Joystick.h>

/**
 * @class JoystickSender
 * @brief Class for sending USB HID Joystick reports.
 * 
 * Manages USB HID communication with the host, sending USB
 * joystick reports using the mheironimus/Joystick library.
 * Configures the Ry axis (Throttle) to simulate a handbrake.
 * 
 * @note Uses begin(false) for manual report sending control
 * 
 * @code
 * JoystickSender sender;
 * sender.begin();
 * 
 * // In loop:
 * sender.send(512); // Send value to Ry axis
 * @endcode
 * 
 * @see HandbrakeController for full orchestration
 */
class JoystickSender {
public:
    /**
     * @brief JoystickSender constructor.
     */
    JoystickSender();

    /**
     * @brief Initializes the USB joystick.
     * @note Configures manual send mode (begin(false))
     * @note Sets Throttle axis range: 0-1023
     */
    void begin();

    /**
     * @brief Sends a value to the Ry axis of the USB joystick.
     * @param value Value to send (range: 0-1023)
     * @note Automatically updates the USB report
     */
    void send(int value);

private:
    Joystick_ _joystick; ///< USB joystick instance
};