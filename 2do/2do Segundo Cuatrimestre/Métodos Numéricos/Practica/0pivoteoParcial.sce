function B = pivoteoParcial (A)
    n = size(A, 1);
    B = A;
    for k=1:n-1
        pivote=k;
        entradamax = abs(B(k,k));
        for i=k+1:n
            entradatest = abs(B(i,k))
            if entradatest>entradamax then
                pivote=i;
                entradamax = entradatest;
            end
        end
        temp = B(pivote,:); B(pivote,:) = B(k,:); B(k,:) = temp;
    end

endfunction
