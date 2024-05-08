function aprox = taylor(f, x, n)
    i = 0;
    while i <= n
        r(i+1) = derivada(f, 0, i, 0.001)/factorial(i);
        i = i+1;
    end
    p = poly(r, 'x', 'c');
    aprox = mihorner(p, x, n, 0);
endfunction
