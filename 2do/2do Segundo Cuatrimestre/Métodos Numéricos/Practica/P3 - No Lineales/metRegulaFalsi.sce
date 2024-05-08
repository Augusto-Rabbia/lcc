function r = regulaFalsi(fun,a,b,tol)
    deff("y=f(x)","y="+fun);
    iter = 0;
    c = b - f(b)*(b-a)/(f(b)-f(a));
    while (abs(b-c) > tol && f(c) <> 0) do
        if (f(a)*f(c) < 0) then
            b = c;
            c = b - f(b)*(b-a)/(f(b)-f(a));
        elseif (f(b)*f(c) < 0) then
            a = c;
            c = b - f(b)*(b-a)/(f(b)-f(a));
        end
        iter = iter + 1;
    end
    disp("Cantidad de iteraciones realizadas: ", iter);
    r = c;
endfunction
/*
regulaFalsi("sin(x)-x**2 / 2", -%pi, %pi/10, 0.01)
Cantidad de iteraciones realizadas: 4.
ans  = 0.0028750
*/
