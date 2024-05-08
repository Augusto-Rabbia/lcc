/* Para aplicar punto fijo en f(x), hay que primero obtener la función de la forma x = g(x)
Luego, hay que verificar que el valor absoluto la derivada de g(x) sea menor que 1 en 
el intervalo donde se desea encontrar el punto fijo. En caso contrario no convergerá.
*/
function pfijo = metPuntoFijo(fung, x0, tol, maxIter)
    deff("y=f(x)","y="+fung);
    x = x0;
    x = f(x);
    iter = 1;
    while (abs(x-x0) > tol && iter < maxIter) do
        x0 = x;
        x = f(x);
        iter = iter + 1;
    end
    disp("Cantidad de iteraciones realizadas: ", iter);
    pfijo = x;
endfunction

/*
metPuntoFijo("2**(x-1)", 0.5, 0.001, 500)

Cantidad de iteraciones realizadas: 14.
ans  = 0.9981283
*/


function r = cotaErrPuntoFijo(fung, x0, cotaSupremo, cantIter)
    deff("y=f(x)","y="+fung);
    x1 = f(x0);
    r = cotaSupremo**cantIter / (1- cotaSupremo) * abs(x1 - x0);
endfunction

