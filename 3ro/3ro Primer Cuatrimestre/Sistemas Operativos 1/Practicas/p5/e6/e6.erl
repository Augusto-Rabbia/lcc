-module(e6).
-export([init/1, crear_contadores/3, contadores/4]).

contadores(_,_,SUM,1) ->
    io:fwrite("Terminé con ~p\n", [SUM]);
% El proceso recibe un número y lo envía a su sucesor.
contadores(N, Pid, SUM, Pasos) ->
    Pid ! N,
    receive
        M -> contadores(M, Pid, M + SUM, Pasos-1);
        _ -> mal
    end.

% El proceso 1 será el mismo que creó a todos los demás, lo que lo hace más simple,
% al proceso N simplemente le damos el Pid del proceso creado con Init().
crear_contadores(Pasos, 1, Pid) -> 
    contadores(1, Pid, 1, Pasos);
% Creamos los N procesos, y les damos como argumento el total de procesos (Pasos),
% Su número privado (en esta caso N), y el Pid del proceso con el que se tiene
% que comunicar.
crear_contadores(Pasos, N, Pid) ->
    CPid = spawn(fun() -> contadores(N, Pid, N, Pasos) end),
    crear_contadores(Pasos, N-1, CPid).

% Init toma como argumento la cantidad de procesos que se desean iniciar.
init(N) ->
    crear_contadores(N, N, self()).