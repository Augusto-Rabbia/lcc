// Método de Diferencias Divididas de Newton
function p = DD_Newton(x,y)
    // Entrada: x,y = vectores puntos de interpolación (x,y)
    // Salida: w = polinomio de diferencias divididas de Newton
    p = 0
    s = poly(0,"x");
    n = length(x);
    for j=n:-1:2
        p = p*(s-x(j-1)) + DD(x(1:j),y(1:j))*(s-x(j-1));
    end
    p = p + y(1)
endfunction

// Diferencias divididas
function w = DD(x,y)
    // Entrada: 
    // Salida: 
    n = length(x)
    if n==1 then
        w = y(1)
    else
        w = (DD(x(2:n),y(2:n))-DD(x(1:n-1),y(1:n-1)))/(x(n)-x(1));
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
