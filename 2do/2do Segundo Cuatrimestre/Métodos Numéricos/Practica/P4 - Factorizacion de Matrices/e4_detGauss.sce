function d = detGauss(A)
    // Entrada: A matriz n*n
    // Salida: det(A), a través de eliminación de Gauss.
    // Nota: la función no realiza pivoteo.
    [nA, mA] = size(A);
    
    if nA<>mA then
        error('Invalid matrix sizes.');
        abort;
    end
    
    // Eliminación progresiva
    n = nA;
    for k=1:n-1
        for i=k+1:n             //No hace falta modificar los valores debajo de la diagonal
            OpElemental = A(i,k)/A(k,k);
            A(i,k+1:n) = A(i,k+1:n) - A(k,k+1:n)*OpElemental;
        end;
    end;
    
    d=A(1,1);
    for i=2:n
        d = d * A(i,i);
    end
endfunction

/*
Ejemplo 1:
A = [1 1; 3 4]
detGauss(A) = 1

Ejemplo 2:
A = [5 7 1; 51 1 9; -3 8 -1]
detGauss(A) = 214
*/
