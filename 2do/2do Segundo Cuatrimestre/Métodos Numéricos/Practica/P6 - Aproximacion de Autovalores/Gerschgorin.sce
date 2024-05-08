function [c, r] = Gerschgorin(A)
    n = size(A,1);
    for i=1:n
        r(i)=0;
        for j=1:i-1
            r(i) = r(i) + abs(A(i,j));
        end
        for j=i+1:n
            r(i) = r(i) + abs(A(i,j));
        end
        c(i) = A(i,i);
    end
endfunction

function [c, r] = GerschgorinPol(p)
    n = degree(p);
    
    coeffsPoly = coeff(p);
    r = 0;
    for i=1:n-1
        r = r + coeffsPoly(i);
    end
    
    c = -coeffsPoly(n);
endfunction


// Hay que llevar los metodos con un ej de prueba
// Estudiar los metodos tmb
// Cuando resolvamos el examen hay que escribir la entrada y salida de las sols del examen.
