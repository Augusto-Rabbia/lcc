-module(cltest).
-export([start/1, send/1, starttext/1, sendtext/1, senddel/1, startdel/1, sendget/1, startget/1]).

send(M) ->
    N = integer_to_binary(M),
    {ok, Socket} = gen_tcp:connect(localhost, 8889, [{active, true}]),
    Str = "TEST_STRING",
    FinalStr = list_to_binary(string:join(lists:duplicate(1, Str), "")), 
    Nlen = byte_size(N),
    Len1 = <<Nlen:32>>,
    FinalStrlen = byte_size(FinalStr),
    Len2 = <<FinalStrlen:32>>,
    Message = <<11, Len1/binary, N/binary, Len2/binary, FinalStr/binary>>,
    ManyMessage = binary_to_list(Message),
    ManyStr = list_to_binary(string:join(lists:duplicate(2, ManyMessage), "")),
    gen_tcp:send(Socket, ManyStr),
    receive
        {tcp, _, Paquete} -> io:fwrite("~p, ~p\n", [Paquete, M])
    end,
    timer:sleep(10000), %sleep de 10s
    ok.

start(1) ->
    spawn( fun() -> send(1) end ),
    ok;
start(N) ->
    spawn( fun() -> send(N) end ),
    start(N-1),
    ok.

senddel(M) ->
    N = integer_to_binary(M),
    {ok, Socket} = gen_tcp:connect(localhost, 8889, [{active, true}]),
    Nlen = byte_size(N),
    Len1 = <<Nlen:32>>,
    Message = <<12, Len1/binary, N/binary>>,
    gen_tcp:send(Socket, Message),
    receive
        {tcp, _, Paquete} -> io:fwrite("~p, ~p\n", [Paquete, M])
    end,
    timer:sleep(10000),
    ok.

startdel(1) ->
    spawn( fun() -> senddel(1) end ),
    ok;
startdel(N) ->
    spawn( fun() -> senddel(N) end ),
    startdel(N-1),
    ok.

sendget(M) ->
    N = integer_to_binary(M),
    {ok, Socket} = gen_tcp:connect(localhost, 8889, [{active, true}]),
    Nlen = byte_size(N),
    Len1 = <<Nlen:32>>,
    Message = <<13, Len1/binary, N/binary>>,
    gen_tcp:send(Socket, Message),
    receive
        {tcp, _, Paquete} -> io:fwrite("~p, ~p\n", [Paquete, M])
    end,
    timer:sleep(20000),
    ok.

startget(1) ->
    spawn( fun() -> sendget(1) end ),
    ok;
startget(N) ->
    spawn( fun() -> sendget(N) end ),
    startget(N-1),
    ok.

%% TEST de TEXTO
sendtext(M) ->
    {ok, Socket} = gen_tcp:connect(localhost, 8888, [{active, true}]),
    Message = io_lib:format("GET ~p\n", [M]),
    gen_tcp:send(Socket, Message),

    io:fwrite("~s\n", [Message]),
    receive
        {tcp, _, Paquete} -> io:fwrite("~s, ~p\n", [Paquete, M])
    end,
    timer:sleep(7*1000).

starttext(1) ->
    spawn( fun() -> sendtext(1) end ),
    ok;
starttext(N) ->
    spawn( fun() -> sendtext(N) end ),
    starttext(N-1),
    ok.
