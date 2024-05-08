function circ(r,x,y)
    xarc(x-r,y+r,2*r,2*r,0,360*64)
endfunction

function dibujarGersch(A)
    [n,m] = size(A);
    centros = diag(A);
    radios = sum(abs(A),'c') - abs(centros) ;
    
    // buscamos calcular un rectángulo que contenga a todos los circulos
    // esquina inferiro izquierda
    
    mx = round (min(centros - radios)-1);
    my = round (min(-radios)-1);
    
    // esquina superior derecha
    
    Mx = round(max(centros+radios)+1);
    My = round(max(radios)+1);
    
    rectangulo = [mx my Mx My];
    
    // dibujamos los autovalores
    plot2d(real(spec(A)),imag(spec(A)),-1,"035","",rectangulo)
    replot(rectangulo); // reeplaza al rect
    xgrid();
    
    for i=1:n
        circ(radios(i),centros(i),0)
    end
endfunction

function dibujarGerschPol(pol)
    n = degree(pol);
    coeffsPoly = coeff(pol);
    
    
    centroNoTrivial = -coeffsPoly(n);
    radioNoTrivial = 0;
    for i=1:n-1
        radioNoTrivial = radioNoTrivial + coeffsPoly(i);
    end
    
    // buscamos calcular un rectángulo que contenga a todos los circulos
    // esquina inferiro izquierda
    
    mx = round (min(centroNoTrivial - radioNoTrivial, -1)-1);
    my = round (min(-radioNoTrivial, 1)-1);
    
    // esquina superior derecha
    
    Mx = round(max(centroNoTrivial+radioNoTrivial, 1)+1);
    My = round(max(radioNoTrivial, 1)+1);
    
    rectangulo = [mx my Mx My];
    
    // dibujamos los autovalores
    plot2d(real(roots(pol)),imag(roots(pol)),-1,"035","",rectangulo)
    replot(rectangulo); // reeplaza al rect
    xgrid();
    
    circ(1, 0, 0)
    circ(radioNoTrivial,centroNoTrivial,0)
endfunction

function dibujarGerschConTranspuesta(A)
    [n,m] = size(A);
    centros = diag(A);                          // Centros de A
    abscentros = abs(centros);                  // Calculamos el valor absoluto una única vez
    radios = sum(abs(A),'c') - abscentros;      // Radios de A
    centros(n+1:2*n) = centros;                 // Centros de A' (= centros de A)
    radios(n+1:2*n) = sum(abs(A),'r')' - abscentros;    // Radios de A'
    
    // buscamos calcular un rectángulo que contenga a todos los circulos
    // esquina inferiro izquierda
    
    mx = round (min(centros - radios)-1);
    my = round (min(-radios)-1);
    
    // esquina superior derecha
    
    Mx = round(max(centros+radios)+1);
    My = round(max(radios)+1);
    
    rectangulo = [mx my Mx My];
    
    // dibujamos los autovalores
    plot2d(real(spec(A)),imag(spec(A)),-1,"035","",rectangulo)
    replot(rectangulo); // reeplaza al rect
    xgrid();
    
    for i=1:n+n
        circ(radios(i),centros(i),0)
    end
    gca().children(1:n).foreground = color("red");
    gca().children(n+1:2*n).foreground = color("blue");
endfunction

