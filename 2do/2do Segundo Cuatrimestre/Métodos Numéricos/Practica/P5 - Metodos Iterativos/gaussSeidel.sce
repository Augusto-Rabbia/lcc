function x = GaussSeidel(A, b, x0, eps, maxIter)
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
    
    x=x0; // Condición inicial
    n = nA;
    err = eps + 1;
    iter = 0;
    
    while err > eps && iter < maxIter // Método de Gauss-Seidel
        xk = x;
        x(1) = (1/A(1,1))*(b(1) - A(1,2:n)*x(2:n));
        for i=2:n-1
            x(i) = (1/A(i,i))*(b(i) - A(i,1:i-1)*x(1:i-1) - A(i,i+1:n)*x(i+1:n));
        end
        x(n) = (1/A(n,n))*(b(n) - A(n,1:n-1)*x(1:n-1));
        err = norm(x-xk);
        iter = iter + 1;
    end
    
    printf("%d iteraciones realizadas.", iter);
endfunction
/*
A = [6 0 3; 0 1 1; 1 0 1]
b = [1 3 4]'
GaussSeidel(A,b,[0,0,0]',0.001,100) = [-3.6664327, -4.6661987, 7.6664327]

15 iteraciones realizadas.
*/
