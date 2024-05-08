/* 
*/
function r = metSecante(fun, x0, x1, tol, maxIter)
    deff("y=f(x)","y="+fun);
    iter = 0;
    while (abs(x1-x0) > tol && iter < maxIter) do
        xtemp = x1;
        x1 = x1 - f(x1)*(x1-x0)/(f(x1)-f(x0));
        x0 = xtemp;
        iter = iter + 1;
    end
    disp("Cantidad de iteraciones realizadas: ", iter);
    r = x1;
endfunction

/*
metSecante("sin(x)-x**2 / 2", -%pi, %pi/10, 0.01, 1000)
Cantidad de iteraciones realizadas: 4.

ans  = 0.0000445
*/
