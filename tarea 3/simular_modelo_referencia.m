function sim = simular_modelo_referencia(p,tspan,tipo)
%SIMULAR_MODELO_REFERENCIA Simula xdot_m = Am*xm + Bm*r.

f = @(t,xm) p.Am*xm + p.Bm*senal_referencia(t,p,tipo);
[t,xm] = ode15s(f,tspan,p.xm0,p.ode_options);

r = zeros(length(t),1);
for k = 1:length(t)
    r(k) = senal_referencia(t(k),p,tipo);
end

sim.t = t;
sim.xm = xm;
sim.r = r;
end
