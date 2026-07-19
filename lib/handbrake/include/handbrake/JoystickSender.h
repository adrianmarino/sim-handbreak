#pragma once
#include <Joystick.h>

/**
 * @class JoystickSender
 * @brief Clase para envío de reportes USB HID Joystick.
 * 
 * Gestiona la comunicación USB HID con el host, enviando
 * reportes de joystick USB utilizando la librería mheironimus/Joystick.
 * Configura el eje Ry (Throttle) para simular freno de mano.
 * 
 * @note Usa begin(false) para control manual de envío de reportes
 * 
 * @code
 * JoystickSender sender;
 * sender.begin();
 * 
 * // En loop:
 * sender.send(512); // Envía valor al eje Ry
 * @endcode
 * 
 * @see HandbrakeController para orquestación completa
 */
class JoystickSender {
public:
    /**
     * @brief Constructor de JoystickSender.
     */
    JoystickSender();

    /**
     * @brief Inicializa el joystick USB.
     * @note Configura modo de envío manual (begin(false))
     * @note Establece rango del eje Throttle: 0-1023
     */
    void begin();

    /**
     * @brief Envía un valor al eje Ry del joystick USB.
     * @param value Valor a enviar (rango: 0-1023)
     * @note Actualiza automáticamente el reporte USB
     */
    void send(int value);

private:
    Joystick_ _joystick; ///< Instancia del joystick USB
};