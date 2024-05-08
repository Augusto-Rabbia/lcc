function r = myroots(pol)
    a = coeff(pol, 2);
    b = coeff(pol, 1);
    c = coeff(pol, 0);
    discriminante = b**2 - 4*a*c;
    sqrtdisc = sqrt(discriminante);
    if discriminante > 0 then
        if b < 0 then
            r(1) = (2*c)/(-b + sqrtdisc);
            r(2) = (-b + sqrtdisc)/(2*a);
        else
            r(1) = (2*c)/(-b - sqrtdisc);
            r(2) = (-b - sqrtdisc)/(2*a);
        end
    else
        if b < 0 then
            r(1) = (2*c)/(-b - sqrtdisc);
            r(2) = (-b - sqrtdisc)/(2*a);
        else
            r(1) = (2*c)/(-b + sqrtdisc);
            r(2) = (-b + sqrtdisc)/(2*a);
       end
    end
endfunction

