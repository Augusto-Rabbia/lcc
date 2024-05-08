%Ej. 8 (Sincronizaci´on). Complete el c´odigo siguiente para implementar Locks y Sem´aforos en Erlang
%usando paso de mensajes. Las funciones testLock/0 y testSem/0 son casos de uso.
%De ninguna manera!!! se debe usar busy waiting ni esperas arbitrarias

-module(sync).
-export([createLock/0, lock/1, unlock/1, destroyLock/1]).
-export([createSem/1, semP/1, semV/1, destroySem/1]).
-export([testLock/0, testSem/0, locker/2]).

sacar_de_lista(0, [Hd|Tl]) -> 
    {Hd, Tl};
sacar_de_lista(N, [Hd|Tl]) -> 
    {ELT, LIST} = (sacar_de_lista(N-1, Tl)),
    {ELT, [Hd|LIST]}.

% N = 1 significa que el lock no está tomado, N = 0 si el lock está tomado
locker(N, PidList) ->
    receive 
        {tomar, Pid} -> case N of
                            0 -> locker(0, [Pid|PidList]);    % Añadimos el proceso a la lista de los que espera tomar el lock
                            N -> Pid ! permitido,
                                 locker(N-1, [])
                        end;
        {soltar} -> case length(PidList) of
                        0 -> locker(N+1, []);
                        % Si hay procesos esperando, queremos despertar uno. Usamos random para simular no determinismo.
                        Len -> Selected = rand:uniform(Len)-1, 
                               {PidToWake, NewPidList} = sacar_de_lista(Selected, PidList),
                               PidToWake ! permitido,
                               locker(N, NewPidList)
                    end;
        {destroy} -> ok;
        _ -> locker(N, PidList)
    end.

% Creamos un procesos locker con valor 1 y la lista vacía.
createLock () -> 
    spawn(fun() -> locker(1, []) end).
% "Tomar el lock" es esperar un mensaje de permiso para avanzar 
% a la región crítica desde el locker.
lock(L) -> 
    L ! {tomar, self()},
    receive
        permitido -> ok;
        _ -> lock(L)
    end.
% "Soltar el lock" es enviar un mensaje al locker de que salimos de
% la región crítica.
unlock (L) -> L ! {soltar}.
% Matamos al proceso locker. 
destroyLock (L) -> L ! {destroy}.

% implementamos semáforos con la misma función, dando más permitidos.
createSem (N) -> spawn(fun() -> locker(N, []) end).
destroySem (S) -> S ! {destroy}.
semP (S) -> 
    S ! {tomar, self()},
    receive
        permitido -> ok;
        _ -> semP(S)
    end.
semV (S) -> S ! {soltar}.
f (L, W) ->
    lock(L),
    % regioncritica(),
    io:format("uno ~p~n", [self()]),
    io:format("dos ~p~n", [self()]),
    io:format("tre ~p~n", [self()]),
    io:format("cua ~p~n", [self()]),
    unlock(L),
    W ! finished.

waiter (L, 0) -> destroyLock(L);
waiter (L, N) -> receive finished -> waiter(L, N-1) end.

waiter_sem (S, 0) -> destroySem(S);
waiter_sem (S, N) -> receive finished -> waiter_sem(S, N-1) end.

testLock () ->
    L = createLock(),
    W = spawn(fun () -> waiter(L, 3) end),
    spawn (fun () -> f(L, W) end),
    spawn (fun () -> f(L, W) end),
    spawn (fun () -> f(L, W) end),
    ok.

sem (S, W) ->
    semP(S),
    %regioncritica(), bueno, casi....
    io:format("uno ~p~n", [self()]),
    io:format("dos ~p~n", [self()]),
    io:format("tre ~p~n", [self()]),
    io:format("cua ~p~n", [self()]),
    io:format("cin ~p~n", [self()]),
    io:format("sei ~p~n", [self()]),
    semV(S),
    W ! finished.
testSem () ->
    S = createSem(2), % a lo sumo dos usando io al mismo tiempo
    W = spawn (fun () -> waiter_sem (S, 5) end),
    spawn (fun () -> sem (S, W) end),
    spawn (fun () -> sem (S, W) end),
    spawn (fun () -> sem (S, W) end),
    spawn (fun () -> sem (S, W) end),
    spawn (fun () -> sem (S, W) end),
    ok.

