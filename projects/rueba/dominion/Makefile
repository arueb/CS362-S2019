CFLAGS = -Wall -fpic -coverage -lm -std=c99

default: unittestresults

unittestresults: unittest1.c unittest2.c unittest3.c unittest4.c cardtest1.c cardtest2.c cardtest3.c cardtest4.c dominion.c rngs.c

	echo "Result for running unit tests:" > unittestresults.out
	echo "unittest1.c:" >> unittestresults.out
	gcc -o unittest1 unittest1.c dominion.c rngs.c $(CFLAGS)
	./unittest1 >> unittestresults.out
	gcov dominion.c >> unittestresults.out -b -f

	echo "unittest2.c:" >> unittestresults.out
	gcc -o unittest2 unittest2.c dominion.c rngs.c $(CFLAGS)
	./unittest2 >> unittestresults.out
	gcov dominion.c >> unittestresults.out -b -f

	echo "unittest3.c:" >> unittestresults.out
	gcc -o unittest3 unittest3.c dominion.c rngs.c $(CFLAGS)
	./unittest3 >> unittestresults.out
	gcov dominion.c >> unittestresults.out -b -f

	echo "unittest4.c:" >> unittestresults.out
	gcc -o unittest4 unittest4.c dominion.c rngs.c $(CFLAGS)
	./unittest4 >> unittestresults.out
	gcov dominion.c >> unittestresults.out -b -f

	echo "cardtest1.c:" >> unittestresults.out
	gcc -o cardtest1 cardtest1.c dominion.c rngs.c $(CFLAGS)
	./cardtest1 >> unittestresults.out
	gcov dominion.c >> unittestresults.out -b -f

	echo "cardtest2.c:" >> unittestresults.out
	gcc -o cardtest2 cardtest2.c dominion.c rngs.c $(CFLAGS)
	./cardtest2 >> unittestresults.out
	gcov dominion.c >> unittestresults.out -b -f

	echo "cardtest3.c:" >> unittestresults.out
	gcc -o cardtest3 cardtest3.c dominion.c rngs.c $(CFLAGS)
	./cardtest3 >> unittestresults.out
	gcov dominion.c >> unittestresults.out -b -f

	echo "cardtest4.c:" >> unittestresults.out
	gcc -o cardtest4 cardtest4.c dominion.c rngs.c $(CFLAGS)
	./cardtest4 >> unittestresults.out
	gcov dominion.c >> unittestresults.out -b -f

unittest1: unittest1.c dominion.c rngs.c

		echo "Result for running unit tests:" > unittest1.out
		echo "unittest1.c:" >> unittest1.out
		gcc -o unittest1 unittest1.c dominion.c rngs.c $(CFLAGS)
		./unittest1 >> unittest1.out
		gcov dominion.c >> unittest1.out -b -f

unittest2: unittest2.c dominion.c rngs.c

		echo "Result for running unit tests:" > unittest2.out
		echo "unittest2.c:" >> unittest2.out
		gcc -o unittest2 unittest2.c dominion.c rngs.c $(CFLAGS)
		./unittest2 >> unittest2.out
		gcov dominion.c >> unittest2.out -b -f

unittest3: unittest3.c dominion.c rngs.c

		echo "Result for running unit tests:" > unittest3.out
		echo "unittest3.c:" >> unittest3.out
		gcc -o unittest3 unittest3.c dominion.c rngs.c $(CFLAGS)
		./unittest3 >> unittest3.out
		gcov dominion.c >> unittest3.out -b -f

unittest4: unittest4.c dominion.c rngs.c

		echo "Result for running unit tests:" > unittest4.out
		echo "unittest4.c:" >> unittest4.out
		gcc -o unittest4 unittest4.c dominion.c rngs.c $(CFLAGS)
		./unittest4 >> unittest4.out
		gcov dominion.c >> unittest4.out -b -f

cardtest1: cardtest1.c dominion.c rngs.c

		echo "Result for running unit tests:" > cardtest1.out
		echo "cardtest1.c:" >> unittest4.out
		gcc -o cardtest1 cardtest1.c dominion.c rngs.c $(CFLAGS)
		./cardtest1 >> cardtest1.out
		gcov dominion.c >> cardtest1.out -b -f

cardtest2: cardtest2.c dominion.c rngs.c

		echo "Result for running unit tests:" > cardtest2.out
		echo "cardtest2.c:" >> unittest4.out
		gcc -o cardtest2 cardtest2.c dominion.c rngs.c $(CFLAGS)
		./cardtest2 >> cardtest2.out
		gcov dominion.c >> cardtest2.out -b -f

cardtest3: cardtest3.c dominion.c rngs.c

		echo "Result for running unit tests:" > cardtest3.out
		echo "cardtest3.c:" >> unittest4.out
		gcc -o cardtest3 cardtest3.c dominion.c rngs.c $(CFLAGS)
		./cardtest3 >> cardtest3.out
		gcov dominion.c >> cardtest3.out -b -f

cardtest4: cardtest4.c dominion.c rngs.c

		echo "Result for running unit tests:" > cardtest4.out
		echo "cardtest4.c:" >> unittest4.out
		gcc -o cardtest4 cardtest4.c dominion.c rngs.c $(CFLAGS)
		./cardtest4 >> cardtest4.out
		gcov dominion.c >> cardtest4.out -b -f

clean:
	rm unittestresults.out

cleanall:
	rm unittestresults.out unittest1 unittest2 unittest3 unittest4 cardtest2 cardtest2 cardtest3 cardtest4 *.gcda *.gcno *.gcov
