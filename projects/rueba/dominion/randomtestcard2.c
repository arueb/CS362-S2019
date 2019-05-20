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
  state->deckCount[state->whoseTurn] = rand() % (50) + 1;
  state->handCount[state->whoseTurn] = rand() % (50) + 1;
  state->discardCount[state->whoseTurn] = rand() % (50);
  return 1;
}

int assignRandomCards(int array[], int length){
  for (int i = 0; i < length; i++){
    array[i] = rand() % (treasure_map + 1); // assign random card
  }
  return 1;
}

int assignTreasureCards(int array[], int length){
  for (int i = 0; i < length; i++){
    array[i] = rand() % (3) + 4; // assign random treasure card
  }
  return 1;
}

int countTreasureCards(int array[], int length){
  int count = 0;
  for (int i = 0; i < length; i++){
    if (array[i] == copper || array[i] == silver || array[i] == gold){
      count++;
    }
  }
  return count;
}

int drawsToTreasures(int array[], int length, int numTreasures){
  int count = 0;
  int drawn = 0;

  for (int i = length - 1; i >= 0; i--){
    drawn++;
    if (array[i] == copper || array[i] == silver || array[i] == gold){
      count++;
      if (count == numTreasures)
        return drawn;
    }
  }
  return drawn;
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
  char *fName = "adventurerEffect()";
  srand(time(NULL));
  int discarded = 1;
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
  int treasureCardsInDeck = 0;
  int treasureCardsinHandFails = 0;
  int returnValFails = 0;
  int totalFails = 0;
  int expectedDeckCount = 0;
  int expectedDiscardCount = 0;
  int totalTreasuresToDraw = 0;
  int treasureCardsInDiscard = 0;
  clock_t startTime, endTime;
  double secondsToRun;

  startTime = clock(); //start the timer

  printf("\n\n=============================================================== \n");
  printf ("Testing %s:\n", fName);
  printf("=============================================================== \n");

  for (int test = 1; test <= 2500; test++){ // loop through test cases

    // set up new test and randomize inputs
    memset(&G, 23, sizeof(struct gameState));     // clear the game state
    initializeGame(numPlayer, k, seed, &G);       // initialize a new game
    ramdomGameState(&G, numPlayer);
    player = G.whoseTurn;
    assignRandomCards(G.deck[player], G.deckCount[player]);
    assignRandomCards(G.hand[player], G.handCount[player]);
    assignTreasureCards(G.discard[player], G.discardCount[player]);
    handpos = rand() % G.handCount[player];

    G.hand[player][handpos] = adventurer; // set adventurer card to handPos

    treasureCardsInDeck = countTreasureCards(G.deck[player], G.deckCount[player]);
    treasureCardsInDiscard = countTreasureCards(G.discard[player], G.discardCount[player]);
    totalTreasuresToDraw = treasureCardsInDeck+ treasureCardsInDiscard;
    if (totalTreasuresToDraw > 2) totalTreasuresToDraw = 2;


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
    returnVal = adventurerEffect(player, &testG, handpos);

    // check if number of treasure cards is as expected
    assertTrue(
      countTreasureCards(testG.hand[player], testG.handCount[player]), // treaure cards in test hand
      countTreasureCards(G.hand[player], G.handCount[player]) + totalTreasuresToDraw,
      "treasureCardsInHand",
      &treasureCardsinHandFails,
      &totalFails
      );

    // calculate expected deck count
    if (treasureCardsInDeck==0){
      expectedDeckCount = G.discardCount[player] - drawsToTreasures(G.discard[player], G.discardCount[player], 2);
    } else if (treasureCardsInDeck == 1){
      expectedDeckCount = G.discardCount[player] - drawsToTreasures(G.discard[player], G.discardCount[player], 1);
    } else {
      expectedDeckCount = G.deckCount[player] - drawsToTreasures(G.deck[player], G.deckCount[player], 2);
    }

    // check if the deckCount is as expected
    assertTrue(
      testG.deckCount[player],
      expectedDeckCount,
      "deckCount",
      &deckCountFails,
      &totalFails
    );

    // check if the handCount is as expected
    // this test exposes a bug (played adventurer card is not removed from hand)
    assertTrue(
      testG.handCount[player],
      G.handCount[player] + totalTreasuresToDraw - discarded,
      "handCount",
      &handCountFails,
      &totalFails
    );

    // calculate expected discard count
    if (treasureCardsInDeck==0){
      expectedDiscardCount =
        G.deckCount[player] // reveal entire deck
        + G.discardCount[player]
        - (G.discardCount[player] >= 2 ? 2 : G.discardCount[player])
        + discarded;
    } else if (treasureCardsInDeck == 1){
      expectedDiscardCount =
        G.deckCount[player] // reveal entire deck
        + G.discardCount[player]
        - (G.discardCount[player] >= 1 ? 1 : G.discardCount[player])
        + discarded;
    } else {
      expectedDiscardCount = G.discardCount[player]
        + drawsToTreasures(G.deck[player], G.deckCount[player], 2)
        - totalTreasuresToDraw
        + discarded;
    }

    // checks if discard count is as expected
    assertTrue(
      testG.discardCount[player],
      expectedDiscardCount,
      "discardCount",
      &discardCountFails,
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

  } // close testing loop

  // stop the timer
  endTime = clock();
  secondsToRun = ((double) (endTime - startTime)) / CLOCKS_PER_SEC;

  // print overall testing results
  printf("\n\n=================== RANDOM TESTING RESULTS ===================\n");
  printf("Function tested: %s \n", fName);
  printf("treasureCardsInHand tests failed: %d\n", treasureCardsinHandFails);
  printf("deckCount tests failed: %d\n", deckCountFails);
  printf("handCount tests failed: %d\n", handCountFails);
  printf("discardCount tests failed: %d\n", discardCountFails);
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
