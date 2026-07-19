#pragma once
#include "AnalogSensor.h"
#include "MovingAverageFilter.h"
#include "Calibrator.h"
#include "JoystickSender.h"

/**
 * @class HandbrakeController
 * @brief Facade that orchestrates the full handbrake pipeline.
 * 
 * Implements the Facade pattern to coordinate system components:
 * sensor, filter, calibrator, and sender. Follows the Dependency
 * Inversion Principle (DIP) by receiving dependencies via injection.
 * 
 * ## Data Flow
 * ```
 * Sensor → Filter → Calibrator → USB Sender
 * ```
 * 
 * @code
 * // Configure components
 * AnalogSensor sensor(A3);
 * MovingAverageFilter filter(8);
 * Calibrator calibrator({945, 735, 0, 1023});
 * JoystickSender sender;
 * 
 * // Create controller with injection
 * HandbrakeController handbrake(&sensor, &filter, &calibrator, &sender);
 * 
 * void setup() {
 *     handbrake.begin();
 * }
 * 
 * void loop() {
 *     handbrake.update();
 *     delay(10);
 * }
 * @endcode
 * 
 * @see AnalogSensor for hardware reading
 * @see MovingAverageFilter for signal filtering
 * @see Calibrator for value mapping
 * @see JoystickSender for USB sending
 */
class HandbrakeController {
public:
    /**
     * @brief Constructor with dependency injection.
     * @param sensor Pointer to the analog sensor
     * @param filter Pointer to the moving average filter
     * @param calibrator Pointer to the value calibrator
     * @param sender Pointer to the USB sender
     * 
     * @pre All pointers must be valid
     * @post Controller takes logical ownership of the components
     */
    HandbrakeController(
        AnalogSensor* sensor,
        MovingAverageFilter* filter,
        Calibrator* calibrator,
        JoystickSender* sender
    );

    /**
     * @brief Initializes all pipeline components.
     * @note Calls begin() on each injected component
     * @note Must be called in setup()
     */
    void begin();

    /**
     * @brief Executes a complete pipeline cycle.
     * @note Read → Filter → Calibrate → Send
     * @note Call in loop() with 10ms interval (100Hz)
     */
    void update();

private:
    AnalogSensor* _sensor;          ///< Injected analog sensor
    MovingAverageFilter* _filter;   ///< Injected moving average filter
    Calibrator* _calibrator;        ///< Injected value calibrator
    JoystickSender* _sender;        ///< Injected USB sender
};