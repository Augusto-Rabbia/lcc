function [x, amp] = gaussTridiag(A, b)
    // Entrada: A matriz n*n tridiagonal
    // Salida: Solución al sistema Ax = b. Y la matriz ampliada obtenida amp
    [n, mA] = size(A);
    [nb, mb] = size(b);
    if n<>mA || n<>nb || mb<>1 then
        error('Invalid matrix sizes.');
        abort;
    end
    
    amp = [A b];
    for k=1:n-1
        opElemental = amp(k+1,k)/amp(k,k);
        amp(k+1,k+1) = amp(k+1,k+1) - amp(k+1,k) * opElemental;
        amp(k+1,n+1) = amp(k+1,n+1) - amp(k,n+1) * opElemental;
        amp(k+1,k) = 0;        // No es necesario este valor
    end
    
    x(n) = amp(n,n+1)/amp(n,n);
    for k=n-1:-1:1
        x(k) = (amp(k,n+1) - amp(k,k+1)*x(k+1))/amp(k,k);
    end
endfunction

/*
Ejemplo 1:
A = [1 1 0; 1 2 1; 0 1 2]
b = [2 4 3]'
[x, amp] = gaussTridiag(A, b)
x = [1 1 1]'
amp = 
[1. 1. 0. 2.;
 0. 1. 1. 2.;
 0. 0. 1. 1.]

Ejemplo 2:
A = [11 23 0; 2 1 4; 0 3 2]
b = [9 1 9]'
[x, amp] = gaussTridiag(A, b)
 x  = [-10.079144 5.2117647 -0.9882353
amp  = 
[11. 23.        0.          9.       ;
 0.  0.6363636  4.         -0.6363636;
 0.  0.        -12.142857   12.      ]
*/
