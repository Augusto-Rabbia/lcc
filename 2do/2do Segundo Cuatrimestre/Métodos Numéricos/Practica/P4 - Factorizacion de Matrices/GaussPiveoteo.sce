function [x,aum] = gausselimPP(A,b)
    // Esta función obtiene la solución del sistema de ecuaciones lineales A*x=b, 
    // dada la matriz de coeficientes A y el vector b.
    // La función implementa el método de Eliminación Gaussiana con pivoteo parcial.
    
    [nA,mA] = size(A) 
    [nb,mb] = size(b)
    
    if nA<>mA || mA<>nb || mb<>1 then
        error('Invalid matrix sizes.');
        abort;
    end
    
    aum = [A b]; // Matriz aumentada
    n = nA;    // Tamaño de la matriz
    for k=1:n-1
        // Pivoteo Parcial
        kpivot = k; amax = abs(aum(k,k));  //pivoteo
        for i=k+1:n
            if abs(aum(i,k))>amax then
                kpivot = i; amax = aum(i,k);
            end
        end
        temp = aum(kpivot,:); aum(kpivot,:) = aum(k,:); aum(k,:) = temp;
        
        // Eliminación progresiva
        for i=k+1:n
            OpElemental = aum(i,k)/aum(k,k);
            aum(i,k+1:n+1) = aum(i,k+1:n+1) - aum(k,k+1:n+1)*OpElemental;
            aum(i,1:k) = 0;  // no hace falta para calcular la solución x
        end
    end
    
    // Sustitución regresiva
    x(n) = aum(n,n+1)/aum(n,n);
    for i = n-1:-1:1
        x(i) = (aum(i,n+1)-aum(i,i+1:n)*x(i+1:n))/aum(i,i);
    end;
endfunction

/*
// Ejemplo de aplicación
A2 = [0 2 3; 2 0 3; 8 16 -1]
b2 = [7 13 -3]'

[x2,a2] = gausselimPP(A2,b2)
disp(x2)
disp(a2)
*/
