function [L, U] = luDoolittle(A)
    [n, m] = size(A);
    if n<>m then
        disp("Invalid matrix size.");
        abort();
    end
    
    L = eye(n,n);
    U = zeros(n,n);
    
    // Método de Doolittle
    U(1,:) = A(1,:);                    // Casos iniciales
    L(2:n,1) = A(2:n,1)/U(1,1);
    for i=2:n
        for j=i:n
            U(i,j) = A(i,j)-L(i,1:i-1)*U(1:i-1,j);
        end
        for j=i+1:n
            L(j,i) = (A(j,i)-L(j,1:i-1)*U(1:i-1,i))/U(i,i);
        end
    end
endfunction

function [L, U] = luCrout(A)
    [n, m] = size(A);
    if n<>m then
        disp("Invalid matrix size.");
        abort();
    end
    
    U = eye(n,n);
    L = zeros(n,n);
    
    L(:,1) = A(:,1);
    for j=2:n
        for i=1:n
            if (i >= j) then
                L(i,j) = A(i,j) - L(1,1:j-1)*U(1:j-1,i);
            else
                U(i,j) = (A(i,j) - U(j,1:j-1)*L(1:j-1,j)) / L(i,i);
            end
        end
    end
endfunction

/*
A = [1 2 1; 1 4 1; 1 0 6]
[L, U] = luDoolittle(A)

U  = 
   1.   2.   1.
   0.   2.   0.
   0.   0.   5.

 L  = 
   1.   0.   0.
   1.   1.   0.
   1.  -1.   1.

A = [5 1 1;0.3 6 -3; 0 0 -2]
[L, U] = luDoolittle(A)

U  = 
   5.   1.     1.  
   0.   5.94  -3.06
   0.   0.    -2.  

 L  = 
   1.     0.   0.
   0.06   1.   0.
   0.     0.   1.
*/
