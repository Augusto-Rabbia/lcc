-module(turnos).
-export([server/0, contador/1, get_request/2, getnum/0, wait_connect/2]).
-export([limpiar/1, responder/2]).


% Numero de turno global
contador(N) ->
    receive
        {num, Pid} -> Pid ! N,
                    contador(N+1);
        _ -> contador(N)
    end.

getnum() ->
    servidor ! {num, self()},
    receive
        N -> N
    end.

server() ->
    {ok, ListenSocket} = gen_tcp:listen(8000, [{reuseaddr, true}]),
    spawn (fun () -> wait_connect(ListenSocket, 0) end),
    register(servidor, self()),
    contador(0).

wait_connect(ListenSocket, N) ->
    {ok, Socket} = gen_tcp:accept(ListenSocket),
    spawn (fun () -> wait_connect (ListenSocket, N+1) end),
    get_request(Socket, []).

% Remueve entradas no válidas del buzón, es decir, hasta un \n
% que no sea ni NUEVO\n ni CHAU\n
limpiar([$\n |TL]) -> 
    TL;
limpiar([_|TL]) -> limpiar(TL).

% Decide si el comando es válido y responde en consecuencia
responder(Socket, "NUEVO\n" ++ TL) ->
    Num = getnum(),
    Reply = io_lib:format("~p\n", [Num]),
    gen_tcp:send(Socket, Reply),
    responder(Socket, TL);

responder(Socket, "CHAU\n" ++ _) ->
    io:fwrite("Cliente ~p se desconectó", [Socket]),
    gen_tcp:close(Socket);
    
responder(Socket, "") ->
    get_request(Socket, "");

responder(Socket, Buzon) -> 
    case string:str(Buzon, string:chr(10)) > 0 of
        true -> NewBuzon = limpiar(Buzon),
                responder(Socket, NewBuzon);
        false -> get_request(Socket, Buzon)
    end.

get_request(Socket, Buzon) ->
    io:fwrite("Esperando mensajes de ~p~n", [Socket]),
    receive
        {tcp, Socket, X} -> MSG = Buzon ++ X,
                            responder(Socket, MSG);
        _ -> get_request(Socket, Buzon)
    end.
