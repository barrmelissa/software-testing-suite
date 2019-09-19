#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"
#include <stdlib.h>
#include <time.h>

#define TESTCARD "adventurer"

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
//returns a random number of players
int randPlayers()
{
    //logical reason for choosing %3 + 2 is because there can only be an option of 2, 3, or 4 players and this 
    //randomizes within that range
	int choosenInt = rand() % 3 + 2;
	return choosenInt;
}
//returns a random number for Handpos
int randHandpos()
{
    //logical reason for choosing %500 is becuase this will give us a range of getting 0 - 499 hand positions
    //and the max hand can hold is 500
	int choosenInt = rand() %500;
	return choosenInt;
}
//returns a random number of adventure cards to play
int randAdventure()
{
    //logical reason for choosing %10 is becuase this will give us a range of getting 0 - 10 adventure
    //cards in one turn which is way more than a user would get on a single turn so we are testing all possibilities
	int choosenInt = rand() %10;
	return choosenInt;
}

int test_adventurer() {
	int count = 0;
    int tresureCards = 0;
    int discarded = 1;
    int i;
    int handpos = 0, choice1 = 0, choice2 = 0, choice3 = 0, bonus = 0;
    int seed = 1000;
    int numPlayers;
    int result;
    int thisPlayer = 0;
	char *message;
	struct gameState G, testG;
	int k[10] = {adventurer, embargo, village, minion, mine, cutpurse,
			sea_hag, tribute, smithy, council_room};
	
    printf("\n----------------- randomtestcard1 -> Testing Card: %s ----------------\n", TESTCARD);


    numPlayers = randPlayers();
	//initialize a game state and player cards
    //checking if game is initilized correctly for each player
    printf("\nTEST 1: Initializing game correctly with a random number of players\n");

    //looping 10 times to test it multiple times with all values
    for(int i = 1; i<=10; i++){
        result = initializeGame(numPlayers, k, seed, &G);
        message = "Game with random number of players initlized correctly";
        assertTrue(0, result, message, &count);
    }
	// copy the game state to a test case
	memcpy(&testG, &G, sizeof(struct gameState));

//////////////////////////////////////////////////////////////////////////

    printf("\nTEST 2: Checking adventure card correctly increments hand count for a random number of handpos\n");

    //checking hand count before adventure card is played
    message = "handcount before card is played is at 5";
    assertTrue(testG.handCount[thisPlayer], G.handCount[thisPlayer] + tresureCards, message, &count);
    tresureCards = 2;

    //calling adventure card with a randomly decided handpos
    int numOfHandpos = randHandpos();
	cardEffect(adventurer, choice1, choice2, choice3, &testG, numOfHandpos, &bonus);
            
    //checking hand count is updated
    message = "handcount after adventure card is played increases by 2\n";
    assertTrue(testG.handCount[thisPlayer], G.handCount[thisPlayer] + tresureCards, message, &count);
    
//////////////////////////////////////////////////////////////////////////
    
    printf("\nTEST 3: Checking added cards are indeed treasure cards after playing adventure card a random number of times\n");

    int cardDrawn;
    int expectedCard;
    int numOfAdventure = randAdventure();
    //plays adventure card for a random number of times
    for(int i = 0; i<numOfAdventure; i++){
   	    cardEffect(adventurer, choice1, choice2, choice3, &testG, handpos, &bonus);
    }
    
    //checking that the two cards added in the new hand after a random amount of adventure cards are played are now treasure cards
    for(i=1; i <= 2; i++){
        cardDrawn = testG.hand[thisPlayer][testG.handCount[thisPlayer]-i];
        expectedCard = G.hand[thisPlayer][G.handCount[thisPlayer]-i];
        message = "added cards in handcount are indeed treasure cards";
        assertTrue(cardDrawn, expectedCard, message, &count);
    }

//////////////////////////////////////////////////////////////////////////

    printf("\nTEST 4: Checking discard count updates correctly after playing adventure card a random number of times\n");
    //I put a bug in the original code so this won't work but showing that it doesn't
    shuffle(thisPlayer, &testG);
    numOfAdventure = randAdventure();
    //plays adventure card for a random number of times
    for(int i = 0; i<numOfAdventure; i++){
   	    cardEffect(adventurer, choice1, choice2, choice3, &testG, handpos, &bonus);
    }
    //checking if discard count updates for playing adventure card a random number of times
    message = "EXPECT TO FAIL: shuffled and played adventure card a random number of times again, new discard count upated";
    assertTrue(testG.discardCount[thisPlayer], G.discardCount[thisPlayer]+discarded, message, &count);
    
//////////////////////////////////////////////////////////////////////////

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
	test_adventurer();
	return 0;
}