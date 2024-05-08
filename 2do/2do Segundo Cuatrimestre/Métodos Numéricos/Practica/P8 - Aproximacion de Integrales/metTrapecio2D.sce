// Metodo del trapecio, integracion multiple
function w = trapecio_volumen(f, a, b, c, d, n)
    deff("z = F(x, y)", "z = " + f);
    deff("y = C(x)", "y = " + c);
    deff("y = D(x)", "y = " + d);
    h = (b - a) / n;
    
    w = 0.5 * trapecio_fixed(F, a, C(a), D(a), n);
    for i = 1:n-1 do
        x_i = a + i * h;
        w = w + trapecio_fixed(F, x_i, C(x_i), D(x_i), n);
    end
    w = h * (w + 0.5 * trapecio_fixed(F, b, C(b), D(b), n));
endfunction


// Metodo de trapecio con una variable fija. Recibe una funcion de dos
// variables y ejectuta el metodo del trapecio con la primer variable fija
function w = trapecio_fixed(F, x, a, b, n)
    h = (b - a) / n;
    w = (F(x, a) + F(x, b)) / 2;
    for i = 1:n-1
        y_i = a + i * h;
        w = w + F(x, y_i);
    end
    w = w * h;
endfunction
