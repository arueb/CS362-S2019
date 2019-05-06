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
  char *fName = "treasure_map via cardEffect()";
  int seed = 1000;
  int numPlayer = 4;
  int player = 0;
  int k[10] = {adventurer, council_room, feast, gardens, mine
             , remodel, smithy, village, baron, great_hall};
  struct gameState G, testG;
  int returnVal;
  int assertPassed = 1;
  int hasTreasureMap= 0;
  int handpos = 0, choice1 = 0, choice2 = 0, choice3 = 0, bonus = 0;

  printf("\n\n* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * \n");
  printf ("Testing %s:\n", fName);
  printf("* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * \n");

  for (int test = 1; test <= 4; test++){ // loop through test cases

    memset(&G, 23, sizeof(struct gameState));     // clear the game state
    initializeGame(numPlayer, k, seed, &G);       // initialize a new game
    G.hand[player][handpos] = treasure_map;       // set treasure_map card to handPos


    // set up test cases
    switch(test){
      case 1: // player 0 has 1 additional treasure map
        // test to make sure two cards are trashed and gold coins gained
        hasTreasureMap = TRUE;
        G.hand[player][3] = treasure_map;
        break;

      case 2: // player 0 has 2 additional treasure maps
        // test to make sure only two cards are trashed and gold coins gained
        hasTreasureMap = TRUE;
        G.hand[player][2] = treasure_map;
        G.hand[player][3] = treasure_map;
        break;

      case 3: // player 1 has 0 additional treasure maps
        // test to make sure only one card is trashed and no gold coins gained
        player = 1;
        G.whoseTurn = 1;
        hasTreasureMap = FALSE;
        break;

      case 4: // player 1 has 0 additional treasure maps and only one card in hand
        // test to make sure only one card is trashed and no gold coins gained
        player = 0;
        G.whoseTurn = 0;
        hasTreasureMap = FALSE;
        G.handCount[player] = 1;
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
    printHand(&testG, player);

    // call function
    returnVal = cardEffect(treasure_map, choice1, choice2, choice3, &testG, handpos, &bonus);

    // print hand after function call
    printf("\nhand after %s called: \n", fName);
    printHand(&testG, player);

    // check if the handCount is as expected (should reduce by 1 or 2 cards (if additional treasure card present)
    printf("handCount = %d, expected = %d\n",
            testG.handCount[player],
            hasTreasureMap ? G.handCount[player] - 2 : G.handCount[player] - 1 // trash 1 or 2 cards (if another treasure_map)
          );
    assertPassed = asserttrue(testG.handCount[player] ==
      (hasTreasureMap ? G.handCount[player] - 2 : G.handCount[player] - 1), assertPassed);

    // check if the deckCount is as expected (should increase by 0 or 4 (if additional trash card is present))
    printf("deckCount = %d, expected = %d\n", testG.deckCount[player], G.deckCount[player] + (hasTreasureMap ? 4 : 0));
    assertPassed = asserttrue(testG.deckCount[player] == G.deckCount[player]+ (hasTreasureMap ? 4 : 0), assertPassed);

    // check if the discardCount is correct (should not change since card(s) are trashed)
    printf("discardCount = %d, expected = %d\n", testG.discardCount[player], G.discardCount[player]);
    assertPassed = asserttrue(testG.discardCount[player] == G.discardCount[player], assertPassed);

    // check if the playedCardCount is correct (should not change since card(s) are trashed)
    printf("playedCardsCount = %d, expected = %d\n", testG.playedCardCount, G.playedCardCount);
    assertPassed = asserttrue(testG.playedCardCount == G.playedCardCount, assertPassed);

    // check if the gold coin supply decreased by 4 if two treasure cards are trashed or 0 if only 1 treause card is trashed
    printf("gold supply = %d, expected = %d\n", testG.supplyCount[gold], G.supplyCount[gold] - (hasTreasureMap ? 4 : 0));
    assertPassed = asserttrue(testG.supplyCount[gold] == G.supplyCount[gold] - (hasTreasureMap ? 4 : 0), assertPassed);

    // check retun of cardEffect function
    printf("cardEffect() return value = %d, expected = %d\n", returnVal, (hasTreasureMap ? 1 : -1));
    assertPassed = asserttrue(returnVal == (hasTreasureMap ? 1 : -1), assertPassed);

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
