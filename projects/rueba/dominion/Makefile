CFLAGS = -Wall -fpic -coverage -lm -std=c99

default: randomtestresults

randomtestresults: randomtestcard1.c randomtestcard2.c randomtestcard3.c dominion.c rngs.c

	echo "Randomized test results:" > randomtestresults.out

	echo "randomtestcard1.c:" >> randomtestresults.out
	gcc -o randomtestcard1 randomtestcard1.c dominion.c rngs.c $(CFLAGS)
	./randomtestcard1 >> randomtestresults.out
	gcov dominion.c >> randomtestresults.out -b -f

	echo "randomtestcard2.c:" >> randomtestresults.out
	gcc -o randomtestcard2 randomtestcard2.c dominion.c rngs.c $(CFLAGS)
	./randomtestcard2 >> randomtestresults.out
	gcov dominion.c >> randomtestresults.out -b -f

	echo "randomtestcard3.c:" >> randomtestresults.out
	gcc -o randomtestcard3 randomtestcard3.c dominion.c rngs.c $(CFLAGS)
	./randomtestcard3 >> randomtestresults.out
	gcov dominion.c >> randomtestresults.out -b -f

randomtestcard1: randomtestcard1.c dominion.c rngs.c

		echo "Result for running random tests:" > randomtestcard1.out
		echo "randomtestcard1.c:" >> randomtestcard1.out
		gcc -o randomtestcard1 randomtestcard1.c dominion.c rngs.c $(CFLAGS)
		./randomtestcard1 >> randomtestcard1.out
		gcov dominion.c >> randomtestcard1.out -b -f

randomtestcard2: randomtestcard2.c dominion.c rngs.c

		echo "Result for running random tests:" > randomtestcard2.out
		echo "randomtestcard2.c:" >> randomtestcard2.out
		gcc -o randomtestcard2 randomtestcard2.c dominion.c rngs.c $(CFLAGS)
		./randomtestcard2 >> randomtestcard2.out
		gcov dominion.c >> randomtestcard2.out -b -f

randomtestcard3: randomtestcard3.c dominion.c rngs.c

		echo "Result for running random tests:" > randomtestcard3.out
		echo "randomtestcard3.c:" >> randomtestcard3.out
		gcc -o randomtestcard3 randomtestcard3.c dominion.c rngs.c $(CFLAGS)
		./randomtestcard3 >> randomtestcard3.out
		gcov dominion.c >> randomtestcard3.out -b -f


clean:
		rm randomtestcard1.out randomtestcard2.out andomtestcard3.out

cleanall:
		rm randomtestcard1.out randomtestcard2.out andomtestcard3.out randomtestcard1 randomtestcard2 andomtestcard3 *.gcda *.gcno
