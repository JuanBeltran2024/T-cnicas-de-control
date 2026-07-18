function ac = analizar_acoplamiento(p)
%ANALIZAR_ACOPLAMIENTO Comprueba las condiciones Am=A+B*Kx y Bm=B*kr.
%
% Salida:
%   ac.Kx_ideal_ls : solución de mínimos cuadrados para Kx
%   ac.kr_ideal_ls : solución de mínimos cuadrados para kr
%   ac.RA, ac.RB   : residuos de acoplamiento

A = p.A; B = p.B; Am = p.Am; Bm = p.Bm;

% Para B de columna completa, pinv(B) = (B'*B)^(-1)B'.
ac.Kx_ideal_ls = pinv(B)*(Am - A);      % 1x4
ac.kr_ideal_ls = pinv(B)*Bm;            % escalar

ac.Am_proyectada = A + B*ac.Kx_ideal_ls;
ac.Bm_proyectada = B*ac.kr_ideal_ls;
ac.RA = Am - ac.Am_proyectada;
ac.RB = Bm - ac.Bm_proyectada;

ac.norma_RA = norm(ac.RA,'fro');
ac.norma_RB = norm(ac.RB,2);
ac.error_rel_A = ac.norma_RA/norm(Am,'fro');
ac.error_rel_B = ac.norma_RB/norm(Bm,2);

% Matriz de controlabilidad sin requerir ctrb().
n = size(A,1);
Co = B;
for k = 1:n-1
    Co = [Co, A^k*B]; %#ok<AGROW>
end
ac.rango_controlabilidad = rank(Co);

ac.eig_A  = eig(A);
ac.eig_Am = eig(Am);
ac.eig_Am_proyectada = eig(ac.Am_proyectada);

fprintf('\n================ ANÁLISIS DE ACOPLAMIENTO ================\n');
fprintf('Rango de controlabilidad de (A,B): %d de %d\n',ac.rango_controlabilidad,n);
fprintf('Kx ideal aproximada (mínimos cuadrados):\n');
disp(ac.Kx_ideal_ls);
fprintf('kr ideal aproximada: %.10f\n',ac.kr_ideal_ls);
fprintf('||Am-(A+B*Kx)||_F = %.6e\n',ac.norma_RA);
fprintf('Error relativo de Am = %.6e\n',ac.error_rel_A);
fprintf('||Bm-B*kr||_2 = %.6e\n',ac.norma_RB);
fprintf('Error relativo de Bm = %.6e\n',ac.error_rel_B);
fprintf('Autovalores de A:\n'); disp(ac.eig_A.');
fprintf('Autovalores de Am:\n'); disp(ac.eig_Am.');
fprintf('==========================================================\n\n');

if ~exist(p.carpeta_resultados,'dir')
    mkdir(p.carpeta_resultados);
end
save(fullfile(p.carpeta_resultados,'acoplamiento.mat'),'ac');
writematrix(ac.Kx_ideal_ls,fullfile(p.carpeta_resultados,'Kx_ideal_aproximada.csv'));
writematrix(ac.kr_ideal_ls,fullfile(p.carpeta_resultados,'kr_ideal_aproximada.csv'));
writematrix(ac.RA,fullfile(p.carpeta_resultados,'residuo_A.csv'));
writematrix(ac.RB,fullfile(p.carpeta_resultados,'residuo_B.csv'));
end
