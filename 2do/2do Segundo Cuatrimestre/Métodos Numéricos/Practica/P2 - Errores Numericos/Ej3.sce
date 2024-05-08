function r = mihorner(p, x0, n, k)
    //k must start as 0
    if (n == k) then
        r = coeff(p, n);
    else
        r = coeff(p, k) + x0 * mihorner(p, x0, n, k+1);
    end
endfunction

function r = mihorner_con_derivada(p, x0, n, k)
    r(1) = 0;
    r(2) = 0;
    //k must start as 0
    if (n == k) then
        r(1) = coeff(p, n);
        r(2) = r(1);
    else
        temp =  mihorner_con_derivada(p, x0, n, k+1);
        r(1) = coeff(p, k) + x0 * temp(1);
        if k > 0 then
           r(2) = r(1) + x0 * temp(2);;
          
        else
            r(2) = temp(2);
        end
    end
endfunction

