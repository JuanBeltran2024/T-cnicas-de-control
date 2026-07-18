function graficar_mrac(sim,p,prefijo)
%GRAFICAR_MRAC Genera figuras independientes de estados, errores, control y ganancias.

nombres = {'u_x','u_z','theta','q'};
ylabels = {'u_x [m/s]','u_z [m/s]','\theta [deg]','q [deg/s]'};

% Planta frente al modelo de referencia.
for i = 1:4
    fig = figure('Name',['Seguimiento ',nombres{i}]);
    yp = sim.x(:,i);
    ym = sim.xm(:,i);
    if i >= 3
        yp = rad2deg(yp);
        ym = rad2deg(ym);
    end
    plot(sim.t,ym,'--','LineWidth',1.5); hold on;
    plot(sim.t,yp,'LineWidth',1.2);
    grid on;
    xlabel('Tiempo [s]'); ylabel(ylabels{i});
    legend('Modelo de referencia','Planta','Location','best');
    title(['Seguimiento MRAC: ',nombres{i}]);
    exportar_figura_vectorial(fig,[prefijo,'_seguimiento_',nombres{i}],p.carpeta_figuras);
end

% Error de cada estado.
for i = 1:4
    fig = figure('Name',['Error ',nombres{i}]);
    ye = sim.e(:,i);
    if i >= 3
        ye = rad2deg(ye);
    end
    plot(sim.t,ye,'LineWidth',1.2);
    grid on;
    xlabel('Tiempo [s]'); ylabel(['e_',nombres{i}]);
    title(['Error de seguimiento: ',nombres{i}]);
    exportar_figura_vectorial(fig,[prefijo,'_error_',nombres{i}],p.carpeta_figuras);
end

% Señal de control.
fig = figure('Name','Señal de control');
plot(sim.t,sim.u,'LineWidth',1.2);
grid on;
xlabel('Tiempo [s]'); ylabel('u [unidad de deflexión del elevador]');
title('Señal de control MRAC');
exportar_figura_vectorial(fig,[prefijo,'_control'],p.carpeta_figuras);

% Ganancias Kx.
for i = 1:4
    fig = figure('Name',['Ganancia Kx',num2str(i)]);
    plot(sim.t,sim.Kx(:,i),'LineWidth',1.2);
    grid on;
    xlabel('Tiempo [s]'); ylabel(['K_{x',num2str(i),'}']);
    title(['Evolución de K_{x',num2str(i),'}']);
    exportar_figura_vectorial(fig,[prefijo,'_Kx',num2str(i)],p.carpeta_figuras);
end

% Ganancia kr.
fig = figure('Name','Ganancia kr');
plot(sim.t,sim.kr,'LineWidth',1.2);
grid on;
xlabel('Tiempo [s]'); ylabel('k_r');
title('Evolución de k_r');
exportar_figura_vectorial(fig,[prefijo,'_kr'],p.carpeta_figuras);
end
