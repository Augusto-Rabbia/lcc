function w = L(pol, x, i)
    w = 1;
    n = length(x);
    for j=1:n
        if j<>i then
            w = w*(p-x(j))/(x(i)-x(j));
        end
    end
endfunction


// Polinomio interpolante

function w = Lagrange(p, x, y)
    w = 0;
    n = length(x);
    for j=1:n
        w = w + y(j)*L(p,x,j); // LaTex $\sum_{i=1}^{n} y_i * L(p, x, i)$
    end
endfunction



// Diferencias divididas

function w = DD(x,y) 
    n = length(x);
    if n==2 then
        w = (y(2)-y(1))/(x(2)-x(1));
    else
        w = (DD(x(2:n), y(2:n)) - DD(x(1:n-1), y(1:n-1)))/(x(n)-x(1));
    end

endfunction


function w = NewtonPol(p,x,y)
   n = length(x);
   w = 0;
   for j=n:-1:2
       w = (w + DD(x(1:j), y(1:j)))*(p-x(j-1))
   end
   w = w + y(1);
endfunction









