#pragma once

/**
 * @class MovingAverageFilter
 * @brief Moving average filter for signal smoothing.
 * 
 * Implements a circular moving average filter (ring buffer) to
 * reduce noise in analog sensor readings. The filter averages
 * the last N samples to obtain a stable value.
 * 
 * @note Uses dynamic memory allocation for the circular buffer
 * 
 * @code
 * MovingAverageFilter filter(8);
 * filter.begin();
 * 
 * // In loop:
 * int raw = analogRead(A3);
 * int filtered = filter.process(raw);
 * @endcode
 * 
 * @see AnalogSensor for hardware reading
 * @see Calibrator for value mapping
 */
class MovingAverageFilter {
public:
    /**
     * @brief Moving average filter constructor.
     * @param samples Number of samples for averaging (filter window)
     * @note Higher samples = more smoothing but higher latency
     */
    MovingAverageFilter(int samples);

    /**
     * @brief Destructor - frees circular buffer memory.
     */
    ~MovingAverageFilter();

    /**
     * @brief Initializes the filter to zero.
     * @note Must be called before the first process()
     */
    void begin();

    /**
     * @brief Processes a new sample and returns the average.
     * @param newValue New value to add to the filter
     * @return Average of the last N samples
     */
    int process(int newValue);

private:
    int _samples;    ///< Filter window size
    int* _readings;  ///< Circular sample buffer
    int _readIndex;  ///< Current index in circular buffer
    long _total;     ///< Accumulator for efficient average calculation
};