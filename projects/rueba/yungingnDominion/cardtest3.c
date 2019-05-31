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
#include "rngs.h"

typedef int bool;
#define TRUE  1
#define FALSE 0

int asserttrue(bool condition, int flag){
  if (!condition){
    printf("*** ASSERT FAILED ***\n");
    return 0;
  }
  return flag; // flag is fed/returned so that any failed test will cause an overall failure at end of test
}

int main() {
  char *fName = "smithyEffect() via cardEffect()";
  int seed = 1000;
  int numPlayer = 2;
  int player = 0;
  int k[10] = {adventurer, council_room, feast, gardens, mine
             , remodel, smithy, village, baron, great_hall};
  struct gameState G, testG;
  int returnVal;
  int assertPassed = 1;
  int handpos = 0, choice1 = 0, choice2 = 0, choice3 = 0, bonus = 0;
  int newCards = 3, discarded = 1;

  printf("\n\n* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * \n");
  printf ("Testing %s:\n", fName);
  printf("* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * \n");

  for (int test = 1; test <= 6; test++){ // loop through test cases

    memset(&G, 23, sizeof(struct gameState));     // clear the game state
    initializeGame(numPlayer, k, seed, &G);       // initialize a new game
    handpos = 0;

    // set up test cases
    switch(test){
      case 1: // initial gameState configuration from first hand position
        handpos = 0;
        break;
      case 2: // card played from last hand position (boundary)
        handpos = 4;
        break;
      case 3: // card played from illegal hand position (negative)
        handpos = -1;
        break;
      case 4: // additional smithy card exists in hand
        G.handCount[player] = 1;
        break;
      case 5: // handCount at max prior to draw
        G.handCount[player] = MAX_HAND;
        for (int i = 0; i < G.handCount[player]; i++){
          G.hand[player][i] = gold;
        }
      case 6: // additional smithy card exists in hand
        G.handCount[player] = 0;
        break;
    }

    G.hand[player][handpos] = smithy;       // set smithy card to handPos

    // copy player 0 game state counts to other players
    for (int p = 1; p < numPlayer; p++){
      G.handCount[p] = G.handCount[0];
      G.deckCount[p] = G.deckCount[0];
      G.discardCount[p] = G.discardCount[0];
      for (int t = 0; t < G.handCount[0]; t++) G.hand[p][t] = G.hand[0][t];
      for (int u = 0; u < G.deckCount[0]; u++) G.deck[p][u] = G.deck[0][u];
      for (int v = 0; v < G.discardCount[0]; v++) G.discard[p][v] = G.discard[0][v];
    }

    memcpy(&testG, &G, sizeof(struct gameState)); // copy the game state to a test case

    printf("\nRunning test %d for player %d... \n", test, player);

    // print hand before function call
    printf("\ninitial hand: \n");
    printHand(&G, player);

    // call function
    returnVal = cardEffect(smithy, choice1, choice2, choice3, &testG, handpos, &bonus);

    // print hand after function call
    printf("\nhand after %s called: \n", fName);
    printHand(&testG, player);

    // check if the handCount is as expected
    printf("handCount = %d, expected = %d\n", testG.handCount[player], G.handCount[player] + newCards - discarded);
    assertPassed = asserttrue(testG.handCount[player] == G.handCount[player] + newCards - discarded, assertPassed);

    // check if the deckCount is correct
    printf("deckCount = %d, expected = %d\n",
      testG.deckCount[player],
      (G.deckCount[player] < 3 ? G.deckCount[player] - newCards + G.discardCount[player]  : G.deckCount[player]  - newCards)
    );

    assertPassed = asserttrue(testG.deckCount[player] ==
      (G.deckCount[player] < 3 ? G.deckCount[player] - newCards + G.discardCount[player] : G.deckCount[player] - newCards),
      assertPassed
    );

    // check if the playedCardCount is correct
    printf("playedCardsCount = %d, expected = %d\n", testG.playedCardCount, G.playedCardCount+discarded);
    assertPassed = asserttrue(testG.playedCardCount == G.playedCardCount+discarded, assertPassed);

    // check if the state remains unaffected for other players
    for (int i = 0; i < numPlayer; i++){
      if (i != player){
        // check handCount
        printf("player %d handCount = %d, expected = %d\n", i, testG.handCount[i], G.handCount[i]);
        assertPassed = asserttrue(testG.handCount[i] == G.handCount[i], assertPassed);
        // check deckCount
        printf("player %d deckCount = %d, expected = %d\n", i, testG.deckCount[i], G.deckCount[i]);
        assertPassed = asserttrue(testG.deckCount[i] == G.deckCount[i], assertPassed);
        // check discardCount
        printf("player %d discardCount = %d, expected = %d\n", i, testG.discardCount[i], G.discardCount[i]);
        assertPassed = asserttrue(testG.discardCount[i] == G.discardCount[i], assertPassed);
      }
    }

    // check retun of cardEffect function
    printf("cardEffect return value = %d, expected = %d\n", returnVal, 0);
    assertPassed = asserttrue(returnVal == 0, assertPassed);


  } // close testing loop

  // determine overall pass/fail with flag
  printf("\n\n* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\n");
  printf("Overall %s result: ", fName);
  if (assertPassed){
    printf("All tests passed!\n");
  } else {
    printf("Some tests failed!\n");
  }
  printf("* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * \n\n");

  return 0;
}
