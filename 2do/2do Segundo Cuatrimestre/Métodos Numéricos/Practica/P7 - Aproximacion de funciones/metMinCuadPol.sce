
// Aproximación polinomial de mínimos cuadrados polinomial para matrices con rango completo
function [p,err] = minCuad_pol(x,y,n)
    // Entrada: x,y = vectores 1xn; n = grado de la aproximación
    // Salida: p = polinomio de mínimos cuadrados; err = vector de errores (eps = Ax-b)
    
    // Calculando la matriz del método
    A = matriz_mc(x,n);
    b = y;
    
    // Aplicando el método de mínimos cuadrados
    w = gausselimPP((A')*A,(A')*(b'));
    p = poly(w,"x","coeff");
    err = A*w-b';
endfunction

// Matriz del método de mínimos cuadrados polinomial
function A = matriz_mc(x,n)
    // Entrada: x vectore 1xn; n = grado de la aproximación
    // Salida: A = matriz del método de mínimo cuadrados
    m = length(x);
    A = ones(m,1);
    for j=2:(n+1) do
        A = [A,(x').^(j-1)];
    end
endfunction

// Solución de sistema de ecuaciones por Eliminación Gaussiana. 
// (No es exactamente la función que hice para la unidad 4 ya que no calcula/devuelve la matriz aumentada)
function x = gausselimPP(A,b)
    // Esta función obtiene la solución del sistema de ecuaciones lineales A*x=b, 
    // dada la matriz de coeficientes A y el vector b.
    // La función implementa el método de Eliminación Gaussiana con pivoteo parcial.
    
    [nA,mA] = size(A) 
    [nb,mb] = size(b)
    
    if nA<>mA || mA<>nb || mb<>1 then
        error('Invalid matrix sizes.');
        abort;
    end
    
    aum = [A b]; // Matriz aumentada
    n = nA;      // Tamaño de la matriz
    for k=1:n-1
        // Pivoteo Parcial
        kpivot = k; amax = abs(aum(k,k));  //pivoteo
        for i=k+1:n
            if abs(aum(i,k))>amax then
                kpivot = i; amax = aum(i,k);
            end
        end
        temp = aum(kpivot,:); aum(kpivot,:) = aum(k,:); aum(k,:) = temp;
        
        // Eliminación progresiva
        for i=k+1:n // No hace falta calcular los valores i<=k
            OpElemental = aum(i,k)/aum(k,k);
            aum(i,k+1:n+1) = aum(i,k+1:n+1) - aum(k,k+1:n+1)*OpElemental;
        end
    end
    
    // Sustitución regresiva
    x(n) = aum(n,n+1)/aum(n,n);
    for i = n-1:-1:1
        x(i) = (aum(i,n+1)-aum(i,i+1:n)*x(i+1:n))/aum(i,i);
    end;
endfunction
