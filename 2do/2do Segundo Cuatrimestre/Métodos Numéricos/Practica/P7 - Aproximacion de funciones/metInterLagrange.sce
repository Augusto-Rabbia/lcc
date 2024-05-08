function pol = interpolacionLagrange(X,Y)
    n = length(X);
    if n<>length(Y) then
        disp('Input vectors must have the same size');
    end
    
    x = poly([0,1],"x","coeff");
    
    for k=1:n
        L(k) = 1;
        for i=1:k-1
            L(k) = L(k) * (x-X(i))/(X(k)-X(i));
        end
        for i=k+1:n
            L(k) = L(k) * (x-X(i))/(X(k)-X(i));
        end
    end
    
    pol = L(1)*Y(1);
    for k=2:n
        pol = pol + L(k)*Y(k);
    end
endfunction

// Error de interpolación
function w = errInterpolacion(puntoEvaluacion,x,cot)
    // Entrada: puntoEvaluacion = valor real, x = nodos de interpolación, cot = cota de |f^(n))|
    // Salida: w = error de interpolación en x = puntoEvaluacion
    n = length(x)
    w = cot/(factorial(n))
    for i=1:n do
        w = w*abs(puntoEvaluacion - x(i))
    end
endfunction
