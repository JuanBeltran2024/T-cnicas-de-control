function P = resolver_lyapunov(Am,Q)
%RESOLVER_LYAPUNOV Resuelve Am'*P + P*Am = -Q.
%
% Usa lyap si está disponible. Si no, utiliza vectorización y producto de
% Kronecker, evitando depender de Control System Toolbox.

n = size(Am,1);

if exist('lyap','file') == 2
    P = lyap(Am',Q);
else
    L = kron(eye(n),Am') + kron(Am',eye(n));
    vecP = L \ (-Q(:));
    P = reshape(vecP,n,n);
end

% Corrige pequeños errores de redondeo numérico.
P = 0.5*(P + P');
end
