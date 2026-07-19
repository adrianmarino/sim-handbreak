#pragma once

/**
 * @class MovingAverageFilter
 * @brief Filtro de media móvil para suavizado de señales.
 * 
 * Implementa un filtro de media móvil circular (ring buffer) para
 * reducir ruido en lecturas de sensores analógicos. El filtro
 * promedia las últimas N muestras para obtener un valor estable.
 * 
 * @note Usa asignación dinámica de memoria para el buffer circular
 * 
 * @code
 * MovingAverageFilter filter(8);
 * filter.begin();
 * 
 * // En loop:
 * int raw = analogRead(A3);
 * int filtered = filter.process(raw);
 * @endcode
 * 
 * @see AnalogSensor para lectura de hardware
 * @see Calibrator para mapeo de valores
 */
class MovingAverageFilter {
public:
    /**
     * @brief Constructor del filtro de media móvil.
     * @param samples Número de muestras para el promedio (ventana del filtro)
     * @note Mayor samples = más suavizado pero mayor latencia
     */
    MovingAverageFilter(int samples);

    /**
     * @brief Destructor - libera memoria del buffer circular.
     */
    ~MovingAverageFilter();

    /**
     * @brief Inicializa el filtro en cero.
     * @note Debe llamarse antes del primer process()
     */
    void begin();

    /**
     * @brief Procesa una nueva muestra y retorna el promedio.
     * @param newValue Nuevo valor a agregar al filtro
     * @return Promedio de las últimas N muestras
     */
    int process(int newValue);

private:
    int _samples;    ///< Tamaño de la ventana del filtro
    int* _readings;  ///< Buffer circular de muestras
    int _readIndex;  ///< Índice actual en el buffer circular
    long _total;     ///< Acumulador para cálculo eficiente del promedio
};