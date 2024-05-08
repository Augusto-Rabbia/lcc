/*
Res parcial:
A = matrizConduccionCalor(2)

[U,L,P] = PALUPP(A)

x0 = b = [10,12,12,12,10];

x1 = resLU(L,U,x0) = [5.2, 8., 8.8, 8., 5.2]

x2 = resLU(L,U,x1) = [3.1323077, 5.2307692, 5.9446154, 5.2307692, 3.1323077]

x3 = resLU(L,U,x2) = [1.9897751, 3.4082840, 3.9155503, 3.4082840, 1.9897751]

x4 = resLU(L,U,x4) = [1.2857111, 2.2193901, 2.5586221, 2.2193901, 1.2857111]

*/

function x = resLU(L,U,b)
    n = size(U,1);
    
    // Sustitución progresiva
    y(1) = b(1) / L(1,1);
    for i=2:n
        y(i) = (b(i) - L(i,1:i-1)*y(1:i-1))/L(i,i);
    end;
    
    // Sustitución regresiva
    x(n) = y(n)/U(n,n);
    for i=n-1:-1:1
        x(i) = (y(i)-U(i,i+1:n)*x(i+1:n))/U(i,i);
    end;
    
endfunction
