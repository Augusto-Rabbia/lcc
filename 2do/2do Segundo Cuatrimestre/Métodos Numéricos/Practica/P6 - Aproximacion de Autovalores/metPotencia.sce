/*
Res parcial

A = [9 1 -2 1; 0 8 1 1; -1 0 7 0; 1 0 0 1]

AValPotencia(A,[1 1 1 1]',0.00001,100) = 9.7048613
64 iteraciones realizadas.

AValPotencia(A,[1 0 0 1]',0.00001,100) = 9.7048584
56 iteraciones realizadas.
*/

function [valor,z] = AValPotencia(A,z0,eps,maxIter)
    
    valor = 0;
    w = A * z0;
    z = w / norm(w,%inf);
    
    [m,j] = max(abs(w));
    valor = w(j);
    
    iter = 1;
    
    while((iter <= maxIter) & (norm(z-z0,%inf) > eps))
        z0 = z;
        w = A* z0;
        z = w / norm(w,%inf);
        [m,j] = max(abs(w));
        valor = w(j);
        iter=iter+1;
    end
    
    printf("%d iteraciones realizadas.", iter);
endfunction

/*
A = [6 0 3; 0 1 1; 1 0 1]

[val, z] = AValPotencia(A,[1,1,1]',0.001,100)

val = 6.5414236
z = [1, 0.0327031, 0.1804614]

5 iteraciones realizadas.
*/


