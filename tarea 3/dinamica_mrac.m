function dz = dinamica_mrac(t,z,p,tipo,Am_usada,Bm_usada,P,Gamma_x,gamma_r)
%DINAMICA_MRAC Ecuaciones diferenciales del controlador MRAC directo.
%
% Estado aumentado:
%   z = [x(4); xm(4); Kx^T(4); kr]

x    = z(1:4);
xm   = z(5:8);
KxT  = z(9:12);
kr   = z(13);

r = senal_referencia(t,p,tipo);
e = xm - x;

% Señal escalar que aparece en la ley de adaptación.
s = e'*P*p.B;

% Ley de control u = Kx*x + kr*r.
u = KxT'*x + kr*r;

% Planta y modelo de referencia.
xdot  = p.A*x + p.B*u;
xmdot = Am_usada*xm + Bm_usada*r;

% Leyes adaptativas obtenidas por Lyapunov.
KxT_dot = Gamma_x*x*s;
kr_dot  = gamma_r*r*s;

dz = [xdot; xmdot; KxT_dot; kr_dot];
end
