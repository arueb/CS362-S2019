/******************************************************************************
 ** Author: Alex Rueb
 ** Date: 5/5/2019
 ** Description: Unit Test of Dominion codebase for Assignment 3
 ******************************************************************************/

#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <time.h>
#include "rngs.h"

typedef int bool;
#define TRUE 1
#define FALSE 0

int ramdomGameState(struct gameState *state, int numPlayer){
  state->whoseTurn = rand() % numPlayer;
  state->deckCount[state->whoseTurn] = rand() % 50 + 1;
  state->handCount[state->whoseTurn] = rand() % 50 + 1;
  state->discardCount[state->whoseTurn] = rand() % 50 + 1;
  state->playedCardCount = rand() % 50 + 1;
  return 1;
}

int assertTrue(int result, int expected, char *testProp, int *propFails, int* totalFails){
  if (result != expected){
    printf("* * * ASSERT FAILED * * *\n");
    printf("%s: %d, expected: %d\n",testProp, result, expected);
    (*propFails)++;
    (*totalFails)++;
    return 0;
  }
  return 1;
}

int main() {
  char *fName = "smithyEffect()";
  srand(time(NULL));
  int newCards = 3;
  int discarded = 1;
  int seed = 1000;
  int numPlayer = 4;
  int handpos = 0;
  int player = 0;
  int k[10] = {adventurer, council_room, feast, gardens, mine
             , remodel, smithy, village, baron, great_hall};
  struct gameState G, testG;
  int returnVal;
  int playedCardCountFails = 0;
  int handCountFails = 0;
  int deckCountFails = 0;
  int otherHandCountFails = 0;
  int otherDeckCountFails = 0;
  int otherDiscardCountFails = 0;
  int returnValFails = 0;
  int totalFails = 0;
  int tempFails;
  clock_t startTime, endTime;
  double secondsToRun;

  startTime = clock(); //start the timer

  printf("\n\n=============================================================== \n");
  printf ("Testing %s:\n", fName);
  printf("=============================================================== \n");

  for (int test = 1; test <= 2500; test++){ // loop through test cases

    // set up test case and randomize inputs
    memset(&G, 23, sizeof(struct gameState));     // clear the game state
    initializeGame(numPlayer, k, seed, &G);       // initialize a new game
    ramdomGameState(&G, numPlayer);               // randomize game state counts
    player = G.whoseTurn;
    handpos = rand() % G.handCount[player];       // choose random hand position
    G.hand[player][handpos] = smithy;             // set smithy card to handPos

    memcpy(&testG, &G, sizeof(struct gameState)); // copy the game state to a test case

    tempFails = totalFails; // store number of total failures before tests in loop
    printf("\nRunning test %d... \n", test);

    // call function
    returnVal = smithyEffect(player, &testG, handpos);

    // check if the handCount is as expected
    assertTrue(
      testG.handCount[player],
      G.handCount[player] + newCards - discarded,
      "handCount",
      &handCountFails,
      &totalFails
    );

    // check if the deckCount is correct
    assertTrue(
      testG.deckCount[player],
      (G.deckCount[player] < 3 ? G.deckCount[player] - newCards + G.discardCount[player] : G.deckCount[player] - newCards),
      "deckCount",
      &deckCountFails,
      &totalFails
    );

    // check if the playedCardCount is correct
    assertTrue(
      testG.playedCardCount,
      G.playedCardCount+discarded,
      "playedCardCount",
      &playedCardCountFails,
      &totalFails
    );

    // check if the state counts remains unaffected for other players
    for (int i = 0; i < numPlayer; i++){
      if (i != player){
        // check handCount
        assertTrue(
          testG.handCount[i],
          G.handCount[i],
          "other player's handCount",
          &otherHandCountFails,
          &totalFails
        );

        // check deckCount
        assertTrue(
          testG.deckCount[i],
          G.deckCount[i],
          "other player's deckCount",
          &otherDeckCountFails,
          &totalFails
        );

        // check discardCount
        assertTrue(
          testG.discardCount[i],
          G.discardCount[i],
          "other player's discardCount",
          &otherDiscardCountFails,
          &totalFails
        );
      }
    }

    // check retun of cardEffect function
    assertTrue(
      returnVal,
      0,
      "returnVal",
      &returnValFails,
      &totalFails
    );

    // if one test failed in this iteration of the loop, print error conditions
    if (totalFails > tempFails){
      printf("Failure conditions: player: %d, deckCount: %d, handCount %d, discardCount %d, handpos: %d\n",
        G.whoseTurn,
        G.deckCount[G.whoseTurn],
        G.handCount[G.whoseTurn],
        G.discardCount[G.whoseTurn],
        handpos
      );
    }
  } // close testing loop

  // stop the timer
  endTime = clock();
  secondsToRun = ((double) (endTime - startTime)) / CLOCKS_PER_SEC;

  // print overall testing results
  printf("\n\n=================== RANDOM TESTING RESULTS ===================\n");
  printf("Function tested: %s \n", fName);
  printf("deckCount tests failed: %d\n", deckCountFails);
  printf("handCount tests failed: %d\n", handCountFails);
  printf("playedCardCount tests failed: %d\n", playedCardCountFails);
  printf("otherDiscardCount tests failed: %d\n", otherDiscardCountFails);
  printf("otherHandCount tests failed: %d\n", otherHandCountFails);
  printf("otherDeckCount tests failed: %d\n", otherDeckCountFails);
  printf("returnVal tests failed: %d\n", returnValFails);
  printf("Execution time: %f seconds\n\n", secondsToRun);
  
  if (totalFails == 0){
    printf("All tests passed!\n");
  } else {
    printf("Some tests failed!\n");
  }
  printf("=============================================================== \n\n");
  return 0;
}
