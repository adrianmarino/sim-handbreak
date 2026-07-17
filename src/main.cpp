#include <Arduino.h>
#include <Joystick.h>

const int POT_PIN = A3;

// Configuracion de Joystick de un solo eje compatible con Linux y Windows:
// Declaramos solo 1 boton para forzar la deteccion de joydev de Linux, pero
// deshabilitamos por completo el eje X (includeXAxis = false, includeYAxis = true).
// El dispositivo tendra fisicamente solo el Eje Y (1 solo eje analogo).
Joystick_ Joystick(
  JOYSTICK_DEFAULT_REPORT_ID, 
  JOYSTICK_TYPE_GAMEPAD,
  1, 0,          // 1 boton de control minimo y 0 hats
  false, true,   // Eje X INHABILITADO, Eje Y HABILITADO (solo 1 eje real en el reporte)
  false, false, false, false, 
  false, false, false, false, false
);

// Filtro de media movil para reducir ruido analogico
const int FILTER_SAMPLES = 8;
int readings[FILTER_SAMPLES];
int readIndex = 0;
long total = 0;

void setup() {
  pinMode(POT_PIN, INPUT);
  
  for (int i = 0; i < FILTER_SAMPLES; i++) {
    readings[i] = 0;
  }
  
  Joystick.begin(false); // begin(false) para control de envio manual
  Joystick.setYAxisRange(0, 1023); // Solo configuramos el rango del eje Y
}

void loop() {
  // 1. Leer analoogico y aplicar filtro de media movil
  total = total - readings[readIndex];
  readings[readIndex] = analogRead(POT_PIN);
  total = total + readings[readIndex];
  readIndex = (readIndex + 1) % FILTER_SAMPLES;
  
  int filteredValue = total / FILTER_SAMPLES;
  
  // Calibracion real medida: Reposo = 955, A Fondo = 725.
  // Aplicamos zona muerta de seguridad:
  const int ADC_REPOSO = 945;    
  const int ADC_A_FONDO = 735;   

  int mappedValue = map(filteredValue, ADC_REPOSO, ADC_A_FONDO, 0, 1023);
  mappedValue = constrain(mappedValue, 0, 1023); 

  // 2. Transmitir el valor calibrado unicamente en el eje Y
  Joystick.setYAxis(mappedValue);
  Joystick.sendState(); // Enviar reporte USB manual
  
  delay(10); // Frecuencia de 100Hz (intervalo de 10ms)
}
