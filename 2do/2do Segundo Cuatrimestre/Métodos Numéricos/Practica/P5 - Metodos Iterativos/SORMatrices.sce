function x = SORMatrices(A, b, x0, w, eps, maxIter)
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
    
    L = zeros(n,n);
    U = L;
    for i=1:n
        for j=1:i-1
            L(i,j) = A(i,j);
        end
        for j=i+1:n
            U(i,j) = A(i,j);
        end
    end
    D = diag(diag(A));
    N = inv(w*L+D);             // N inversa de la diagonal de A
    T = N*((1-w)*D - w*U);         // M = (I-(D-1*A))
    c = w*N*b;
    
    x = x0;
    xk = x;
    x = c+T*xk;
    iter = 1;
    
    while norm(x-xk) > eps && iter < maxIter
        xk = x;
        x = c+T*xk;
        iter = iter + 1;
    end
    printf("%d iteraciones realizadas.", iter);
endfunction
/*
A = [6 0 3; 0 1 1; 1 0 1]
b = [1 3 4]'
SOR(A,b,[0,0,0]',1.25,0.001,100) = [-3.6666395, -4.6666021, 7.6666687]

9 iteraciones realizadas.
*/
