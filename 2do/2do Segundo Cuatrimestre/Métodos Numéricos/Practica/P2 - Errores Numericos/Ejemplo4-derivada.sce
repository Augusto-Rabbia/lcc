function r = derivadaenpunto(fun, x, h)
    deff("y=f(x)", "y="+fun);
    r = (f(x+h) - f(x))/h;
endfunction

function r = derivadanesima(fun, x, n, h)
    deff("y=f(x)", "y="+fun);
    if (n == 0)then
        r = x;
    else
        derant = derivadanesima(fun, x, n-1, h);
        r = (f(derant+h) - f(derant))/h;
    end
endfunction
