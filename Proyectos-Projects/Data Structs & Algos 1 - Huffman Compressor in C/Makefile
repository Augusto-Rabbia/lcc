huff: main.o compresor.o descompresor.o io.o huff_btree_slist.o
	gcc -Werror -g -Wall -Wextra -std=c99 main.o huff_btree_slist.o descompresor.o compresor.o io.o -o huff

tests: tests.o compresor.o descompresor.o io.o huff_btree_slist.o
	gcc -Werror -g -Wall -Wextra -std=c99 tests.o huff_btree_slist.o descompresor.o compresor.o io.o -o tests

tests.o: tests.c io.h compresor.h descompresor.h
	gcc -c -Werror -g -Wall -Wextra -std=c99 tests.c

main.o: main.c io.h compresor.h descompresor.h
	gcc -c -Werror -g -Wall -Wextra -std=c99 main.c

compresor.o: compresor.c huff_btree_slist.o compresor.h io.h
	gcc -c -Werror -g -Wall -Wextra -std=c99 compresor.c

descompresor.o: descompresor.c huff_btree_slist.o descompresor.h io.h
	gcc -c -Werror -g -Wall -Wextra -std=c99 descompresor.c

io.o: io.c io.h
	gcc -c -Werror -g -Wall -Wextra -std=c99 io.c

huff_btree_slist.o: huff_btree_slist.c huff_btree_slist.h
	gcc -c -Werror -g -Wall -Wextra -std=c99 huff_btree_slist.c

clean:
	rm *.o
