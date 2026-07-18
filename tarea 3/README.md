# Taller MRAC - JAXA MuPAL

Este paquete implementa un **MRAC directo de realimentación completa de estados** para la planta longitudinal del enunciado.

## Archivos principales

- `main_mrac_jaxa.m`: ejecuta todo el taller.
- `datos_jaxa.m`: contiene matrices, condiciones iniciales y parámetros de diseño.
- `analizar_acoplamiento.m`: calcula las ganancias ideales aproximadas y los residuos de acoplamiento.
- `simular_modelo_referencia.m`: simula el modelo de referencia sin la planta.
- `simular_mrac.m` y `dinamica_mrac.m`: integran el sistema adaptativo.
- `graficar_modelo_referencia.m` y `graficar_mrac.m`: generan figuras independientes.
- `comparar_ganancias_adaptacion.m`: compara `gamma = 1, 10, 100`.
- `exportar_figura_vectorial.m`: guarda cada figura en SVG y PDF.

## Ejecución

1. Abra MATLAB.
2. Cambie la carpeta actual a `MRAC_JAXA_Taller`.
3. Ejecute:

```matlab
main_mrac_jaxa
```

Los resultados se guardan en:

- `figuras/`: archivos SVG y PDF.
- `resultados/`: MAT y CSV.

## Supuestos configurables

El enunciado no especifica la amplitud del paso. El archivo `datos_jaxa.m` usa un comando de pitch de `5 deg`:

```matlab
p.r_amp = deg2rad(5);
```

Para usar un paso unitario, cambie a:

```matlab
p.r_amp = 1;
```

El tren de pulsos usa 5 s en nivel alto y 5 s en cero. Estos tiempos también se modifican en `datos_jaxa.m`.

## Verificación del redondeo

Las matrices impresas satisfacen las condiciones de acoplamiento con un residuo pequeño, pero no exactamente. El programa simula:

- el modelo impreso, que reproduce el taller;
- un modelo proyectado exactamente acoplable, que permite verificar la conclusión teórica `e(t) -> 0`.

## Valores de comprobación

Con 5 deg, condiciones iniciales nulas, `Q=I`, `Gamma_x=10I` y `gamma_r=10`, una integración independiente de las mismas ecuaciones produjo aproximadamente:

- Ganancia ideal por mínimos cuadrados:
  `Kx* = [0.02847356, 0.01420932, -316.46687571, -100.28912236]`
- `kr* = 322.65042179`
- Paso a 60 s:
  `Kx(tf) ≈ [-1.69935, -8.70093, -0.10252, -0.15603]`, `kr(tf) ≈ 1.58255`
- El modelo exactamente proyectado produjo `||e(tf)||_2` del orden de `1e-5`.

Pequeñas diferencias son normales por el solver y por los instantes exactos de discontinuidad del tren de pulsos.
