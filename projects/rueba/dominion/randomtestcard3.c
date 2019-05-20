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
  state->whoseTurn = rand() % numPlayer; // choose random player's turn
  for (int i = 0; i < numPlayer; i++){ // loop throuh players
    // assign random counts
    state->deckCount[i] = rand() % (50) + 1;
    state->handCount[i] = rand() % (50) + 1;
    state->discardCount[i] = rand() % (50);
  }
  return 1;
}

int assignRandomCards(int array[], int length){
  for (int i = 0; i < length; i++){
    array[i] = rand() % (treasure_map + 1); // assign random card
  }
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
  char *fName = "councilRoomEffect()";
  srand(time(NULL));
  int newCards = 4;
  int discarded = 1;
  int buys = 1;
  int seed = 1000;
  int numPlayer = 4;
  int handpos = 0;
  int player = 0;
  int k[10] = {adventurer, council_room, feast, gardens, mine
             , remodel, smithy, village, baron, great_hall};
  struct gameState G, testG;
  int returnVal;
  int handCountFails = 0;
  int deckCountFails = 0;
  int discardCountFails = 0;
  int otherHandCountFails = 0;
  int otherDeckCountFails = 0;
  int otherDiscardCountFails = 0;
  int returnValFails = 0;
  int totalFails = 0;
  int buyFails = 0;
  int expectedDeckCount = 0;
  int expectedHandCount = 0;
  clock_t startTime, endTime;
  double secondsToRun;

  startTime = clock(); //start the timer  

  printf("\n\n=============================================================== \n");
  printf ("Testing %s:\n", fName);
  printf("=============================================================== \n");

  for (int test = 1; test <= 2500; test++){ // loop through test cases

    // set up new test case and randomize inputs
    memset(&G, 23, sizeof(struct gameState));     // clear the game state
    initializeGame(numPlayer, k, seed, &G);       // initialize a new game
    ramdomGameState(&G, numPlayer);
    player = G.whoseTurn;
    assignRandomCards(G.deck[player], G.deckCount[player]);
    assignRandomCards(G.discard[player], G.discardCount[player]);
    assignRandomCards(G.hand[player], G.handCount[player]);
    handpos = rand() % G.handCount[player];

    G.hand[player][handpos] = council_room; // set council_room card to handPos
    memcpy(&testG, &G, sizeof(struct gameState)); // copy the game state to a test case

    printf("\nRunning test %d... \n", test);
    printf("Test conditions: player: %d, deckCount: %d, handCount %d, discardCount %d, handpos: %d\n",
      G.whoseTurn,
      G.deckCount[G.whoseTurn],
      G.handCount[G.whoseTurn],
      G.discardCount[G.whoseTurn],
      handpos
    );

    // call function
    returnVal = councilRoomEffect(player, &testG, handpos);

    // calculate expected handCount
    if (newCards <= (G.deckCount[player] + G.discardCount[player])){
      expectedHandCount = G.handCount[player] + newCards - discarded;
    } else {
      expectedHandCount = G.handCount[player] + G.deckCount[player] + G.discardCount[player] - discarded;
    }

    // check if handCount is as expectedDeckCount
    assertTrue(
      testG.handCount[player],
      expectedHandCount,
      "handCount",
      &handCountFails,
      &totalFails
    );

    // calculate expected deckCount
    expectedDeckCount = G.deckCount[player] - newCards;
    if (expectedDeckCount < 0){
      expectedDeckCount += G.discardCount[player]; // shuffle discard pile into new deck
    }
    if (expectedDeckCount < 0 ){
      expectedDeckCount = 0;
    }

    // check if the deckCount is as expected
    assertTrue(
      testG.deckCount[player],
      expectedDeckCount,
      "deckCount",
      &deckCountFails,
      &totalFails
    );

    // check if the discardCount is as expected
    assertTrue(
      testG.discardCount[player],
      newCards > G.deckCount[player] ? 0 : G.discardCount[player],
      "discardCount",
      &discardCountFails,
      &totalFails
    );

    // check if numBuys is as expected
    assertTrue(
      testG.numBuys,
      G.numBuys + buys,
      "buys",
      &buyFails,
      &totalFails
    );

    // check if the state counts are as expected other players
    for (int i = 0; i < numPlayer; i++){
      if (i != player){
        // check handCount is increased
        assertTrue(
          testG.handCount[i],
          G.handCount[i] + 1,
          "other player's handCount",
          &otherHandCountFails,
          &totalFails
        );

        // check deckCount is decreased (or reshuffled)
        assertTrue(
          testG.deckCount[i],
          G.deckCount[i] <  1 ? G.discardCount[i] : G.deckCount[i] - 1,
          "other player's deckCount",
          &otherDeckCountFails,
          &totalFails
        );

        // check discardCount is not changed
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

  } // close testing loop

  // stop the timer
  endTime = clock();
  secondsToRun = ((double) (endTime - startTime)) / CLOCKS_PER_SEC;

  // print overall testing results
  printf("\n\n=================== RANDOM TESTING RESULTS ===================\n");
  printf("Function tested: %s \n", fName);
  printf("handCount tests failed: %d\n", handCountFails);
  printf("deckCount tests failed: %d\n", deckCountFails);
  printf("discardCount tests failed: %d\n", discardCountFails);
  printf("buy tests failed: %d\n", buyFails);
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
