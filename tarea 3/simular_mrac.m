function sim = simular_mrac(p,ac,tspan,tipo,Gamma_x,gamma_r,usar_proyectado)
%SIMULAR_MRAC Integra planta, modelo y parámetros adaptativos.

if usar_proyectado
    Am_usada = ac.Am_proyectada;
    Bm_usada = ac.Bm_proyectada;
else
    Am_usada = p.Am;
    Bm_usada = p.Bm;
end

P = resolver_lyapunov(Am_usada,p.Q);

% Verificaciones básicas.
if any(real(eig(Am_usada)) >= 0)
    error('El modelo de referencia no es Hurwitz.');
end
if min(eig(P)) <= 0
    error('La matriz P calculada no es positiva definida.');
end
if min(eig(Gamma_x)) <= 0 || gamma_r <= 0
    error('Las ganancias de adaptación deben ser positivas.');
end

z0 = [p.x0; p.xm0; p.Kx0; p.kr0];

f = @(t,z) dinamica_mrac(t,z,p,tipo,Am_usada,Bm_usada,P,Gamma_x,gamma_r);
[t,z] = ode15s(f,tspan,z0,p.ode_options);

sim.t  = t;
sim.x  = z(:,1:4);
sim.xm = z(:,5:8);
sim.e  = sim.xm - sim.x;
sim.Kx = z(:,9:12);
sim.kr = z(:,13);
sim.P  = P;
sim.Am_usada = Am_usada;
sim.Bm_usada = Bm_usada;
sim.usar_proyectado = usar_proyectado;

N = length(t);
sim.r = zeros(N,1);
sim.u = zeros(N,1);
for k = 1:N
    sim.r(k) = senal_referencia(t(k),p,tipo);
    sim.u(k) = sim.Kx(k,:)*sim.x(k,:)' + sim.kr(k)*sim.r(k);
end

sim.Kx_final = sim.Kx(end,:);
sim.kr_final = sim.kr(end);
sim.Acl_final = p.A + p.B*sim.Kx_final;
sim.eig_Acl_final = eig(sim.Acl_final);
sim.norma_error_final = norm(sim.e(end,:),2);
sim.error_theta_rms = sqrt(mean(sim.e(:,3).^2));
sim.u_max = max(abs(sim.u));
end
