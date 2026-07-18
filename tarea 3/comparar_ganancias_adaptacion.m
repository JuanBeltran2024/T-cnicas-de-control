function tabla = comparar_ganancias_adaptacion(p,ac)
%COMPARAR_GANANCIAS_ADAPTACION Compara escalas gamma=1,10,100 para el paso.

casos = p.gamma_cases;
N = numel(casos);
sims = cell(N,1);

for i = 1:N
    g = casos(i);
    sims{i} = simular_mrac(p,ac,p.t_step,'step',g*eye(4),g,false);
end

% Comparación de error de pitch.
fig = figure('Name','Comparación de ganancias - error de pitch'); hold on;
for i = 1:N
    plot(sims{i}.t,rad2deg(sims{i}.e(:,3)),'LineWidth',1.2);
end
grid on;
xlabel('Tiempo [s]'); ylabel('e_\theta [deg]');
legend(compose('\gamma = %g',casos),'Location','best');
title('Efecto de la ganancia de adaptación sobre el error de pitch');
exportar_figura_vectorial(fig,'comparacion_gamma_error_theta',p.carpeta_figuras);

% Comparación de la señal de control.
fig = figure('Name','Comparación de ganancias - control'); hold on;
for i = 1:N
    plot(sims{i}.t,sims{i}.u,'LineWidth',1.2);
end
grid on;
xlabel('Tiempo [s]'); ylabel('u');
legend(compose('\gamma = %g',casos),'Location','best');
title('Efecto de la ganancia de adaptación sobre la señal de control');
exportar_figura_vectorial(fig,'comparacion_gamma_control',p.carpeta_figuras);

Gamma = casos(:);
ErrorFinal = zeros(N,1);
ErrorThetaRMS_deg = zeros(N,1);
ControlMax = zeros(N,1);
for i = 1:N
    ErrorFinal(i) = sims{i}.norma_error_final;
    ErrorThetaRMS_deg(i) = rad2deg(sims{i}.error_theta_rms);
    ControlMax(i) = sims{i}.u_max;
end

tabla = table(Gamma,ErrorFinal,ErrorThetaRMS_deg,ControlMax);
disp(tabla);
writetable(tabla,fullfile(p.carpeta_resultados,'comparacion_ganancias.csv'));
end
