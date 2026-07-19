#pragma once
#include "AnalogSensor.h"
#include "MovingAverageFilter.h"
#include "Calibrator.h"
#include "JoystickSender.h"

/**
 * @class HandbrakeController
 * @brief Facade que orquesta todo el pipeline del freno de mano.
 * 
 * Implementa el patrón Facade para coordinar los componentes
 * del sistema: sensor, filtro, calibrador y sender. Sigue el
 * principio de inversión de dependencias (DIP) recibiendo las
 * dependencias por inyección.
 * 
 * ## Flujo de datos
 * ```
 * Sensor → Filtro → Calibrador → Sender USB
 * ```
 * 
 * @code
 * // Configurar componentes
 * AnalogSensor sensor(A3);
 * MovingAverageFilter filter(8);
 * Calibrator calibrator({945, 735, 0, 1023});
 * JoystickSender sender;
 * 
 * // Crear controller con inyección
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
 * @see AnalogSensor para lectura de hardware
 * @see MovingAverageFilter para filtrado de señal
 * @see Calibrator para mapeo de valores
 * @see JoystickSender para envío USB
 */
class HandbrakeController {
public:
    /**
     * @brief Constructor con inyección de dependencias.
     * @param sensor Puntero al sensor analógico
     * @param filter Puntero al filtro de media móvil
     * @param calibrator Puntero al calibrador de valores
     * @param sender Puntero al enviador USB
     * 
     * @pre Todos los punteros deben ser válidos
     * @post El controller toma posesión lógica de los componentes
     */
    HandbrakeController(
        AnalogSensor* sensor,
        MovingAverageFilter* filter,
        Calibrator* calibrator,
        JoystickSender* sender
    );

    /**
     * @brief Inicializa todos los componentes del pipeline.
     * @note Llama begin() a cada componente inyectado
     * @note Debe llamarse en setup()
     */
    void begin();

    /**
     * @brief Ejecuta un ciclo completo del pipeline.
     * @note Lee → Filtra → Calibra → Envía
     * @note Llamar en loop() con intervalo de 10ms (100Hz)
     */
    void update();

private:
    AnalogSensor* _sensor;          ///< Sensor analógico inyectado
    MovingAverageFilter* _filter;   ///< Filtro de media móvil inyectado
    Calibrator* _calibrator;        ///< Calibrador de valores inyectado
    JoystickSender* _sender;        ///< Sender USB inyectado
};