#pragma once
#include <Arduino.h>

/**
 * @struct CalibrationConfig
 * @brief Configuración de calibración para el mapeo de valores.
 * 
 * Define los rangos de entrada y salida para el mapeo de valores
 * del sensor analógico al rango del joystick USB.
 */
struct CalibrationConfig {
    int inputMin;   ///< Valor mínimo de entrada (ADC reposo)
    int inputMax;   ///< Valor máximo de entrada (ADC fondo de escala)
    int outputMin;  ///< Valor mínimo de salida (joystick)
    int outputMax;  ///< Valor máximo de salida (joystick)
};

/**
 * @class Calibrator
 * @brief Clase para mapeo y calibración de valores.
 * 
 * Convierte valores del rango del ADC (0-1023) al rango deseado
 * del joystick, aplicando constrains para evitar valores fuera
 * de rango. Permite calibración personalizada según el potenciómetro.
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
 * @see MovingAverageFilter para filtrado previo al mapeo
 */
class Calibrator {
public:
    /**
     * @brief Constructor de Calibrator.
     * @param config Configuración de calibración con rangos de entrada/salida
     */
    Calibrator(CalibrationConfig config);

    /**
     * @brief Mapea un valor del rango de entrada al rango de salida.
     * @param value Valor a mapear (típicamente del ADC)
     * @return Valor mapeado y constraineado al rango de salida
     */
    int map(int value);

private:
    CalibrationConfig _config; ///< Configuración de calibración almacenada
};