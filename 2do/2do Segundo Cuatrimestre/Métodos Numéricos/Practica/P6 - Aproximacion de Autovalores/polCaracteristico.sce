function p = polCaracteristico(A)
    x = poly([0,1],"x","coeff")
    p = det(A-x*eye(3,3))
endfunction
