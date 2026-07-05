#include <Arduino.h>

// =====================================================
// PINES
// =====================================================

const byte SENSOR_PIN = A3;
const byte PWM_PIN = 9;
const byte IN1_PIN = 7;
const byte IN2_PIN = 8;

// =====================================================
// PWM DE 12 BITS
// =====================================================

const uint16_t PWM_MAX = 4095;

// =====================================================
// SENSOR
// =====================================================

const int OFFSET_ADC = 410;

// =====================================================
// EXPERIMENTO DE ESCALONES
// =====================================================

// Cambia aquí los porcentajes
const float escalonesPWM[] = {
  0.0,
  18.0,
  21.0,
  13.0,
  19.0,
  14.0,
  24.0,
  10.0,
  18.0,



};

const int NUM_ESCALONES =
  sizeof(escalonesPWM) / sizeof(escalonesPWM[0]);

// Duración de cada escalón
const unsigned long DURACION_ESCALON_MS = 10000;

// Muestreo cada 10 ms
const unsigned long MUESTREO_MS = 10;

int escalonActual = 0;

unsigned long tiempoInicio = 0;
unsigned long tiempoInicioEscalon = 0;
unsigned long tiempoAnterior = 0;

// =====================================================
// PROTOTIPOS
// =====================================================

void configurarPWM12bits();
void escribirPWM12bits(uint16_t valor);
void aplicarPorcentaje(float porcentaje);

// =====================================================
// SETUP
// =====================================================

void setup() {
  Serial.begin(115200);

  pinMode(SENSOR_PIN, INPUT);
  pinMode(PWM_PIN, OUTPUT);
  pinMode(IN1_PIN, OUTPUT);
  pinMode(IN2_PIN, OUTPUT);

  // Dirección fija del motor
  digitalWrite(IN1_PIN, HIGH);
  digitalWrite(IN2_PIN, LOW);

  configurarPWM12bits();
  escribirPWM12bits(0);

  Serial.println("tiempo_s,pwm_percent,pwm_12bits,angulo_deg");

  delay(3000);

  tiempoInicio = millis();
  tiempoInicioEscalon = millis();

  aplicarPorcentaje(escalonesPWM[escalonActual]);
}

// =====================================================
// LOOP
// =====================================================

void loop() {
  unsigned long tiempoActual = millis();

  // Cambiar al siguiente escalón
  if (tiempoActual - tiempoInicioEscalon >= DURACION_ESCALON_MS) {
    escalonActual++;

    if (escalonActual >= NUM_ESCALONES) {
      escribirPWM12bits(0);
      Serial.println("FIN");
      while (true) {
        // Experimento terminado
      }
    }

    tiempoInicioEscalon = tiempoActual;
    aplicarPorcentaje(escalonesPWM[escalonActual]);
  }

  // Lectura y envío de datos
  if (tiempoActual - tiempoAnterior >= MUESTREO_MS) {
    tiempoAnterior += MUESTREO_MS;

    int lecturaADC = analogRead(SENSOR_PIN);
    int lecturaCorregida = lecturaADC - OFFSET_ADC;

    float anguloGrados =
      lecturaCorregida * 360.0 / 1023.0;

    uint16_t pwmValor = round(
      escalonesPWM[escalonActual] * PWM_MAX / 100.0
    );

    float tiempoSegundos =
      (tiempoActual - tiempoInicio) / 1000.0;

    Serial.print("Tiempo: ");
    Serial.print(tiempoSegundos, 2);
    Serial.print(" , ");

    Serial.print("PWM: ");
    Serial.print(escalonesPWM[escalonActual], 2);
    Serial.print(" , ");

    Serial.print("Angulo: ");
    Serial.println(anguloGrados, 3);
  }
}

// =====================================================
// APLICAR PWM EN PORCENTAJE
// =====================================================

void aplicarPorcentaje(float porcentaje) {
  porcentaje = constrain(porcentaje, 0.0, 100.0);

  uint16_t pwmValor = round(
    porcentaje * PWM_MAX / 100.0
  );

  escribirPWM12bits(pwmValor);
}

// =====================================================
// TIMER 1: PWM DE 12 BITS EN PIN 9
// =====================================================

void configurarPWM12bits() {
  // Pin 9 = OC1A
  DDRB |= _BV(PB1);

  // Fast PWM, modo 14, salida no inversora
  TCCR1A =
      _BV(COM1A1) |
      _BV(WGM11);

  // Sin preescalador
  TCCR1B =
      _BV(WGM13) |
      _BV(WGM12) |
      _BV(CS10);

  ICR1 = PWM_MAX;
  OCR1A = 0;
}

// =====================================================
// ESCRIBIR PWM ENTRE 0 Y 4095
// =====================================================

void escribirPWM12bits(uint16_t valor) {
  if (valor > PWM_MAX) {
    valor = PWM_MAX;
  }

  OCR1A = valor;
}