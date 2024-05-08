// La función graficar necesita que en fun, cuando se multiplica o eleva
// entre términos que continenen a x, se necesitará poner un punto.
// Esto es pq se está trabajando con matrices en realidad.
// Ejemplos:
// x.*x
// cos(x).*x
// x.^x
// sin(x)^x
function graficar(fun,x0,xn,step)
    x = x0:step:xn;
    
    deff("y=f(x)", "y="+fun);
    
    y = f(x);
    disp(x)
    disp(y)
    plot2d(x,y);
endfunction
