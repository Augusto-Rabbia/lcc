function x = GaussSeidelMatrices(A, b, x0, eps, maxIter)
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
    
    for i=1:n
        for j=1:i
            LD(i,j) = A(i,j);
        end
    end
    N = inv(LD);             // N inversa de la diagonal de A
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
A = [6 0 3; 0 1 1; 1 0 1]
b = [1 3 4]'
GaussSeidelMatrices(A,b,[0,0,0]',0.001,100) = [-3.6664327, -4.6661987, 7.6664327]

15 iteraciones realizadas.
*/
