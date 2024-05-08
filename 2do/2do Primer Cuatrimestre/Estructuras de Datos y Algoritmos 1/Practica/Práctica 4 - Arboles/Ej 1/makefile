programa: test.o btree.o
	gcc -Werror -Wall -Wextra test.o btree.o -o programa

test.o: test.c btree.h
	gcc -Werror -Wall -Wextra -c test.c

btree.o: btree.c btree.h
	gcc -Werror -Wall -Wextra -c btree.c

clean:
	rm *.o
