function r = metBiseccion(fun, a, b, err)
    deff("y=f(x)","y="+fun);
    iter = 1;
    c = (b+a)/2;
    while (abs(c-b) > err && f(c) <> 0) do
        if (f(a)*f(c) < 0) then
            b = c;
            c = (b+a)/2;
        elseif (f(b)*f(c) < 0) then
            a = c;
            c = (b+a)/2;
        end
        iter=iter+1;
    end
    printf("%d iteraciones realizadas", iter);
    r = c;
endfunction
/*
metBiseccion("sin(x)-x**2 / 2", -%pi/2, %pi, 0.01)
9 iteraciones realizadas 

ans = 0.0030680
*/


function err = errorBiseccion(a, b, iteraciones)
    err = 1/(2**iteraciones * (b-a));
endfunction
