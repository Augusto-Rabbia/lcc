-module(cltexto).
-export([start_loop/1, start/1, loop/1, put/3, get/2, del/2, stats/1]).


start_loop(IP) ->
    {ok, Socket} = gen_tcp:connect(IP, 8888, [{active, true}]),
    loop(Socket).

loop(Socket) ->
    receive
        {put, K, V} ->  Message = io_lib:format("PUT ~s ~s\n", [K, V]),
                        gen_tcp:send(Socket, Message);
        {del, K} -> Message = io_lib:format("DEL ~s\n", [K]),
                    gen_tcp:send(Socket, Message);
        {get, K} -> Message = io_lib:format("GET ~s\n", [K]),
                    gen_tcp:send(Socket, Message);
        {stats} -> Message = "STATS\n",
                   gen_tcp:send(Socket, Message)
    end,
    receive
        {tcp, _, Paquete} -> io:fwrite("~s\n", [Paquete])
    end,
    loop(Socket).

start(IP) ->
    Pid = spawn(fun () -> start_loop(IP) end),
    Pid.

put(ID, K, V) ->
    ID ! {put, K, V},
    ok.

get(ID, K) ->
    ID ! {get, K},
    ok.

del(ID, K) ->
    ID ! {del, K},
    ok.

stats(ID) ->
    ID ! {stats},
    ok.
