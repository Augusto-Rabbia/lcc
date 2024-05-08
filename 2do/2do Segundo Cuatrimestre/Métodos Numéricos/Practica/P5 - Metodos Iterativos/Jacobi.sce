function x = jacobi(A, b, x0, eps, maxIter)
    [nA, mA] = size(A);
    [nB, mB] = size(b);
    if nA<>mA || mA<>nB || mB <> 1 then
        error('Invalid matrix sizes');
        abort;
    end
    
    n = nA;
    x = x0;
    xk = x;
    
    for i=1:n                   //iteración 1 método de Jacobi
        suma = 0;
        for j=1:i-1
            suma = suma + A(i,j) * xk(j);
        end
        for j=i+1:n
            suma = suma + A(i,j) * xk(j);
        end
        x(i) = 1/A(i,i) *(b(i)-suma);
    end
    
    iter = 1;
    
    while (norm(x-xk) > eps && iter < maxIter)    //Método de Jacobi
        xk = x;
        for i=1:n
            suma = 0;
            for j=1:i-1
                suma = suma + A(i,j) * xk(j);
            end
            for j=i+1:n
                suma = suma + A(i,j) * xk(j);
            end
            x(i) = 1/A(i,i) *(b(i)-suma);
        end
        iter = iter+1;
    end
    printf("%d iteraciones realizadas.", iter);
endfunction

/*
A = [6 0 3; 0 1 1; 1 0 1]
b = [1 3 4]'
jacobi(A,b,[0,0,0]',0.001,100) = [-3.6657308, -4.6647949, 7.6657715]

25 iteraciones realizadas.
*/

