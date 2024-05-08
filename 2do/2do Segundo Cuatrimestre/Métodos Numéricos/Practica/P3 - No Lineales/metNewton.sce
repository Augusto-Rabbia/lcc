function root = metNewton(fun,x0,tol,iter)
    deff("y=f(x)","y="+fun);
    i = 0;
    x1 = x0 - f(x0)/numderivative(f,x0)
    while abs(x1-x0)>tol && i < iter
        i = i+1;
        x0 = x1;
        x1 = x0 - f(x0)/numderivative(f,x0)
    end
    if (abs(x1-x0)> tol) then disp('Se alcanzo el máximo de iteraciones'); end
    disp("Cantidad de iteraciones realizadas: ", i);
    root = x1;
endfunction

/*
metNewtonNonLinear("sin(x)-x**2 / 2", -%pi, 0.01, 100)
Cantidad de iteraciones realizadas: 3.
ans = 0.0000135
*/
