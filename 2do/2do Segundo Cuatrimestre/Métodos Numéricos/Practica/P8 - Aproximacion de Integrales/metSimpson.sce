// Regla de Simpson: f e C4 en [a b] 
// int(a-b)f(x)dx = h/3(f(a) + 4f(x1) + f(b)) - (err)h^5/90 * f''''(c9)
// abs(err) <= h^5/90 * max (x e [a,b]) abs(f''''(x))

// Método de Simpson para aproximar una integral definida
// Entrada: f: función
//          a,b: extremos del intervalo de integración
function A = metSimpson(f,a,b)
    h = (b-a)/2;
    x1 = a+h;
    A = (f(a) + 4*f(x1) + f(b))*h/3;
endfunction

// Encontrar una cota para el error del método de Simpson
// Entrada: a,b: extremos del intervalo de integración
//          n cantidad de subintervalos
//          cota: cota superior de f''''(x)
function x = cotaErrorSimpson(a,b,n,cota)
    h = (b-a)/n;
    x = (h**4)*(b-a)/180 * cota;
endfunction

// Método de Simpson compuesto para aproximar una integral definida
// Entrada: f: función
//          a,b: extremos del intervalo de integración
//          n cantidad de subintervalos. Entero par.
function A = metSimpsonComp(f,a,b,n)
    if modulo(n,2)<>0 then
        disp("n debe que ser un entero par.")
        abort();
    end
    h = (b-a)/n;
    for j=1:n-1
        x(j) = a + j*h;
    end
    
    A = f(a)+f(b);
    for j=1:2:n-1
        A = A + 4*f(x(j));
    end
    for j=2:2:n-1
        A = A + 2*f(x(j));
    end
    A = A*h/3;
endfunction
