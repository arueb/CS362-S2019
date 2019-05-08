CFLAGS = -Wall -fpic -coverage -lm -std=c99

default: testme

testme: testme.c

	echo "Output for random input testing of testme.c:" >> testme.out
	gcc -o testme testme.c $(CFLAGS)
	./testme >> testme.out

clean:
	rm testme.out

cleanall:
	rm testme.out testme *.gcda *.gcno *.gcov
