// SOR
// Por teorema, si a_ii = 0 para todo i, SOR converge solo si 0 < w < 2
function x = SOR(A, b, x0, w, eps, maxIter)
    [nA, mA] = size(A);
    [nB, mB] = size(b);
    if nA<>mA || mA<>nB || mB <> 1 then
        error('Invalid matrix sizes');
        abort;
    end
    
    x=x0 // Condición inicial
    n = nA;
    err = eps + 1;
    iter = 0;
    
    while err > eps && iter < maxIter // Método de SOR
        xk = x;
        x(1) = (1-w)*x(1) + (w/A(1,1))*(b(1) - A(1,2:n)*x(2:n));
        for i=2:n-1
            x(i) = (1-w)*x(i) + (w/A(i,i))*(b(i) - A(i,1:i-1)*x(1:i-1) - A(i,i+1:n)*x(i+1:n));
        end
        x(n) = (1-w)*x(n) + (w/A(n,n))*(b(n) - A(n,1:n-1)*x(1:n-1));
        err = norm(x-xk, %inf);
        iter = iter + 1;
    end
    
    printf("%d iteraciones realizadas.", iter);
endfunction

// Omega óptimo
function w = omega_SOR(A)
    // Entrada: A matriz nxn tridiagonal y definida positiva
    // Salida: w óptimo del método de SOR
    
    [n,m] = size(A);
    T_j = eye(n,n)-diag(1./diag(A))*A;
    autovalores = spec(T_j);
    rho = max(abs(autovalores));
    w = 2/(1+sqrt(1-rho^2));
endfunction
