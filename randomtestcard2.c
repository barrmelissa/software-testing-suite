
#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"
#include <stdlib.h>
#include <time.h>

#define TESTCARD "smithy"

//made my own assertTrue function
int assertTrue(int result, int test, char *message, int *count){
    if(result == test){
        printf("PASS: %s\n", message);
    }else{
        printf("FAIL: %s\n", message);
        *count += 1;
    }
    return 0;
}

int randSmithy()
{
    //logical reason for choosing %10 is becuase this will give us a range of getting 0 - 9 smithy
    //cards in one turn which is way more than a user would get on a single turn so we are testing all possibilities
	int choosenInt = rand() %10;
	return choosenInt;
}

int randHandCount()
{
    //logical reason for choosing %6 is becuase this will give us a range of getting 0 - 5 cards and the hand count
    //starts at 5 so the hand count should remain the same after the smithy card is played if it is played between 0-5
	int choosenInt = rand() %6;
	return choosenInt;
}

int test_smithy() {
	int count = 0;

    int handpos = 0, choice1 = 0, choice2 = 0, choice3 = 0, bonus = 0;
    int seed = 1000;
    int numPlayers = 2;
    int thisPlayer = 0;
	char *message;
	struct gameState G, testG;
	int k[10] = {adventurer, embargo, village, minion, mine, cutpurse,
			sea_hag, tribute, smithy, council_room};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	// initialize a game state and player cards
	initializeGame(numPlayers, k, seed, &G);

	printf("\n----------------- randomtestcard2.c -> Testing Card: %s ----------------\n", TESTCARD);

    printf("\nTEST 1: Hand count correctly updated after running smithy card a random number of times\n");

	// copy the game state to a test case
	memcpy(&testG, &G, sizeof(struct gameState));

    //plays smithy card for a random number of times
    int numOfHand = randHandCount();
    for(int i = 0; i<numOfHand; i++){
	cardEffect(smithy, choice1, choice2, choice3, &testG, handpos, &bonus);
    }
    //checks if the correct number of actions are updated after village card is played a random number of times
 	message = "hand count updated correctly after running smithy a random number of times";
	assertTrue(testG.handCount[thisPlayer], G.handCount[thisPlayer] + numOfHand, message, &count);


////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	initializeGame(numPlayers, k, seed, &G);

    printf("\nTEST 2: Played card count updated correctly after running smithy card a random number of times\n");

	// copy the game state to a test case
	memcpy(&testG, &G, sizeof(struct gameState));

    //plays smithy card for a random number of times
    int numOfSmithy = randSmithy();
    for(int i = 0; i<numOfSmithy; i++){
	cardEffect(smithy, choice1, choice2, choice3, &testG, handpos, &bonus);
    }

    //checks if the correct number of actions are updated after village card is played a random number of times
 	message = "number of cards played is equivlent to number of times smithy card was played.";
	assertTrue(testG.playedCardCount, G.playedCardCount + numOfSmithy, message, &count);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////



    if(count == 0){
        printf("\nAll tests passed!\n\n");
    }
    else{
        printf("\nERROR: %d tests failed\n\n", count);
    }

	return 0;
}

int main(){
    srand(time(NULL));
	test_smithy();
	return 0;
}