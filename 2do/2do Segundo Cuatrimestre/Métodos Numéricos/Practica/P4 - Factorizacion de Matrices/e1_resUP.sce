function x = resTriangSup(A, b)
    // Entrada: A matriz triangular superior n*n, b vector de Rn
    // Salida: Solución al sistema Ax = b.
    [nA, mA] = size(A);
    [nB, mB] = size(b);
    if nA<>mA || mA<>nB || mB <> 1 then
        error('Invalid matrix sizes.');
        abort;
    end
    
    aum = [A b]; // Matriz aumentada
    n = nA;
    
    // Sustitución regresiva
    x(n) = aum(n,n+1)/aum(n,n);
    for i = n-1:-1:1
        x(i) = (aum(i,n+1)-aum(i,i+1:n)*x(i+1:n))/aum(i,i);
    end;
endfunction


/*
Ejemplo 1:
A = [1 1 2; 0 1 1; 0 0 5]
b = [9 5 15]'
resTriangSup(A, b) = [1 2 3]'

Ejemplo 2:
A = [5 0 1; 0 2 0; 0 0 1]
b = [6 2 1]'
resTriangSup(A, b) = [1 2 3]'
*/
