# Proyecto Final: Control de Posición de un Péndulo propulsado por Hélice (Motor DC)

Este directorio contiene todo el código, modelos y diseño teórico de un sistema de control de lazo cerrado para mantener un péndulo en una posición específica (70°) utilizando el empuje generado por una hélice acoplada a un motor DC.

## 🛠 Arquitectura del Proyecto

El proyecto está dividido en tres pilares fundamentales:

1.  **Identificación y Diseño Teórico (MATLAB)**
2.  **Simulación y Validación de Robustez (Simulink)**
3.  **Implementación Física (Arduino)**

### 1. Modelado Matemático y MATLAB (`.mlx`)
Se realizó una caracterización física de la planta obteniendo la inercia ($J$), fricción viscosa ($b_p$), constante de torque ($K_{\tau}$) y fuerza de gravedad. Con esto, se generó la función de transferencia $G(s)$ linealizada alrededor del punto de operación (70°).

Para el control, se utilizó la metodología de **Sintonización Progresiva mediante Lugar de las Raíces (Root Locus Equivalent)**. Esta elegante técnica permitió reescribir la ecuación característica en cada iteración para aislar y sintonizar gráficamente el $K_p$, luego el $K_i$, y finalmente el $K_d$ con un filtro pasa-bajos.

### 2. Simulación No Lineal en Simulink (`lazo_cerrado.slx`)
El diseño continuo $C(s)$ se validó en un modelo no lineal que incluye:
*   Saturación física del PWM (0 a 100%).
*   Bloques de inyección de perturbaciones (para evaluar *Disturbance Rejection*).
*   Inyección de Ruido Blanco limitado en banda, emulando la resolución del ADC y el potenciómetro.

### 3. Firmware Discreto en Arduino (`Codigo_ESP.ino`)
El controlador continuo se discretizó e implementó en un microcontrolador usando temporizadores de hardware:
*   **Muestreo Estricto:** Se usa la función `millis()` para asegurar un $T_s = 10ms$.
*   **PWM de Alta Resolución:** Se configuró el Timer1 en modo Fast PWM (Modo 14) para tener 12-bits de resolución de control.
*   **Lazo de Control Discreto:** Se utilizó aproximación de Euler para calcular la acción Integral y Derivativa en diferencias finitas.
*   **Clamping (Anti-Windup):** El código contiene bloqueos condicionales para el integrador en caso de saturación del motor, evitando así picos peligrosos (windup).

## 🚀 Cómo usar este repositorio

1.  Abre el archivo `proyecto_caracterizacion_PID_Simulink_corregido.mlx` en MATLAB.
2.  Corre las secciones de caracterización para cargar los parámetros en el Workspace.
3.  Corre la sección final (Sección 10) que interactuará automáticamente con `lazo_cerrado.slx` para generar las 6 gráficas de evaluación del informe.
4.  Para la implementación física, carga `Codigo_ESP.ino` en el Arduino y envía referencias desde el Serial Monitor usando el formato `<70>`.
