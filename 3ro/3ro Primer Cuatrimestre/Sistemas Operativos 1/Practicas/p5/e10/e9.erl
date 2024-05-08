-module(e9).
-export([init/0, hello/0, hello_helper/1]).

hello() ->
    receive after 1000 -> ok end,
    io:fwrite("Hola ~p~n", [case rand:uniform(10) of 10 -> 1/uno; _ -> self() end]),
    hello().

hello_helper(Pid) ->
    receive
        {'EXIT', Pid, normal} -> ok;
        _ -> PidNuevo = spawn_link(fun () -> hello() end),
            hello_helper(PidNuevo)
    end.

init() -> 
    process_flag(trap_exit, true),
    Pid = spawn_link(fun () -> hello() end),
    hello_helper(Pid).
