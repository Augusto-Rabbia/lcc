// Regla trapecio: f e C2 en [a b] 
// Entonces int(a-b)f(x)dx = (f(a) + f(b))*h / 2 - (err) + h^3/12*f''(x))
// abs(err) <= h^3/12 * max (x e [a,b]) abs(f''(x))

// Método del Trapecio para aproximar una integral definida
// Entrada: f: función
//          a,b: extremos del intervalo de integración
function A = metTrapecio(f,a,b)
    h = (b-a)/2;
    A = (f(a) + f(b))*h;
endfunction

// Encontrar una cota para el error del método del Trapecio
// Entrada: a,b: extremos del intervalo de integración
//          n cantidad de subintervalos
//          cota: cota superior de f''(x)
function x = cotaErrorTrapecio(a,b,n,cota)
    h = (b-a)/n;
    x = (h**2)*(b-a)/12 * cota;
endfunction

// Método del Trapecio compuesto para aproximar una integral definida
// Entrada: f: función
//          a,b: extremos del intervalo de integración
//          n: Cantidad de subintervalos
function A = metTrapecioComp(f,a,b,n)
    h = (b-a)/n;
    for j=1:n-1
        x(j) = a + j*h;
    end
    
    A = 1/2*(f(a)+f(b));
    for j=1:n-1
        A = A + f(x(j));
    end
    A = A*h;
endfunction
