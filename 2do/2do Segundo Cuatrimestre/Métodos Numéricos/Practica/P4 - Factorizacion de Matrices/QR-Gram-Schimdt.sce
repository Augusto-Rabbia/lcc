function [Q, R] = factQR(A)
    // Entrada: A matriz n*m con columnas LI 
    // Sino, se realizará Gram-Schmidt sobre ellas.
    // Salida: Factorización QR
    
    [n,m] = size(A);
    
    if n>m then
        disp("invalid matrix size.");
        abort();
    end
    
    R(1,1) = norm(A(:,1));
    q = A(:,1) / R(1,1);
    Q(:,1) = q;
    for j=2:n
        R(1,j) = A(:,j)'*q;
    end
    for k=2:n
        a = A(:,k);
        sumatoria = 0;
        for i=1:k-1
            sumatoria = sumatoria + (a' * Q(:,i)) * Q(:,i);
        end
        
        h = a - sumatoria
        R(k,k) = norm(h);
        q = h/R(k,k);
        Q(:,k) = q;
        for j=k+1:n
            R(k,j) = A(:,j)'*q;
        end
    end
endfunction

function x = sistQR(A, b)
    [n,m] = size(A);
    
    [Q, R] = factQR(A);     // Obteniendo factorización QR de A
    
    Qb = Q' * b;
    
    aum = [R Qb];
    // Sustitución regresiva;
    x(n) = aum(n,n+1)/aum(n,n);
    for i = n-1:-1:1
        x(i) = (aum(i,n+1)-aum(i,i+1:n)*x(i+1:n))/aum(i,i);
    end;
endfunction

function Q = GramSchmidt(A) 
    // Entrada: A matriz m*n con columnas LI 
    // Salida: B matriz m*n cuyas columnas son una base del EV generado
    // por las columnas de A
    [m,n] = size(A);
    
    Q(:,1) = A(:,1) / norm(A(:,1));
    for k=2:n
        a = A(:,k);
        sumatoria = 0;
        for i=1:k-1
            sumatoria = sumatoria + (a' * Q(:,i)) * Q(:,i);
        end
        a = a - sumatoria;
        Q(:,k) = (a)/norm(a);
    end
endfunction

/*
A = [1 2 3; 3 1 2; -1 -1 -1]

[Q, R] = factQR(A)

 R  = 
   3.3166248   1.8090681   3.0151134
   0.          1.6514456   2.1468793
   0.          0.          0.5477226

 Q  = 
   0.3015113   0.880771    0.3651484
   0.904534   -0.3853373   0.1825742
  -0.3015113  -0.2752409   0.9128709

b = [5, 5, 5]'
sistQR(A) = [-1.6666667 -16.666667 13.333333]
*/
