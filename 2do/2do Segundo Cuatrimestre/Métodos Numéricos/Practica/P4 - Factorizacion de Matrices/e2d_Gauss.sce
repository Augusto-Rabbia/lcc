function x = gaussElim(A,b)
    // Entrada: A matriz n*n, b vector de Rn
    // Salida: Solución al sistema Ax = b.
    // Nota: la función no realiza pivoteo.
    [nA, mA] = size(A);
    [nB, mB] = size(b);
    if nA<>mA || mA<>nB || mB <> 1 then
        error('Invalid matrix sizes.');
        abort;
    end

    aum = [A b]; // Matriz aumentada
    // Eliminación progresiva
    n = nA;
    for k=1:n-1
        for i=k+1:n
            OpElemental = aum(i,k)/aum(k,k);
            aum(i,k+1:n+1) = aum(i,k+1:n+1) - aum(k,k+1:n+1)*OpElemental;
        end;
    end;
    
    // Sustitución regresiva
    x(n) = aum(n,n+1)/aum(n,n);
    for i = n-1:-1:1
        x(i) = (aum(i,n+1)-aum(i,i+1:n)*x(i+1:n))/aum(i,i);
    end;
endfunction


/*
Ejemplo 1:
A = [1 2 4; 5 1 3; 1 1 1]
b = [7 9 3]'
gaussElim(A,b) = [1 1 1]'

Ejemplo 2:
A = [1 42 3; 4 5 3; 0 1 9]
b = [5 12 17]'
gaussElim(A,b) = [1.6481481 -0.0555556 1.8950617]'

*/
