function X = gaussMulti(A, B)
    // Entrada: A matriz n*n, B matriz n*m
    // Salida: Solución al sistema AX = B.
    // Nota: la función no realiza pivoteo.
    [nA, mA] = size(A);
    [nB, mB] = size(B);
    
    if nA<>mB || mA<>nB then
        error('Invalid matrix sizes.');
        abort;
    end
    
    // Eliminación de Gauss
    aum = [A B];
    for k=1:nA
        for i=k+1:nA
            opElemental = aum(i,k)/aum(k,k)
            aum(i,k+1:nA+nB) = aum(i,k+1:nA+nB) - aum(k,k+1:nA+nB) * opElemental;
        end
    end

    // Sustitución regresiva
    for j=1:mB
        X(nA,j) = (aum(nA, nA+j))/aum(nA,nA);
        for k=nA-1:-1:1
            X(k,j) = (aum(k, nA+j) - aum(k, k+1:nA)*X(k+1:nA,j))/aum(k,k);
        end 
    end
endfunction


/*
Ejemplo:
A = [1 2 4 1; 5 1 3 1; 1 1 1 1; 1 8 3 1]
B = [8 10 4 13; 21 123 1 3; 12 1 5 1; 5 1 23 4]'
gaussMulti(A,B) = 
[1. 26.868421   -2.3947368  -2.6842105;
 1. -1.7894737  -1.3684211  -1.1052632;
 1. 7.2631579    2.7894737  -5.6315789;
 1. -31.342105   5.9736842   32.421053]
*/
