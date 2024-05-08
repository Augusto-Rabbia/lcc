function cheb = Poly_Cheby(n)
    pol1 = poly([1], 'x', 'c');
    pol2 = poly([0 1], 'x', 'c');
    if (n == 0) then
        cheb = pol1;
        return;
    elseif (n == 1) then
        cheb = pol2;
        return;
    else
        polx = poly([0 2], 'x', 'c');
        cheb = pol2;
        for i=2:n
            cheb = polx*pol2-pol1;
            pol1 = pol2;
            pol2 = cheb;
        end
    end
endfunction


function nodos = chebyshev_general(a, b, n);
    cheby = Pol_Cheby(n);
    raices = roots(cheby);
    for i=1:n
        nodos(i) = ((b+a)+raices(i)*(b-a))/2
    end
endfunction
