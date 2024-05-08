// Verificación de la convergencia de los métodos iterativos:
// Sea A = L+D+U
/* Sabemos que 
Nx(k+1) = (N - A)xk + b
 x(k+1) = N-1((N-A)xk + b) = (I - N-1*A)*xk + N-1*b
*/

/* Teorema 1: a 1 Si ∥I − N-1 * A∥ < 1, entonces la sucesión {xk}, 
definida por el proceso iterativo anterior, converge a la solución del 
sistema Ax = b para cualquier estimación inicial x0 ∈ R
*/
function r = convergenciaJacT1(A)
    // Entrada: A matriz n*n.
    // Nota: La función asume que A no tiene ningún 0 en la diagonal,
    // por lo que puede ser necesario hacer pivoteo antes de llamarla.
    // Salida: La norma de I − N-1 * A.
    [n, m] = size(A);
    if n<>m then
        disp("Invalid matrix size.");
        abort();
    end
    
    for i=1:n
        Ninv(i,i) = 1/A(i,i);
    end
    r = norm(eye(n,n)-(Ninv*A))
    if r < 1 then
        disp("Jacobi convergirá para cualquier x0 inicial.");
    else
        disp("No es posible asegurar que Jacobi convergirá para cualquier x0 inicial.");
    end
endfunction

function r = convergenciaGST1(A)
    [n, m] = size(A);
    if n<>m then
        disp("Invalid matrix size.");
        abort();
    end
    
    
    for i=1:n
        for j=1:i
            N(i,j) = A(i,j);
        end
    end
    
    r = norm(eye(n,n)-(inv(N)*A))
    if r < 1 then
        disp("Gauss-Seidel convergirá para cualquier x0 inicial.");
    else
        disp("No es posible asegurar que Gauss-Seidel convergirá para cualquier x0 inicial.");
    end
endfunction



/*
Teorema 2: Sea A ∈ Rn×n. El proceso iterativo x(k+1) = B*xk converge a 
x = 0 para todo vector inicial x0 ∈ Rn si y solo si ρ(A) < 1.
*/
function r = convergenciaSii(A)
    [n, m] = size(A);
    if n<>m then
        disp("Invalid matrix size.");
        abort();
    end
    r = max(abs(spec(A)))
    if r < 1 then
        disp("Los procesos iterativos convergirán para cualquier x0 inicial");
    else
        disp("Existen x0 tal que el proceso iterativo no convergerá.");
    end 
endfunction

/*
Teorema 3-4: Si la matriz A ∈ Rn×n es a diagonal dominante, luego la sucesión
{xk}  generada por el método de Jacobi o por el método de Gauss-Seidel 
converge a la solución del sistema Ax = b para cualquier x0 inicial.
*/
function convergenciaJacyGS_T3_4(A)
    [n, m] = size(A);
    if n<>m then
        disp("Invalid matrix size.");
        abort();
    end
    
    diagDominante = 1;
    for i=1:n
        for j=1:n
            if (A(i,i) < A(i,j)) then
                diagDominante = 0;
                break;
            end
        end
        if(diagDominante == 0) then
            break;
        end
    end
    
    if diagDominante == 1 then
        disp("Jacobi y Gauss-Seidel convergirán para cualquier x0 inicial.");
    else
        disp("No es posible asegurar que Jacobi o Gauss-Seidel convergirán para cualquier x0 inicial.");
    end
endfunction
