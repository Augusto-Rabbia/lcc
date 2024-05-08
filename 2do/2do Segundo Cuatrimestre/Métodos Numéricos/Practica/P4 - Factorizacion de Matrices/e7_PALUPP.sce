/*
A = matrizConduccionCalor(2)

[U,L,P] = PALUPP(A)
 U  = 

   5.  -2.    0.         0.          0.       
   0.   4.2  -2.         0.          0.       
   0.   0.    4.047619  -2.          0.       
   0.   0.    0.         4.0117647  -2.       
   0.   0.    0.         0.          4.0029326
 L  = 

   1.    0.          0.          0.          0.
  -0.4   1.          0.          0.          0.
   0.   -0.4761905   1.          0.          0.
   0.    0.         -0.4941176   1.          0.
   0.    0.          0.         -0.4985337   1.
 P  = 

   1.   0.   0.   0.   0.
   0.   1.   0.   0.   0.
   0.   0.   1.   0.   0.
   0.   0.   0.   1.   0.
   0.   0.   0.   0.   1.
*/

function [U, L, P] = PALUPP(A)
    // Entrada: A matriz n*n no singular
    // Salida: factorizació PA = LU
    [n, m] = size(A);
    if n<>m then
        error('Invalid matrix sizes.');
        abort;
    end
    
    U = A;
    L = eye(n,n);
    P = L;
    
    for k=1:n-1
        pivote=k;
        entradamax = abs(U(k,k));
        for i=k+1:n
            entradatest = abs(U(i,k))
            if entradatest>entradamax then
                pivote=i;
                entradamax = entradatest;
            end
        end
        
        temp = U(pivote,k:n); U(pivote,k:n) = U(k,k:n); U(k,k:n) = temp;
        
        temp = L(pivote,1:k-1); L(pivote,1:k-1) = L(k,1:k-1); L(k,1:k-1) = temp;
        
        temp = P(pivote,:); P(pivote,:) = P(k,:); P(k,:) = temp;
        
        for i=k+1:n
            L(i,k) = U(i,k)/U(k,k);
            U(i,k+1:n) = U(i,k+1:n) - U(k,k+1:n)*L(i,k);
            U(i,1:k) = 0;  // no hace falta para calcular la solución x
        end;
    end
endfunction



/*
Ejemplo 1:
A = [1 4 2; 3 1 2; 1 6 4]
[U, L, P] = PALUPP(A)

 P  = 

   0.   1.   0.
   0.   0.   1.
   1.   0.   0.

 L  = 

   1.          0.          0.
   0.3333333   1.          0.
   0.3333333   0.6470588   1.

 U  = 

   3.   1.          2.       
   0.   5.6666667   3.3333333
   0.   0.         -0.8235294
*/


