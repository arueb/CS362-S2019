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
  return flag; // flag is fed in so that any failed test will cause an overall failure
}

int main() {
  char *fName = "adventurerEffect()";
  int seed = 1000;
  int numPlayer = 2;
  int handpos = 0;
  int player = 0;
  int k[10] = {adventurer, council_room, feast, gardens, mine
             , remodel, smithy, village, baron, great_hall};
  struct gameState G, testG;
  int treasureCardsInDeck;
  int assertPassed = 1;
  int returnVal;

  printf("\n\n* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * \n");
  printf ("Testing %s:\n", fName);
  printf("* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * \n");

  for (int test = 1; test <= 4; test++){

    memset(&G, 23, sizeof(struct gameState));     // clear the game state
    initializeGame(numPlayer, k, seed, &G);       // initialize a new game
    G.hand[player][handpos] = adventurer;         // set adventurer card to handPos

    switch(test){
      case 1: // more than two treasure cards remaining in deck
        treasureCardsInDeck = countTreasureCards(G.deck[player], G.deckCount[player]);
        break;
      case 2: // two treasure cards remaining in deck
        G.deck[player][0] = gold;
  			G.deck[player][1] = smithy;
  			G.deck[player][2] = duchy;
  			G.deck[player][3] = estate;
  			G.deck[player][4] = silver;
        treasureCardsInDeck = 2;
        break;
      case 3: // one treasure card remaining in deck
        G.deck[player][0] = feast;
  			G.deck[player][1] = smithy;
  			G.deck[player][2] = silver;
  			G.deck[player][3] = estate;
  			G.deck[player][4] = steward;
        treasureCardsInDeck = 1;
        break;
      case 4: // zero treasure cards remaining in deck
        G.deck[player][0] = feast;
  			G.deck[player][1] = smithy;
  			G.deck[player][2] = great_hall;
  			G.deck[player][3] = estate;
  			G.deck[player][4] = steward;
        treasureCardsInDeck = 0;
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
    returnVal = adventurerEffect(player, &testG, handpos);

    // print hand after function ccall
    printf("\nhand after %s called: \n", fName);
    printHand(&testG, player);

    // check if number of treasure cards is as expected
    printf("\ntreasure cards = %d, expected = %d\n",
      countTreasureCards(testG.hand[player], testG.handCount[player]),
      (countTreasureCards(G.hand[player], G.handCount[player]) + (treasureCardsInDeck < 2 ? treasureCardsInDeck : 2 ))
      );

    assertPassed = asserttrue(countTreasureCards(testG.hand[player], testG.handCount[player]) ==
      countTreasureCards(G.hand[player], G.handCount[player])
      + (treasureCardsInDeck < 2 ? treasureCardsInDeck : 2 ),
      assertPassed
      );
    // check if discard count is as expected
    printf("discarded cards = %d, expected = %d\n",
      testG.discardCount[player],
      G.discardCount[player]
      + G.deckCount[player]
      - testG.deckCount[player]
      - (treasureCardsInDeck < 2 ? treasureCardsInDeck : 2)
      );

    assertPassed = asserttrue(testG.discardCount[player] ==
      G.discardCount[player]
      + G.deckCount[player]
      - testG.deckCount[player]
      - (treasureCardsInDeck < 2 ? treasureCardsInDeck : 2),
      assertPassed
      );

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

  } // close loop

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
