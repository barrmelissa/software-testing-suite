
#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"
#include <stdlib.h>
#include <time.h>

#define TESTCARD "village"

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

int randVillage()
{
    //logical reason for choosing %10 is becuase this will give us a range of getting 0 - 9 village
    //cards in one turn which is way more than a user would get on a single turn so we are testing all possibilities
	int choosenInt = rand() %10;
	return choosenInt;
}

int randHandCount()
{
    //logical reason for choosing %6 is becuase this will give us a range of getting 0 - 5 cards and the hand count
    //starts at 5 so the hand count should remain the same after the village card is played if it is played between 0-5
	int choosenInt = rand() %6;
	return choosenInt;
}

int test_village() {
	int count = 0;
    int newCards = 0;
	int newAction = 0;
    int discarded = 1;

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

	printf("\n----------------- randomtestcard1.c -> Testing Card: %s ----------------\n", TESTCARD);
   
	// // copy the game state to a test case
	memcpy(&testG, &G, sizeof(struct gameState));

    printf("\nTEST 1: Hand count correctly updated after running village card a random number of times\n");
	newCards = 1;
    //plays village card for a random number of times
    int numOfHand = randHandCount();
    for(int i = 0; i<numOfHand; i++){
	cardEffect(village, choice1, choice2, choice3, &testG, handpos, &bonus);
    }

    //checks if the correct number of actions are updated after village card is played a random number of times
 	message = "hand count updated correctly after running village a random number of times";
	assertTrue(testG.handCount[thisPlayer], G.handCount[thisPlayer] + newCards - discarded, message, &count);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    printf("\nTEST 2: Action count correctly updated after running village card a random number of times\n");
	initializeGame(numPlayers, k, seed, &G);
	memcpy(&testG, &G, sizeof(struct gameState));

	newAction = 2;
    //plays village card for a random number of times
    int numOfVillage = randVillage();
    for(int i = 0; i<numOfVillage; i++){
	cardEffect(village, choice1, choice2, choice3, &testG, handpos, &bonus);
    }

    //checks if the correct number of actions are updated after village card is played a random number of times
 	message = "Expect fail: correct number of actions added to total action count after running village a random number of times";
	assertTrue(testG.numActions, G.numActions + (numOfVillage*newAction)-1, message, &count);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    printf("\nTEST 3: Played card count correctly updated after running village card a random number of times\n");
	initializeGame(numPlayers, k, seed, &G);
	memcpy(&testG, &G, sizeof(struct gameState));

    //plays village card for a random number of times
    numOfVillage = randVillage();
    for(int i = 0; i<numOfVillage; i++){
	cardEffect(village, choice1, choice2, choice3, &testG, handpos, &bonus);
    }
    //checks if the correct number of actions are updated after village card is played a random number of times
 	message = "number of cards played is equivlent to number of times village card was played.";
	assertTrue(testG.playedCardCount, G.playedCardCount + numOfVillage, message, &count);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    if(count == 0){
        printf("\nAll tests passed!\n\n");
    }
    else{
        printf("\nERROR: %d tests failed\n", count);
    }

	return 0;
}

int main(){
    srand(time(NULL));
	test_village();
	return 0;
}

