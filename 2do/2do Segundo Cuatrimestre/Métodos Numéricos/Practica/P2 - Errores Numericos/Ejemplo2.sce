//clc // limpia la consola
//clear // borra el contenido de la memoria
// Primera funcion
function y = P1(x)
y = x.^7 - 7*x.^6 + 21*x.^5 - 35*x.^4 + 35*x.^3 - 21*x.^2 + 7*x - 1;
endfunction
// Segunda funcion
function y = P2(x)
y = (x - 1).^7;
endfunction
// Evaluacion de ambas funciones cerca de uno
x = linspace(1-1e-2,1+1e-2,2001);
y1 = P1(x);
y2 = P2(x);
// Grafica de las funciones
plot(x,y1,'b');
plot(x,y2,'b','thickness',2)
legend(["$P1(x)$";"$P2(x)$"]);
