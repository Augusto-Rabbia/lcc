% spawn(fun() -> hello:init() end)
-module(hello).
-export([init/0, hello/0]).

% Sacamos todo lo del ejercicio 9 porque no hacía falta
hello() ->
    receive after 1000 -> ok end,
    io:fwrite("Hola ~p~n", [self()]),
    ?MODULE:hello().

init() -> spawn(fun () -> ?MODULE:hello() end).
