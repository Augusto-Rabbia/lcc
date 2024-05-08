function [x, U] = sistCholesky(A, b)
    [nA, mA] = size(A);
    [nB, mB] = size(b);
    if nA<>mA || mA<>nB || mB <> 1 then
        error('Invalid matrix sizes');
        abort;
    end
    
    n = nA;
    eps = 1.0e-8
    U = zeros(n,n)
    
    // Cholesky
    t = A(1,1);
    if t <= eps then
        printf("Matriz no definida positiva.\n");
        ind = 0;
        return;
    end
    U(1,1)= sqrt(t);
    for j = 2:n
        U(1,j) = A(1,j)/U(1,1);
    end
    
    for k = 2:n
        t = A(k,k) - U(1:k-1,k)'*U(1:k-1,k);
        if t <= eps then
            printf("Matriz no definida positiva.\n");
            ind = 0;
            return;
        end
        U(k,k)= sqrt(t);
        for j = k+1:n
            U(k,j) = (A(k,j) - U(1:k-1,k)' * U(1:k-1,j))/U(k,k);
        end
    end
    ind = 1;
    
    // Sustitución progresiva
    UT = U';
    y(1) = b(1) / UT(1,1);
    for i=2:n
        y(i) = (b(i) - UT(i,1:i-1)*y(1:i-1))/UT(i,i);
    end;
    
    // Sustitución regresiva
    x(n) = y(n)/U(n,n);
    for i=n-1:-1:1
        x(i) = (y(i)-U(i,i+1:n)*x(i+1:n))/U(i,i);
    end;
endfunction
