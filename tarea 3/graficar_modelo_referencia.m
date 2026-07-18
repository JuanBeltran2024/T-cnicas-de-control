function graficar_modelo_referencia(sim,p,prefijo)
%GRAFICAR_MODELO_REFERENCIA Crea una figura independiente por estado.

nombres = {'u_x','u_z','theta','q'};
ylabels = {'u_x [m/s]','u_z [m/s]','\theta [deg]','q [deg/s]'};

for i = 1:4
    fig = figure('Name',['Modelo de referencia - ',nombres{i}]);
    y = sim.xm(:,i);
    if i >= 3
        y = rad2deg(y);
    end
    plot(sim.t,y,'LineWidth',1.4);
    grid on;
    xlabel('Tiempo [s]');
    ylabel(ylabels{i});
    title(['Modelo de referencia: ',nombres{i}]);
    exportar_figura_vectorial(fig,[prefijo,'_modelo_',nombres{i}],p.carpeta_figuras);
end
end
