function r = derivada(f, x, n, h)
    if (n == 0) then
        r = f(x);
    else
        r = (derivada(f, x+h, n - 1, h) - derivada(f, x, n - 1, h))/h;
    end
endfunction

function r = derivadasci(f, x, n)
    if (n == 0) then
        r = f(x);
    else
        r = numderivative(f, derivadasci(f, x, n - 1));
    end
endfunction
