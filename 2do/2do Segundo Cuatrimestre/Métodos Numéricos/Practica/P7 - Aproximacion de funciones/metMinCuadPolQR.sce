function [p, err]  = metMinCuadQR(x,y,n)
    // Entrada: x,y = vectores 1xn; n = grado de la aproximación
    // Salida: p = polinomio de mínimos cuadrados; err = vector de errores (eps = Ax-b)
    
    // Calculando la matriz del método
    A = matriz_mc(x,n);
    b = y';
    
    // Aplicando el método de mínimos cuadrados
    w = sistQR(A,b);
    p = poly(w,"x","coeff");
    err = A*w-b;
endfunction

// Matriz del método de mínimos cuadrados polinomial
function A = matriz_mc(x,n)
    // Entrada: x vectore 1xn; n = grado de la aproximación
    // Salida: A = matriz del método de mínimo cuadrados
    m = length(x);
    A = ones(m,1);
    for j=2:(n+1) do
        A = [A,(x').^(j-1)];
    end
endfunction

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
