# Guía opcional de implementación en Simulink

El enunciado no exige Simulink, pero el mismo controlador puede implementarse con los siguientes subsistemas.

| Ecuación | Bloque o subsistema de Simulink |
|---|---|
| `xdot = A x + B u` | `State-Space` con `A=A`, `B=B`, `C=eye(4)`, `D=zeros(4,1)` |
| `xmdot = Am xm + Bm r` | `State-Space` con `A=Am`, `B=Bm`, `C=eye(4)`, `D=zeros(4,1)` |
| `e = xm - x` | `Sum` de dos entradas con signos `+-` |
| `s = e' P B` | `Matrix Multiply` o `Gain` con ganancia fila `(P*B)'` aplicada a `e` |
| `u = Kx x + kr r` | dos `Product`, un `Dot Product` y un `Sum` con signos `++` |
| `dot(Kx^T) = Gamma_x x s` | `Product` para `x*s`, `Gain` con `Gamma_x`, cuatro `Integrator` |
| `dot(kr) = gamma_r r s` | dos `Product`, `Gain` con `gamma_r`, un `Integrator` |

## Conexiones

1. La salida `x` de la planta entra al cálculo de control y a la ley de adaptación.
2. La salida `xm` del modelo entra al sumador del error.
3. El error usa `e=xm-x`, no `x-xm`.
4. `e` pasa por la ganancia fila `(P*B)'` para producir el escalar `s`.
5. Los cuatro integradores de `Kx^T` tienen condición inicial cero.
6. El integrador de `kr` tiene condición inicial cero.
7. El control `u` vuelve a la entrada de la planta.

## Solver

- Tipo: variable-step.
- Solver recomendado: `ode15s` por el polo rápido del modelo de referencia.
- Relative tolerance: `1e-7`.
- Absolute tolerance: `1e-9`.
- Maximum step size: `0.02 s`.

## Comprobaciones

- Verifique que `P=P'` y `eig(P)>0`.
- Verifique que el bloque del error tenga signos `+-` en el orden `xm-x`.
- Verifique dimensiones: `x,e,xm` son 4x1; `Kx` es 1x4; `u,r,kr,s` son escalares.
- No añada saturación sin modificar la prueba de estabilidad; una saturación rompe la dinámica ideal usada en Lyapunov.
