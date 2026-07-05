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
// EXPERIMENTO
// =====================================================

// Porcentajes que se aplicarán automáticamente
const float nivelesPWM[] = {
  3.7,
  7.4,
  11.1,
  14.8,
  18.5,
  22.2,
  25.9,
  29.6,
  33.3,
  37.0
};

const int NUM_NIVELES = sizeof(nivelesPWM) / sizeof(nivelesPWM[0]);

const unsigned long DURACION_NIVEL_MS = 10000; // 8 s por nivel
const unsigned long MUESTREO_MS = 10;         // 10 ms

int nivelActual = 0;

unsigned long tiempoInicioNivel = 0;
unsigned long tiempoAnterior = 0;
unsigned long tiempoInicioExperimento = 0;

// =====================================================
// PROTOTIPOS
// =====================================================

void configurarPWM12bits();
void escribirPWM12bits(uint16_t pwm);
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

  delay(3000);

  tiempoInicioExperimento = millis();
  tiempoInicioNivel = millis();

  aplicarPorcentaje(nivelesPWM[nivelActual]);

  Serial.println("tiempo_s,pwm_percent,pwm_12bits,angulo_deg");
}

// =====================================================
// LOOP
// =====================================================

void loop() {
  unsigned long tiempoActual = millis();

  // Cambiar al siguiente nivel
  if (tiempoActual - tiempoInicioNivel >= DURACION_NIVEL_MS) {
    nivelActual++;

    if (nivelActual >= NUM_NIVELES) {
      escribirPWM12bits(0);
      Serial.println("FIN");
      while (true); // Termina el experimento
    }

    tiempoInicioNivel = tiempoActual;
    aplicarPorcentaje(nivelesPWM[nivelActual]);
  }

  // Leer y enviar datos cada 10 ms
  if (tiempoActual - tiempoAnterior >= MUESTREO_MS) {
    tiempoAnterior += MUESTREO_MS;

    int lecturaADC = analogRead(SENSOR_PIN);
    int lecturaCorregida = lecturaADC - OFFSET_ADC;

    float anguloGrados =
      lecturaCorregida * 360.0 / 1023.0;

    uint16_t pwmValor =
      round(nivelesPWM[nivelActual] * PWM_MAX / 100.0);

    float tiempo =
      (tiempoActual - tiempoInicioExperimento) / 1000.0;

    Serial.print("Tiempo: ");
    Serial.print(tiempo, 3);
    Serial.print(", ");

    Serial.print("PWM_procentaje: ");
    Serial.print(nivelesPWM[nivelActual], 2);
    Serial.print(", ");

    Serial.print("Grados: ");
    Serial.println(anguloGrados, 3);
  }
}

// =====================================================
// APLICAR PWM EN PORCENTAJE
// =====================================================

void aplicarPorcentaje(float porcentaje) {
  porcentaje = constrain(porcentaje, 0.0, 100.0);

  uint16_t pwm =
    round(porcentaje * PWM_MAX / 100.0);

  escribirPWM12bits(pwm);
}

// =====================================================
// CONFIGURAR TIMER 1 PARA PWM DE 12 BITS
// =====================================================

void configurarPWM12bits() {
  // Pin 9 = OC1A
  DDRB |= _BV(PB1);

  // Fast PWM, modo 14, salida no inversora
  TCCR1A = _BV(COM1A1) | _BV(WGM11);

  // Sin preescalador
  TCCR1B =
      _BV(WGM13) |
      _BV(WGM12) |
      _BV(CS10);

  ICR1 = PWM_MAX;
  OCR1A = 0;
}

// =====================================================
// ESCRIBIR PWM DE 0 A 4095
// =====================================================

void escribirPWM12bits(uint16_t pwm) {
  pwm = constrain(pwm, 0, PWM_MAX);
  OCR1A = pwm;
}