FLAGS=-Werror -g -Wall -Wextra -std=c99

spell-checker: main.o corrector.o palabra.o trie.o tablaHash.o slist.o
	gcc $(FLAGS) main.o corrector.o palabra.o trie.o tablaHash.o slist.o -o spell-checker

test: tests/tests.o corrector.o palabra.o trie.o tablaHash.o slist.o
	gcc $(FLAGS) tests/tests.o corrector.o palabra.o trie.o tablaHash.o slist.o -o tests/s-c-Test

tests.o: tests/tests.c corrector.h palabra.h trie.h tablaHash.h slist.h
	gcc -c $(FLAGS) tests/tests.c

main.o: main.c corrector.h palabra.h trie.h tablaHash.h slist.h
	gcc -c $(FLAGS) main.c

corrector.o: corrector.c palabra.h trie.h tablaHash.h slist.h
	gcc -c $(FLAGS) corrector.c

palabra.o: palabra.c palabra.h
	gcc -c $(FLAGS) palabra.c

trie.o: trie.c trie.h
	gcc -c $(FLAGS) trie.c

tablaHash.o: tablaHash.c tablaHash.h slist.h
	gcc -c $(FLAGS) tablaHash.c

slist.o: slist.c slist.h
	gcc -c $(FLAGS) slist.c

clean:
	rm -rf *.o
	rm -rf tests/tests.o
