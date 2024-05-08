
// Metodos de integracion numerica
// Metodo del trapecio simple
function w = trapecio(f, a, b)
    deff("y = F(x)", "y = " + f);
    w = (F(a) + F(b)) * (b - a) / 2;
endfunction

// Metodo del trapecio compuesto
function w = trapecio_compuesto(f, a, b, n)
    deff("y = F(x)", "y = " + f);
    h = (b - a) / n;
    w = 0;
    for j = 1:n-1 do
        xj = a + j * h;
        w = w + F(xj);
    end
    w = h * (0.5 * (F(a) + F(b)) + w);
endfunction

// Metodo de Simpson
function w = simpson(f, a, b)
    deff("y = F(x)", "y = " + f);
    h = (b - a) / 2;
    c = a + h; // c = (a + b) / 2
    w = (h / 3) * (F(a) + 4 * F(c) + F(b));
endfunction

// Metodo de Simpson compuesto
function w = simpson_compuesto(f, a, b, n)
    deff("y = F(x)", "y = " + f);
    h = (b - a) / n;
    w = 0;
    for j = 1:n-1 do
        xj = a + j * h;
        if modulo(j, 2) == 1 then
            w = w + 4 * F(xj);
        else
            w = w + 2 * F(xj);
        end
    end
    w = (h / 3) * (F(a) + F(b) + w);
endfunction


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
