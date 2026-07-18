# Guía paso a paso: controlador MRAC para la dinámica longitudinal JAXA MuPAL

## 1. Qué pide el taller

El problema entrega una planta lineal de cuatro estados y una entrada:

\[
\dot x = Ax+Bu,
\qquad
x=[u_x\;u_z\;\theta\;q]^T.
\]

Se pide:

1. Simular el modelo de referencia \(\dot x_m=A_mx_m+B_mr\) ante un paso.
2. Comprobar si existen ganancias ideales \(K_x^*\) y \(k_r^*\) que cumplan
   \(A_m=A+BK_x^*\) y \(B_m=Bk_r^*\).
3. Diseñar un MRAC directo que ajuste en línea \(K_x(t)\) y \(k_r(t)\).
4. Demostrar estabilidad y convergencia del error por Lyapunov.
5. Simular con un paso y un tren de pulsos.
6. Comparar las ganancias finales y la matriz congelada \(A+BK_x(t_f)\) con \(A_m\).

## 2. Datos

\[
A=\begin{bmatrix}
-0.0175&0.173&-9.77&-5.63\\
-0.192&-1.09&-0.846&64.6\\
0&0&0&1\\
0.0081&-0.0738&0.0062&-1.9
\end{bmatrix},
\quad
B=\begin{bmatrix}-0.428\\4.91\\0\\4.22\end{bmatrix}.
\]

\[
A_m=\begin{bmatrix}
-0.03&0.166&125.6&37.29\\
-0.052&-1.02&-1554.7&-427.82\\
0&0&0&1\\
0.128&-0.0142&-1335.49&-425.12
\end{bmatrix},
\quad
B_m=\begin{bmatrix}-138.1\\1584.2\\0\\1361.6\end{bmatrix}.
\]

La lectura visual del PDF es importante: el elemento \((1,3)\) de \(A_m\) es `125.6`, no `12.56`, y el primer elemento de \(B_m\) es `-138.1`.

## 3. Conocimientos previos mínimos

### 3.1 Modelo de planta

La matriz \(A\) determina la evolución natural y el acoplamiento entre los cuatro estados. La columna \(B\) determina cómo la única entrada de control afecta cada estado.

### 3.2 Modelo de referencia

El modelo

\[
\dot x_m=A_mx_m+B_mr
\]

representa la dinámica deseada. Debe ser estable, por lo que \(A_m\) debe ser Hurwitz. Sus autovalores son aproximadamente

\[
-421.981,\;-3.170,\;-0.966,\;-0.0528.
\]

Todos tienen parte real negativa. El polo rápido alrededor de \(-422\) hace que el problema sea rígido numéricamente; por eso se recomienda `ode15s`.

### 3.3 Error de seguimiento

Se usa la definición del enunciado:

\[
e=x_m-x.
\]

El objetivo es \(e(t)\to0\).

### 3.4 Parámetros adaptativos

El control es

\[
u=K_x(t)x+k_r(t)r,
\]

con \(K_x(t)\in\mathbb R^{1\times4}\) y \(k_r(t)\in\mathbb R\).

### 3.5 Persistencia de excitación

La prueba de MRAC garantiza error de seguimiento asintótico y parámetros acotados. No garantiza que \(K_x(t)\to K_x^*\) o \(k_r(t)\to k_r^*\). Para identificar de manera única todos los parámetros se necesita una señal regresora suficientemente rica o persistentemente excitante. Un paso constante normalmente no cumple esta condición.

## 4. Paso 1: condiciones de acoplamiento

### 4.1 Ecuaciones

Para que la planta con ganancias ideales reproduzca el modelo:

\[
A+BK_x^*=A_m,
\qquad
Bk_r^*=B_m.
\]

Como \(B\) es \(4\times1\), cada columna de \(A_m-A\) debe ser un múltiplo de \(B\). No se puede imponer una matriz arbitraria de rango completo con una sola dirección de control.

### 4.2 Solución de mínimos cuadrados

Cuando los datos están redondeados, se calcula

\[
K_x^*=B^\dagger(A_m-A),
\qquad
k_r^*=B^\dagger B_m,
\]

con

\[
B^\dagger=(B^TB)^{-1}B^T.
\]

Resultado:

\[
K_x^*\approx
\begin{bmatrix}
0.02847356&0.01420932&-316.46687571&-100.28912236
\end{bmatrix},
\]

\[
k_r^*\approx322.65042179.
\]

Los residuos son

\[
\|A_m-(A+BK_x^*)\|_F\approx7.8169\times10^{-2},
\]

con error relativo \(3.65\times10^{-5}\), y

\[
\|B_m-Bk_r^*\|_2\approx2.1152\times10^{-2},
\]

con error relativo \(1.01\times10^{-5}\).

### 4.3 Interpretación

Las condiciones no se satisfacen exactamente con los números impresos, pero el residuo relativo es muy pequeño. La explicación más probable es el redondeo de las matrices del enunciado. Para la prueba teórica se supone acoplamiento exacto. Para reproducir el PDF se usan las matrices impresas y el residuo se interpreta como una perturbación no acoplada pequeña.

## 5. Paso 2: diseño del MRAC directo por Lyapunov

### 5.1 Control ideal

Si existieran ganancias constantes ideales:

\[
u^*=K_x^*x+k_r^*r,
\]

entonces

\[
\dot x=(A+BK_x^*)x+Bk_r^*r=A_mx+B_mr.
\]

### 5.2 Control adaptativo

Se reemplazan las ganancias desconocidas por estimaciones:

\[
u=K_x(t)x+k_r(t)r.
\]

Defina errores paramétricos:

\[
\widetilde K_x=K_x-K_x^*,
\qquad
\widetilde k_r=k_r-k_r^*.
\]

### 5.3 Dinámica del error

La planta queda

\[
\dot x=A_mx+B_mr+B\widetilde K_xx+B\widetilde k_rr.
\]

Como \(e=x_m-x\):

\[
\dot e=A_me-B\widetilde K_xx-B\widetilde k_rr.
\]

### 5.4 Ecuación de Lyapunov

Elija \(Q=Q^T>0\), en este trabajo \(Q=I_4\), y resuelva

\[
A_m^TP+PA_m=-Q.
\]

Se obtiene aproximadamente

\[
P=\begin{bmatrix}
10.525863&1.551175&-72.999631&-0.809086\\
1.551175&0.752531&-38.433859&-0.710290\\
-72.999631&-38.433859&2359.539588&37.877383\\
-0.809086&-0.710290&37.877383&0.734105
\end{bmatrix}.
\]

Sus autovalores son positivos, por lo que \(P>0\).

### 5.5 Función candidata

\[
V=e^TPe+\widetilde K_x\Gamma_x^{-1}\widetilde K_x^T+
\frac{\widetilde k_r^2}{\gamma_r},
\]

con \(\Gamma_x>0\) y \(\gamma_r>0\).

Derivando:

\[
\dot V=-e^TQe
-2e^TPB\widetilde K_xx
-2e^TPB\widetilde k_rr
+2\widetilde K_x\Gamma_x^{-1}\dot K_x^T
+2\frac{\widetilde k_r}{\gamma_r}\dot k_r.
\]

### 5.6 Leyes adaptativas

Se escogen para cancelar los términos cruzados:

\[
\boxed{\dot K_x^T=\Gamma_xx\,e^TPB}
\]

\[
\boxed{\dot k_r=\gamma_rr\,e^TPB}
\]

Entonces

\[
\boxed{\dot V=-e^TQe\le0}.
\]

### 5.7 Conclusión de estabilidad

De \(V\ge0\) y \(\dot V\le0\):

- \(e\), \(\widetilde K_x\) y \(\widetilde k_r\) permanecen acotados;
- \(e\in L_2\cap L_\infty\);
- con referencia acotada, los estados, el control y \(\dot e\) también son acotados;
- por el lema de Barbalat, \(e(t)\to0\).

La estrategia es **MRAC directo basado en Lyapunov**, no regla MIT. La regla MIT sigue un gradiente de una función de costo, pero no entrega por sí sola la demostración de estabilidad solicitada.

## 6. Algoritmo de simulación

1. Cargar \(A,B,A_m,B_m\).
2. Verificar controlabilidad y estabilidad de \(A_m\).
3. Elegir \(Q\), \(\Gamma_x\), \(\gamma_r\) y condiciones iniciales.
4. Resolver la ecuación de Lyapunov para \(P\).
5. En cada evaluación del integrador:
   - calcular \(r(t)\);
   - calcular \(e=x_m-x\);
   - calcular \(s=e^TPB\);
   - calcular \(u=K_xx+k_rr\);
   - calcular \(\dot x\), \(\dot x_m\), \(\dot K_x\) y \(\dot k_r\).
6. Integrar el estado aumentado con `ode15s`.
7. Graficar estados, errores, control y parámetros.
8. Calcular \(A+BK_x(t_f)\), sus autovalores y su diferencia con \(A_m\).

## 7. Parámetros de simulación usados

- Referencia: paso de pitch de 5 grados.
- Condiciones iniciales: todas cero.
- \(Q=I_4\).
- \(\Gamma_x=10I_4\).
- \(\gamma_r=10\).
- Tiempo: 60 s.
- Tren de pulsos: 5 s en 5 grados y 5 s en cero.
- Solver: `ode15s`, `RelTol=1e-7`, `AbsTol=1e-9`, `MaxStep=0.02 s`.

La amplitud de 5 grados es un supuesto explícito porque el enunciado no fija la amplitud. El código permite cambiarla a un paso unitario.

## 8. Resultados de comprobación numérica

### 8.1 Paso, usando las matrices impresas

A 60 s se obtuvo aproximadamente

\[
K_x(t_f)=
[-1.69935,\;-8.70093,\;-0.10252,\;-0.15603],
\]

\[
k_r(t_f)=1.58255.
\]

El error final fue

\[
\|e(t_f)\|_2\approx8.63\times10^{-3}.
\]

El error pequeño no se hace exactamente cero por el residuo de acoplamiento de las matrices redondeadas.

### 8.2 Paso, modelo proyectado exactamente acoplable

Al usar

\[
\bar A_m=A+BK_x^*,\qquad \bar B_m=Bk_r^*,
\]

la misma simulación produjo

\[
\|e(t_f)\|_2\approx7.1\times10^{-6},
\]

confirmando la predicción teórica.

### 8.3 Por qué las ganancias finales no son las ideales

Aunque el seguimiento es bueno, las ganancias finales están lejos de \(K_x^*\) y \(k_r^*\). Esto no contradice Lyapunov. La prueba garantiza acotamiento de parámetros, no identificación. Un paso deja de aportar información nueva cuando el sistema llega al régimen estacionario.

### 8.4 Matriz congelada de lazo cerrado

Para el paso:

\[
A_{cl}(t_f)=A+BK_x(t_f).
\]

Sus autovalores aproximados son

\[
-22.826\pm43.591j,\qquad -0.00427\pm0.01537j.
\]

Es estable, pero no es parecida a \(A_m\). Esto demuestra que seguimiento de una trayectoria particular no implica convergencia de la matriz congelada al modelo de referencia.

### 8.5 Tren de pulsos

Con el tren 0/5 grados, los parámetros siguen moviéndose cada vez que cambia la referencia. A 60 s se obtuvo aproximadamente

\[
K_x(t_f)=[-1.90546,\;-8.59559,\;-0.92408,\;0.84051],
\]

\[
k_r(t_f)=1.81303.
\]

El pulso aporta más excitación que un paso, pero no garantiza persistencia de excitación suficiente para identificar cinco parámetros independientes. Por eso las ganancias no se estabilizan necesariamente en sus valores ideales. Además, el concepto de “ganancia final” depende de la fase exacta del tren de pulsos.

## 9. Efecto de la ganancia de adaptación

Comparando escalas \(\gamma=1,10,100\):

| Escala | RMS de error de pitch | Máximo \(|u|\) |
|---:|---:|---:|
| 1 | 2.008 deg | 17.43 |
| 10 | 1.653 deg | 38.95 |
| 100 | 1.413 deg | 186.74 |

Una ganancia mayor reduce el error transitorio, pero aumenta mucho el esfuerzo de control y la sensibilidad a ruido, retardos, saturación y dinámicas no modeladas. No debe aumentarse sin revisar límites del actuador.

## 10. Simulink

El PDF del taller no exige Simulink. El archivo `Simulink_guia.md` contiene una implementación opcional bloque por bloque y una tabla ecuación-bloque.

## 11. Errores comunes

1. Leer `125.6` como `12.56` o `-138.1` como `-0138.1` sin verificar la página.
2. Definir el error como \(x-x_m\) y conservar las leyes con el signo de \(x_m-x\).
3. Usar una \(Gamma_x\) no positiva definida.
4. Suponer que \(e\to0\) implica \(K_x\to K_x^*\).
5. Ignorar que una única entrada limita las matrices que pueden acoplarse exactamente.
6. Usar `ode45` con pasos grandes pese al polo rápido del modelo.
7. Añadir saturación al control y seguir usando la prueba ideal sin modificación anti-windup.
8. Juzgar un tren de pulsos solamente por el error en el instante de conmutación.

## 12. Conclusión para el informe

Se diseñó un MRAC directo de realimentación completa de estados para la dinámica longitudinal del JAXA MuPAL. Las condiciones de acoplamiento se cumplen con gran precisión, aunque no exactamente, debido al redondeo de las matrices del enunciado. Las leyes adaptativas obtenidas por Lyapunov garantizan acotamiento de todas las señales y seguimiento asintótico bajo acoplamiento exacto. Las simulaciones muestran que la planta sigue el modelo, pero las ganancias adaptativas no convergen necesariamente a las ganancias ideales, especialmente con una entrada paso, porque no existe suficiente persistencia de excitación. Aumentar las ganancias de adaptación mejora el seguimiento transitorio a costa de mayores picos de control y menor robustez práctica.
