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
  char *fName = "smithyEffect()";
  int newCards = 3;
  int discarded = 1;
  int seed = 1000;
  int numPlayer = 2;
  int handpos = 0;
  int player = 0;
  int k[10] = {adventurer, council_room, feast, gardens, mine
             , remodel, smithy, village, baron, great_hall};
  struct gameState G, testG;
  int assertPassed = 1;
  int returnVal;

  printf("\n\n* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * \n");
  printf ("Testing %s:\n", fName);
  printf("* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * \n");

  for (int test = 1; test <= 4; test++){ // loop through test cases

    memset(&G, 23, sizeof(struct gameState));     // clear the game state
    initializeGame(numPlayer, k, seed, &G);       // initialize a new game
    G.hand[player][handpos] = smithy;             // set smithy card to handPos

    // set up test cases
    switch(test){
      case 1: // test player 0
        player = 0;
        G.whoseTurn = 0;
        break;
      case 2: // test player 1
        player = 1;
        G.whoseTurn = 1;
        break;
      case 3: // exactly three cards in deck
        player = 0;
        G.deckCount[player] = 3;
        G.discardCount[player] = 5;
        break;
      case 4: // test when only 1 card available in deck
        player = 0;
        G.deckCount[player] = 1;
        G.deck[player][0] = sea_hag;
        G.discardCount[player] = 5;

        // load discard deck with minions for reshuffling :)
        for (int j = 0; j < 5; j++){
          G.discard[player][j] = minion;
        }
        break;
    }

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
    returnVal = cardEffect_smithy(player, &testG, handpos);

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
