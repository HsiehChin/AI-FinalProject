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
void human_input(const char* name, int *action, int turn);

int main() {
	int house[14] = { 6,6,6,6,6,6,0,6,6,6,6,6,6,0 };
	bool player0_move = true, player1_move = true, two_player = false;
	int player0, player1, player0_action = -1, player1_action = -1;
	char name[5][20] = { "yu", "pei", "chin" };
	srand(time(NULL));

	/*-- choose player --*/
	printf("The first player is (0)yu or (1)pei or (2)chin or (3)human? ");
	scanf("%d", &player0);
	if (player0 == 3) {
		printf("Please input user name: ");
		scanf("%s", &name[3]);
		two_player = true;
	}
	printf("The second player is (0)yu or (1)pei or (2)chin or (3)human? ");
	scanf("%d", &player1);
	if (player1 == 3) {
		printf("Please input user name: ");
		if (two_player) {
			scanf("%s", &name[4]);
			player1 = 4;
		}
		else {
			scanf("%s", &name[3]);
		}
	}
	printf("\n");
	show_house(house);

	while (1) {

		if (player0_move == true) {
			if (player0 != 3) {
				player0_action = play[player0](house, 0);
				printf("%4s's turn to take action: %d\n\n", name[player0], player0_action);
			}
			else {
				human_input(name[player0], &player0_action, 0);
				printf("\n\n");
			}
			if (relocation(house, player0_action)) {
				player1_move = false;
			}
			else {
				player1_move = true;
			}
			if (check_end(house, 0)) break; //if game end then break loop
			show_house(house);
		}
		if (player1_move == true) {
			if (player1 != 3 && player1 !=4) {
				player1_action = play[player1](house, 1);
				printf("%4s's turn to take action: %d\n\n", name[player1], player1_action);
			}
			else {
				human_input(name[player1], &player1_action, 1);
				printf("\n\n");
			}
			if (relocation(house, player1_action)) {
				player0_move = false;
			}
			else {
				player0_move = true;
			}
			if (check_end(house, 1)) break; //if game end then break loop
			show_house(house);
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

/*---- Input the user action----*/
void human_input(const char* name, int *action, int turn) {
	int lower_bound = 0, higher_bound = 5;
	if (turn == 1) { lower_bound = 7; higher_bound = 12; }
	while (1) {
		printf("%4s's turn to take action: ", name, action);
		scanf("%d", action);
		if (*action >= lower_bound && *action <= higher_bound) break;
		else {
			printf("Pick the wrong bowl, please choose again!\n");
		}
	}
}