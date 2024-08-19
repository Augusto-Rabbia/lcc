-module(clbinario).
-export([start/1, start_loop/1, loop/1, put/3, get/2, del/2, stats/1]).

start_loop(IP) ->
    {ok, Socket} = gen_tcp:connect(IP, 8889, [{active, true}]),
    loop(Socket).

loop(Socket) ->
    receive
        {put, LenKBin, K, LenVBin, V} -> Message = <<11, LenKBin/binary, K/binary, LenVBin/binary, V/binary>>,
                                         io:fwrite("~p\n", [Message]),
                                         gen_tcp:send(Socket, Message);
        {del, LenKBin, K} -> Message = <<12, LenKBin/binary, K/binary>>,
                             gen_tcp:send(Socket, Message);
        {get, LenKBin, K} -> Message = <<13, LenKBin/binary, K/binary>>,
                             gen_tcp:send(Socket, Message);
        {stats} -> Message = <<21>>,
                   gen_tcp:send(Socket, Message)
    end,
    receive
        {tcp, _, Paquete} -> io:fwrite("~p\n", [Paquete])
    end,
    loop(Socket).

start(IP) ->
    Pid = spawn(fun () -> start_loop(IP) end),
    Pid.

%recibe la Key y Value en formato big endian.
put(ID, K, V) ->
    LenK = byte_size(K),
    LenKBin = <<LenK:32>>,
    LenV = byte_size(V),
    LenVBin = <<LenV:32>>,

    ID ! {put, LenKBin, K, LenVBin, V},
    ok.

get(ID, K) -> %13
    LenK = byte_size(K),
    LenKBin = <<LenK:32>>,

    ID ! {get, LenKBin, K},
    ok.

del(ID, K) -> %12
    LenK = byte_size(K),
    LenKBin = <<LenK:32>>,

    ID ! {del, LenKBin, K},
    ok.

stats(ID) -> %21
    ID ! {stats},
    ok.



