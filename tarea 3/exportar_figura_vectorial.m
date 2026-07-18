function exportar_figura_vectorial(fig,nombre_base,carpeta)
%EXPORTAR_FIGURA_VECTORIAL Guarda una figura en SVG y PDF.

if ~exist(carpeta,'dir')
    mkdir(carpeta);
end

archivo_svg = fullfile(carpeta,[nombre_base,'.svg']);
archivo_pdf = fullfile(carpeta,[nombre_base,'.pdf']);

try
    exportgraphics(fig,archivo_svg,'ContentType','vector');
    exportgraphics(fig,archivo_pdf,'ContentType','vector');
catch
    % Compatibilidad con versiones antiguas de MATLAB.
    print(fig,archivo_svg,'-dsvg');
    print(fig,archivo_pdf,'-dpdf','-painters');
end
end
