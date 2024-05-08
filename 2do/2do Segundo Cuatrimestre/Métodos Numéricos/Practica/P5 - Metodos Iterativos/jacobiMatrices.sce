/*
Res parcial

A = [10 -1 2 0; -1 11 -1 3; 2 -1 10 -4; 0 3 -1 8]
b = [6 25 -11 15]'

jacobiMatrices(A,b,[0,0,0,0]',0.00001,100) = [0.9357149, 2.0142849, -0.6714275, 1.0357134]

20 iteraciones realizadas.
*/

function x = jacobiMatrices(A, b, x0, eps, maxIter)
    [nA, mA] = size(A);
    [nB, mB] = size(b);
    if nA<>mA || mA<>nB || mB <> 1 then
        error('Invalid matrix sizes');
        abort;
    end
    n = nA
    
    // Pivoteo Parcial
    aum = [A b]
    for k=1:n
        pivote=k;
        entradamax = abs(aum(k,k));
        for i=k+1:n
            entradatest = abs(aum(i,k))
            if entradatest>entradamax then
                pivote=i;
                entradamax = entradatest;
            end
        end
        temp = aum(pivote,:); aum(pivote,:) = aum(k,:); aum(k,:) = temp;
    end
    
    A = aum(1:n,1:n);
    b = aum(:,n+1);
    
    
    D = diag(diag(A));
    N = inv(D);             // N inversa de la diagonal de A
    M = eye(n,n) - N*A;     // M = (I-(D-1*A))
    Nb = N*b;
    
    x = x0;
    xk = x;
    x = Nb+M*xk;
    iter = 1;
    
    while norm(x-xk) > eps && iter < maxIter
        xk = x;
        x = Nb+M*xk;
        iter = iter + 1;
    end
    printf("%d iteraciones realizadas.", iter);
endfunction

/*
Ejemplo:
A = [6 0 3; 0 1 1; 1 0 1]
b = [1 3 4]'
jacobiMatrices(A,b,[0,0,0]',0.001,100) = [-3.6657308, -4.6647949, 7.6657715]

25 iteraciones realizadas.
*/
