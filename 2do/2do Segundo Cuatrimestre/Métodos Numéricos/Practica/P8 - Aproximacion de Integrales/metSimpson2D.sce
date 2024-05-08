// Metodo de Simpson, integracion multiple
function w = simpson_volumen(f, a, b, c, d, n)
    deff("z = F(x, y)", "z = " + f);
    deff("y = C(x)", "y = " + c);
    deff("y = D(x)", "y = " + d);
    h = (b - a) / n;
    
    w = simpson_fixed(F, a, C(a), D(a), n);
    for i = 1:n-1 do
        x_i = a + i * h;
        if modulo(i, 2) == 1 then
            w = w + 4 * simpson_fixed(F, x_i, C(x_i), D(x_i), n);
        else
            w = w + 2 * simpson_fixed(F, x_i, C(x_i), D(x_i), n);
        end
    end
    w = (h / 3) * (w + simpson_fixed(F, b, C(b), D(b), n));
endfunction

// Metodo de Simpson con una variable fija. Recibe una funcion de dos
// variables y ejectuta el metodo del trapecio con la primer variable fija
function w = simpson_fixed(F, x, a, b, n)
    h = (b - a) / n;
    w = F(x, a) + F(x, b);
    for i = 1:n-1
        y_i = a + i * h;
        if modulo(i, 2) == 1 then
            w = w + 4 * F(x, y_i);
        else
            w = w + 2 * F(x, y_i);
        end
    end
    w = w * (h / 3);
endfunction
