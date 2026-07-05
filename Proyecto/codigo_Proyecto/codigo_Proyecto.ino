int pinSensor = A2;

void setup() {
  Serial.begin(9600);
}

void loop() {
  int lectura = analogRead(pinSensor);
// Definir variables
#define Sensor A1                 // Pin del sensor analógico
#define OutputPWM_GPIO 9           // Pin de salida PWM para el control de la hélice
#define pwmRes 12                  // Resolución del PWM (12 bits)
#define pwmMax 4095                // Valor máximo para el PWM (4095 para 12 bits)

// Variables para la conversión y salida
#define Uunits 100                 // Unidades para la salida de control (u) [mA]

// Variables de tiempo de ejecución
unsigned long pTime = 0;
unsigned long dTime = 0;
long previousMillis = 0;          // Para la función del bucle principal
long Ts = 10;                   // Tiempo de muestreo en ms
long previousMillis2 = 0;         // Para funciones auxiliares
bool up = true;
int i = 0;
float ts = 0.010;

// Advanced Serial Input Variables
const byte numChars = 32;
char receivedChars[numChars];
boolean newData = false;

// Variables de medición
float sensorValue = 0.0;         // Valor leído del sensor analógico
float angulo = 0.0;

// Variables de control del sistema
float Ref = 20;                // angulo de referencia        
float U_t = 0.0;                 // Salida de control (PWM)
unsigned int pwmDuty = 0;        // Ciclo de trabajo del PWM

// Variables para el controlador PID
float k_p = 0.3;    
float k_i = 0.01;
float k_d = 0.028;
int N = 100;
float e_n = 0.0, e_n_1 = 0.0;
float u_n = 0.0 , u_p = 0.0, u_i = 0.0, u_d = 0.0,  u_n_1_i = 0.0, u_n_1_d = 0.0, u_n_1 = 0.0;

// Variables de control
unsigned long pwmStartTime = 0;   // Variable para el tiempo de inicio del PWM constante
unsigned long pwmDuration = 40000; // Duración del PWM constante en milisegundos (3 segundos en este caso)
bool pwmConstant = true;    

//Variables red
float a = 0.1567;
float T1 = 29.1445; 
float b = 2.0538;
float T2 = 9.5981; 
float u_n_m = 0.0, u_n_m_1 = 0.0; 
float u_n_a = 0.0, u_n_a_1 = 0.0;



// Función de calibración
void calibracion(void) {
    unsigned long currentMillis = millis(); // Actualizar el tiempo actual
    if (currentMillis - previousMillis >= Ts) {
        previousMillis = currentMillis;

        e_n = Ref - angulo;

              // Leer el valor del sensor analógico
        sensorValue = max(analogRead(Sensor) - 316, 0);  // Lee el valor del sensor
        angulo = (sensorValue / 1023.0) * 360.0; // Ángulo en grados

        
          u_p = k_p*e_n;
          u_i = (k_i*ts*e_n_1) + u_n_1_i;
          u_d = (k_d*N*e_n) - (k_d*N*e_n_1) - (N*ts*u_n_1_d) + u_n_1_d;
          u_n = u_p + u_i + u_d;

         u_n_m = (ts*(u_p + u_i + u_d) + ts*(u_p + u_n_1_i + u_n_1_d) + 2*a*T1*(u_p + u_i + u_d) - 2*a*T1*(u_p + u_n_1_i + u_n_1_d) + 2*T1*u_n_m_1 - ts*u_n_m_1) / (2 * T1 + ts);
         u_n_a = (ts*u_n_m + ts*u_n_m_1 + 2*b*T2*u_n_m - 2*b*T2*u_n_m_1 + 2*T2*u_n_a_1 - ts*u_n_a_1) / (2 * T2 + ts);

     
        float U_tl = min(max((u_n_m), 0), Uunits); // Control saturado
        pwmDuty = int((U_tl / Uunits) * pwmMax); // Convertir a ciclo de trabajo PWM
        analogWriteADJ(OutputPWM_GPIO, pwmDuty); // Escribir el valor de PWM en el pin

           e_n_1 = e_n;
           u_n_1_i = u_i;
           u_n_1_d = u_d;
           u_n_m_1 = u_n_m;
           u_n_a_1 = u_n_a;

           

     

        // Enviar datos al monitor serial
        Serial.print("Tiempo: ");
        Serial.print(millis()/100);
        Serial.print(", ");
        Serial.print("angulo: ");
        Serial.print(angulo);
        Serial.print(", ");
        Serial.print("PWM: ");
        Serial.print(((pwmDuty * 100.0) / pwmMax));
        Serial.print("%");
        Serial.print(", ");
        Serial.print("error: ");
        Serial.print(e_n);
        Serial.print(", ");
        Serial.print("U_n: ");
        Serial.print(u_n);
        Serial.print(", ");
        Serial.print("U_n_m: ");
        Serial.println(u_n_a);
    }

    // Funciones de entrada serial avanzadas
    recvWithStartEndMarkers();  
    if (newData == true) {
        parseData();
        newData = false;
    }
}

// Configuración del PWM
void setupPWMadj() {
    DDRB |= _BV(PB1) | _BV(PB2);        /* set pins as outputs */
    TCCR1A = _BV(COM1A1) | _BV(COM1B1)  /* non-inverting PWM */
        | _BV(WGM11);                   /* mode 14: fast PWM, TOP=ICR1 */
    TCCR1B = _BV(WGM13) | _BV(WGM12)
        | _BV(CS10);                    /* no prescaling */
    ICR1 = 0x0fff;                      /* TOP counter value - SETS RESOLUTION/FREQUENCY */
}

// Versión de analogWrite() de 12 bits
void analogWriteADJ(uint8_t pin, uint16_t val) {
    switch (pin) {
        case 9: OCR1A = val; break;
        case 10: OCR1B = val; break;
    }
}

// Funciones avanzadas para la entrada serial
void recvWithStartEndMarkers() {
    static boolean recvInProgress = false;
    static byte ndx = 0;
    char startMarker = '<'; // La entrada serial debe comenzar con este carácter
    char endMarker = '>'; // La entrada serial debe terminar con este carácter
    char rc;

    while (Serial.available() > 0 && newData == false) {
        rc = Serial.read();

        if (recvInProgress == true) {
            if (rc != endMarker) {
                receivedChars[ndx] = rc;
                ndx++;
                if (ndx >= numChars) {
                    ndx = numChars - 1;
                }
            }
            else {
                receivedChars[ndx] = '\0'; // Terminar la cadena
                recvInProgress = false;
                ndx = 0;
                newData = true;
            }
        }
        else if (rc == startMarker) {
            recvInProgress = true;
        }
    }
}

// Función para parsear los datos recibidos
void parseData() {
    Ref = atof(receivedChars); // Convertir la entrada serial a un valor flotante y actualizar la referencia
}

void setup() {
    Serial.begin(9600); // Iniciar la comunicación serial a alta velocidad
    
    // Configuración de entrada analógica
    pinMode(Sensor, INPUT); // Configurar el pin del sensor como entrada

    // Configuración del PWM
    setupPWMadj();
    analogWriteADJ(OutputPWM_GPIO, pwmDuty);

    delay(5000); // Esperar 5 segundos antes de iniciar el monitor serial
}

void loop() {
    calibracion();
}
  Serial.print("Lectura analogica: ");
  Serial.println(lectura);

  delay(300);
}