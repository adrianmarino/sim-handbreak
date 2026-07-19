# Sim Handbrake

[![PlatformIO](https://img.shields.io/badge/PlatformIO-Library-orange)](https://platformio.org/)
[![Arduino](https://img.shields.io/badge/Arduino-Leonardo-teal)](https://www.arduino.cc/en/Main/ArduinoBoardLeonardo)
[![License](https://img.shields.io/badge/License-MIT-blue.svg)](LICENSE)

**Simulador de freno de mano USB para sim racing.**

Convierte la lectura analógica de un potenciómetro en un eje de joystick USB reconocido automáticamente por simuladores de carreras como iRacing, Assetto Corsa, Gran Turismo, F1, entre otros.

---

## Tabla de contenidos

- [Características](#características)
- [Hardware requerido](#hardware-requerido)
- [Diagrama de conexión](#diagrama-de-conexión)
- [Instalación](#instalación)
- [Uso](#uso)
- [Calibración](#calibración)
- [Arquitectura del software](#arquitectura-del-software)
- [Estructura del proyecto](#estructura-del-proyecto)
- [API de clases](#api-de-clases)
- [Configuración](#configuración)
- [Solución de problemas](#solución-de-problemas)
- [Mejoras futuras](#mejoras-futuras)
- [Licencia](#licencia)

---

## Características

- ✅ **USB HID nativo** - No requiere drivers adicionales
- ✅ **Filtro de ruido** - Media móvil configurable para lecturas estables
- ✅ **Calibración** - Mapeo personalizable de valores del potenciómetro
- ✅ **Alta frecuencia** - Actualización a 100Hz (10ms)
- ✅ **Arquitectura SOLID** - Código modular y mantenible
- ✅ **Compatible** - iRacing, Assetto Corsa, F1, Gran Turismo, etc.

---

## Hardware requerido

| Componente | Cantidad | Descripción |
|------------|----------|-------------|
| Arduino Leonardo | 1 | Microcontrolador con USB nativo (ATmega32U4) |
| Potenciómetro lineal | 1 | 10KΩ recomendado (0-10KΩ) |
| Cable USB | 1 | Micro-USB (soporte datos, no solo carga) |
| Protoboard/Cables | - | Para conexiones temporales |

### Compatibilidad

- **Arduino Leonardo** (recomendado) - USB HID nativo
- **Arduino Pro Micro** - Mismo chip ATmega32U4
- **SparkFun Pro Micro** - Alternativa compatible

> **Nota**: No se recomienda Arduino Uno/Nano para esta aplicación ya que no tienen USB HID nativo.

---

## Diagrama de conexión

```mermaid
graph TD
    subgraph Arduino["Arduino Leonardo"]
        A3["A3 - Analog Input"]
        VCC["5V"]
        GND_A["GND"]
    end
    
    subgraph Pot["Potenciómetro 10KΩ"]
        P1["Terminal 1<br/>VCC"]
        P2["Terminal 2<br/>Wiper"]
        P3["Terminal 3<br/>GND"]
    end
    
    VCC -->|"Power"| P1
    P2 -->|"Signal"| A3
    P3 -->|"Ground"| GND_A
    
    style Arduino fill:#e3f2fd,stroke:#1565c0,stroke-width:2px
    style Pot fill:#fff3e0,stroke:#ef6c00,stroke-width:2px
    style A3 fill:#4caf50,color:#fff
    style VCC fill:#f44336,color:#fff
    style GND_A fill:#333,color:#fff
    style P2 fill:#ff9800,color:#fff
```

### Pinout detallado

| Potenciómetro | Arduino Leonardo |
|---------------|------------------|
| Terminal 1 (VCC) | 5V |
| Terminal 2 (Wiper) | A3 |
| Terminal 3 (GND) | GND |

---

## Instalación

### Requisitos previos

1. **PlatformIO** instalado (CLI o VS Code)
2. **Arduino Leonardo** conectado via USB

### Pasos

```bash
# 1. Clonar el repositorio
git clone https://github.com/tu-usuario/sim-handbreak.git
cd sim-handbreak

# 2. Instalar dependencias (automático con PlatformIO)
pio run

# 3. Subir al Arduino
pio run -t upload

# 4. (Opcional) Monitorear serial
pio device monitor
```

### Desde VS Code

1. Abrir la carpeta del proyecto en VS Code
2. Instalar extensión PlatformIO IDE
3. Click en "Upload" (flecha hacia arriba)
4. Click en "Serial Monitor" para ver logs

---

## Uso

### Verificación en Windows

1. Conectar el Arduino Leonardo
2. Abrir **Panel de control** → **Dispositivos de juego**
3. Buscar "Handbrake" en la lista
4. Probar los ejes moviendo el potenciómetro

### Verificación en Linux

```bash
# Instalar herramienta de prueba
sudo apt install joystick

# Probar el joystick
jstest /dev/input/js0
```

### Verificación en macOS

```bash
# Usar JoyStick Show
open "JoyStick Show.app"
```

### Configurar en simuladores

1. **iRacing**: Opciones → Calibrar freno de mano → Asignar eje
2. **Assetto Corsa**: Opciones → Controles → Freno de mano → Asignar eje
3. **F1**: Opciones → Ajustes de control → Freno de mano → Asignar eje
4. **Gran Turismo**: Ajustes → Controles → Freno de mano

---

## Calibración

### Valores por defecto

```cpp
const int ADC_REPOSO = 945;    // Valor ADC en reposo
const int ADC_A_FONDO = 735;   // Valor ADC a fondo de escala
```

### Proceso de calibración

1. **Medir valor en reposo** (sin tocar el freno):
   ```bash
   # Con Arduino conectado, abrir monitor serial
   pio device monitor
   # Anotar el valor cuando el freno está suelto
   ```

2. **Medir valor a fondo de escala** (freno total):
   ```bash
   # Tirar del freno completamente
   # Anotar el valor máximo
   ```

3. **Actualizar constantes** en `src/main.cpp`:
   ```cpp
   const int ADC_REPOSO = 950;    // Tu valor medido
   const int ADC_A_FONDO = 720;   // Tu valor medido
   ```

4. **Recompilar y subir**:
   ```bash
   pio run -t upload
   ```

### Ajuste fino

Si el rango no es lineal, puedes ajustar los valores en `CalibrationConfig`:

```cpp
CalibrationConfig config = {
    .inputMin = 950,    // Valor reposo medido
    .inputMax = 720,    // Valor fondo medido
    .outputMin = 0,     // Mínimo joystick
    .outputMax = 1023   // Máximo joystick
};
```

---

## Arquitectura del software

### Principios SOLID aplicados

| Principio | Implementación |
|-----------|----------------|
| **SRP** | Cada clase tiene UNA responsabilidad |
| **OCP** | Calibrator extensible con CalibrationConfig |
| **LSP** | Componentes intercambiables |
| **ISP** | Interfaces pequeñas y específicas |
| **DIP** | Inyección de dependencias en HandbrakeController |

### Diagrama de clases

```mermaid
classDiagram
    class AnalogSensor {
        -uint8_t _pin
        +AnalogSensor(uint8_t pin)
        +void begin()
        +int read()
    }
    
    class MovingAverageFilter {
        -int _samples
        -int* _readings
        -int _readIndex
        -long _total
        +MovingAverageFilter(int samples)
        +~MovingAverageFilter()
        +void begin()
        +int process(int newValue)
    }
    
    class Calibrator {
        -CalibrationConfig _config
        +Calibrator(CalibrationConfig config)
        +int map(int value)
    }
    
    class JoystickSender {
        -Joystick_ _joystick
        +JoystickSender()
        +void begin()
        +void send(int value)
    }
    
    class HandbrakeController {
        -AnalogSensor* _sensor
        -MovingAverageFilter* _filter
        -Calibrator* _calibrator
        -JoystickSender* _sender
        +HandbrakeController(sensor, filter, calibrator, sender)
        +void begin()
        +void update()
    }
    
    HandbrakeController --> AnalogSensor : uses
    HandbrakeController --> MovingAverageFilter : uses
    HandbrakeController --> Calibrator : uses
    HandbrakeController --> JoystickSender : uses
    
    style HandbrakeController fill:#e91e63,color:#fff
    style AnalogSensor fill:#4caf50,color:#fff
    style MovingAverageFilter fill:#2196f3,color:#fff
    style Calibrator fill:#ff9800,color:#fff
    style JoystickSender fill:#9c27b0,color:#fff
```

### Flujo de datos

```mermaid
graph LR
    P["Potenciómetro<br/>0-1023"] --> S["AnalogSensor<br/>Lectura HW"]
    S --> F["MovingAverageFilter<br/>Suavizado"]
    F --> C["Calibrator<br/>Mapeo"]
    C --> J["JoystickSender<br/>USB HID"]
    J --> U["Simulador<br/>iRacing/AC"]
    
    style P fill:#607d8b,color:#fff
    style S fill:#4caf50,color:#fff
    style F fill:#2196f3,color:#fff
    style C fill:#ff9800,color:#fff
    style J fill:#9c27b0,color:#fff
    style U fill:#f44336,color:#fff
```

### Diagrama de secuencia

```mermaid
sequenceDiagram
    participant P as Potenciómetro
    participant S as AnalogSensor
    participant F as MovingAverageFilter
    participant C as Calibrator
    participant J as JoystickSender
    participant U as USB HID
    
    loop Cada 10ms (100Hz)
        P->>S: Valor analógico
        S->>F: rawValue [0-1023]
        F->>C: filteredValue
        C->>J: mappedValue [0-1023]
        J->>U: Reporte USB
        U->>U: Simulador procesa
    end
    
    Note over P,U: Pipeline completo en cada ciclo
```

### Patrones de diseño

- **Facade**: HandbrakeController orquesta todo el pipeline
- **Dependency Injection**: Componentes inyectados por puntero
- **Single Responsibility**: Cada clase hace una cosa

---

## Estructura del proyecto

```
sim-handbreak/
├── src/
│   └── main.cpp                    # Punto de entrada - solo configuración
├── lib/
│   └── handbrake/                  # Biblioteca principal
│       ├── include/
│       │   └── handbrake/
│       │       ├── AnalogSensor.h      # Lectura de hardware
│       │       ├── MovingAverageFilter.h  # Filtrado de señal
│       │       ├── Calibrator.h        # Mapeo y calibración
│       │       ├── JoystickSender.h    # Envío USB
│       │       └── HandbrakeController.h # Orquestador
│       └── src/
│           ├── AnalogSensor.cpp
│           ├── MovingAverageFilter.cpp
│           ├── Calibrator.cpp
│           ├── JoystickSender.cpp
│           └── HandbrakeController.cpp
├── platformio.ini                 # Configuración PlatformIO
├── AGENTS.md                      # Documentación para agentes
└── README.md                      # Este archivo
```

---

## API de clases

### AnalogSensor

```cpp
AnalogSensor sensor(A3);  // Pin analógico
sensor.begin();           // Inicializar
int value = sensor.read(); // Leer [0-1023]
```

### MovingAverageFilter

```cpp
MovingAverageFilter filter(8);  // 8 muestras
filter.begin();                 // Inicializar
int filtered = filter.process(rawValue);  // Filtrar
```

### Calibrator

```cpp
CalibrationConfig config = {945, 735, 0, 1023};
Calibrator calibrator(config);
int mapped = calibrator.map(filteredValue);  // Mapear
```

### JoystickSender

```cpp
JoystickSender sender;
sender.begin();      // Inicializar USB
sender.send(value);  // Enviar eje Ry
```

### HandbrakeController

```cpp
HandbrakeController handbrake(&sensor, &filter, &calibrator, &sender);
handbrake.begin();   // Inicializar todo
handbrake.update();  // Ejecutar pipeline completo
```

---

## Configuración

### Parámetros principales

| Archivo | Constante | Valor | Descripción |
|---------|-----------|-------|-------------|
| `main.cpp` | `POT_PIN` | A3 | Pin analógico del potenciómetro |
| `main.cpp` | `FILTER_SAMPLES` | 8 | Tamaño de ventana del filtro |
| `main.cpp` | `ADC_REPOSO` | 945 | Valor ADC en reposo |
| `main.cpp` | `ADC_A_FONDO` | 735 | Valor ADC a fondo de escala |

### Frecuencia de actualización

El sistema opera a **100Hz** (intervalo de 10ms):

```cpp
delay(10);  // 1000ms / 10ms = 100Hz
```

### Configuración USB

En `platformio.ini`:

```ini
board_build.usb_product = "Handbrake"
board_build.usb_manufacturer = "Adrian"
```

---

## Solución de problemas

### El joystick no aparece

1. Verificar que el cable USB soporte datos
2. Probar otro puerto USB
3. Verificar que `board_build.usb_product` esté configurado

### Lecturas inestables

1. **Aumentar FILTER_SAMPLES** (ej: 16 o 32)
2. **Verificar conexiones** - cables flojos causan ruido
3. **Agregar condensador** de 100nF entre A3 y GND

### Valores invertidos

Si el freno funciona al revés, intercambiar los valores:

```cpp
const int ADC_REPOSO = 735;    // Antes era ADC_A_FONDO
const int ADC_A_FONDO = 945;   // Antes era ADC_REPOSO
```

### Compilación fallida

```bash
# Limpiar y recompilar
pio run -t clean && pio run

# Verificar librerías instaladas
pio pkg list
```

### Upload fallido

1. Presionar botón de reset **dos veces rápido** en el Arduino
2. Ejecutar upload inmediatamente:
   ```bash
   pio run -t upload
   ```

---

## Mejoras futuras

- [ ] **Zona muerta configurable** - Deadzone para el centro
- [ ] **Múltiples presets** - Guardar configuraciones en EEPROM
- [ ] **Auto-calibración** - Rutina automática al iniciar
- [ ] **LED indicador** - Estado del freno de mano
- [ ] **Múltiples ejes** - Asignar a diferentes ejes USB
- [ ] **Firmware OTA** - Actualización via WiFi (ESP32)
- [ ] **Modo analógico/digital** - Cambio entre modos
- [ ] **Curva de respuesta** - Configurar lineal/exponencial

---

## Contribuir

1. Fork el proyecto
2. Crear branch (`git checkout -b feature/nueva-funcionalidad`)
3. Commit (`git commit -m 'Agregar nueva funcionalidad'`)
4. Push (`git push origin feature/nueva-funcionalidad`)
5. Abrir Pull Request

---

## Licencia

MIT License - Ver [LICENSE](LICENSE) para detalles.

---

## Créditos

- [mheironimus/Joystick](https://github.com/mheironimus/Joystick) - Librería USB HID Joystick
- [PlatformIO](https://platformio.org/) - Sistema de build
- [Arduino](https://www.arduino.cc/) - Plataforma de hardware