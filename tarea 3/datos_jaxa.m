function p = datos_jaxa()
%DATOS_JAXA Define la planta, el modelo de referencia y los parámetros de diseño.
%
% Salida:
%   p : estructura con matrices, condiciones iniciales y configuración.

% -------------------- Planta longitudinal --------------------
p.A = [ -0.0175,  0.1730,   -9.7700,   -5.6300;
        -0.1920, -1.0900,   -0.8460,   64.6000;
         0,       0,         0,         1.0000;
         0.0081, -0.0738,    0.0062,   -1.9000 ];

p.B = [ -0.4280;
         4.9100;
         0;
         4.2200 ];

% -------------------- Modelo de referencia --------------------
% Valores verificados directamente en la página 1 del enunciado.
p.Am = [ -0.0300,  0.1660,    125.6000,    37.2900;
         -0.0520, -1.0200,  -1554.7000,  -427.8200;
          0,       0,          0,           1.0000;
          0.1280, -0.0142,  -1335.4900,  -425.1200 ];

p.Bm = [ -138.1000;
         1584.2000;
            0;
         1361.6000 ];

% -------------------- Condiciones iniciales --------------------
p.x0  = zeros(4,1);
p.xm0 = zeros(4,1);
p.Kx0 = zeros(4,1);   % Se almacena Kx^T como vector columna.
p.kr0 = 0;

% -------------------- Señales de referencia --------------------
% El enunciado no fija amplitud. Se usa 5 grados como supuesto de simulación
% físicamente interpretable. Para una entrada paso unitaria, usar p.r_amp = 1.
p.r_amp = deg2rad(5);
p.pulse_on  = 5;      % segundos en nivel alto
p.pulse_off = 5;      % segundos en nivel cero

% -------------------- Parámetros de Lyapunov y adaptación --------------------
p.Q = eye(4);
p.Gamma_x = 10*eye(4);
p.gamma_r = 10;
p.gamma_cases = [1, 10, 100];

% -------------------- Configuración de simulación --------------------
p.t_step  = [0, 60];
p.t_pulse = [0, 60];
p.ode_options = odeset('RelTol',1e-7,'AbsTol',1e-9,'MaxStep',0.02);

% false: usa exactamente Am y Bm impresas en el taller.
% true : proyecta el modelo sobre las condiciones exactas de acoplamiento.
p.usar_modelo_proyectado = false;

p.carpeta_figuras = fullfile(pwd,'figuras');
p.carpeta_resultados = fullfile(pwd,'resultados');
end
