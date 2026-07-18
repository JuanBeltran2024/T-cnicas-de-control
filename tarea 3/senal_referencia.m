function r = senal_referencia(t,p,tipo)
%SENAL_REFERENCIA Genera la referencia de pitch.
%
% tipo = 'step'  : paso de amplitud p.r_amp
% tipo = 'pulse' : tren de pulsos 0/p.r_amp

switch lower(tipo)
    case 'step'
        r = p.r_amp;

    case 'pulse'
        periodo = p.pulse_on + p.pulse_off;
        fase = mod(t,periodo);
        if fase < p.pulse_on
            r = p.r_amp;
        else
            r = 0;
        end

    otherwise
        error('Tipo de referencia no reconocido: %s',tipo);
end
end
