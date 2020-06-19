#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <windows.h>
#include <time.h>

#include "chin.h"
#include "peipei.h"
#include "yu.h"

static int(*play[3])(int*, int) = { yu_no_system, pei_no_system, chin_no_system };

int main() {
	int house[14] = { 6,6,6,6,6,6,0,6,6,6,6,6,6,0 };
	bool player0_move = true, player1_move = true;
	int player0, player1, player0_action = -1, player1_action = -1;
	char name[3][6] = { "yu", "pei", "chin" };

	srand(time(NULL));

	/*-- choose player --*/
	printf("The first player is (0)yu or (1)pei or (2)chin? ");
	scanf("%d", &player0);
	printf("The second player is (0)yu or (1)pei or (2)chin? ");
	scanf("%d", &player1);
	printf("\n");

	while (1) {

		if (player0_move == true) {
			player0_action = play[player0](house, 0);
			show_house(house);
			printf("%4s's turn to take action: %d\n\n", name[player0], player0_action);
			if (relocation(house, player0_action)) {
				player1_move = false;
			}
			else {
				player1_move = true;
			}
			if (check_end(house, 0)) break; //if game end then break loop
		}

		if (player1_move == true) {
			player1_action = play[player1](house, 1);
			show_house(house);
			printf("%4s's turn to take action: %d\n\n", name[player1], player1_action);
			if (relocation(house, player1_action)) {
				player0_move = false;
			}
			else {
				player0_move = true;
			}
			if (check_end(house, 1)) break; //if game end then break loop
		}
	}

	printf("Game is over, calculating final result...\n");
	final_scoring(house); // print result
	printf("Final result:\n");
	show_house(house);
	printf("\n");

	if (house[6] > house[13]) {//player0 win
		printf("%4s win~~~~\n", name[player0]);
	}
	else if (house[6] < house[13]) {//player1 win
		printf("%4s win~~~~\n", name[player1]);
	}
	else {//in a draw
		printf("The game between %s and %s ended in a draw\n", name[player0], name[player1]);
	}

	return 0;
}