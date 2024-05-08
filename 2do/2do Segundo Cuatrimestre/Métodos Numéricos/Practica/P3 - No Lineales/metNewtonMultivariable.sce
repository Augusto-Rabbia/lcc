function xn = metNewtonMultivariable(f, x0, tol, iter)
    if iter ~= 0 then
        J = numderivative(f, x0);
        Jinv = inv(J);
        x1 = x0 - Jinv * f(x0);
        i = 1;
        while abs(x0 - x1) > tol && i < iter
            x0 = x1;
            J = numderivative(f, x0);
            Jinv = inv(J);
            x1 = x0 - Jinv * f(x0);
            i = i + 1;
        end
        xn = x1;
    else
        xn = x0;
    end
    
endfunction
