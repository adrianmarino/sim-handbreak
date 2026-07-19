#pragma once
#include <Arduino.h>

/**
 * @class AnalogSensor
 * @brief Clase para lectura de sensores analógicos.
 * 
 * Encapsula la lectura de pines analógicos del Arduino,
 * proporcionando una interfaz simple y reutilizable para
 * sensores que retornan valores en el rango del ADC (0-1023).
 * 
 * @note Requiere llamada a begin() antes de usar read()
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
     * @brief Constructor de AnalogSensor.
     * @param pin Número de pin analógico (ej: A0, A1, A3)
     */
    AnalogSensor(uint8_t pin);

    /**
     * @brief Inicializa el sensor configurando el pin como entrada.
     * @note Debe llamarse en setup() antes de read()
     */
    void begin();

    /**
     * @brief Lee el valor actual del sensor analógico.
     * @return Valor ADC en rango [0, 1023]
     */
    int read();

private:
    uint8_t _pin; ///< Pin analógico asociado al sensor
};