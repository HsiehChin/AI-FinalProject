#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>

static void maxValue(int houses[], int turn, int depth, int depthMAX, int* mValue, int* action, int init_turn);
static void minValue(int houses[], int turn, int depth, int depthMAX, int* mValue, int* action, int init_turn);
static void compare(int houses[], int* mValue, int turn);
static bool check_end(int houses[], int turn);
static void final_scoring(int houses[]);
static bool relocation(int houses[], int pickedHouse);
// int random_action(int houses[], int player);
int random_action(int houses[], int same_mvalue[], int array_length, int player);
int find_not_empty(int houses[], int player);


int random_action(int houses[], int same_mvalue[], int array_length, int player) {
	int index;
	int count = 0;
	int result;


	index = (rand() % array_length);

	if (houses[same_mvalue[index]] != 0) {
		return same_mvalue[index];
	}
	else {
		return find_not_empty(houses, player);
	}
}

int find_not_empty(int houses[], int player) {
	int start, end;
	int action = 0;
	if (player == 0) {
		start = 0;
		end = 5;
	}
	else {
		start = 7;
		end = 12;
	}

	for (int i = start; i <= end; i++) {
		if (houses[i] != 0) {
			action = i;
			break;
		}
	}

	return action;
}


static int minimaxDecision(int houses[], int turn, int depthMAX) {
	int mValue = INT_MIN;
	int action = 0;


	maxValue(houses, turn, 0, depthMAX, &mValue, &action, turn);


	return action;
}


static void maxValue(int houses[], int turn, int depth, int depthMAX, int* mValue, int* action, int init_turn) {
	int mValue_copy = INT_MIN;
	int copy[14];
	bool flag = check_end(houses, turn);
	int all_mvalue[6] = { 0 };
	int count = 0;
	for (int i = 0; i < 14; i++) {
		copy[i] = houses[i];
	}


	if (depth < depthMAX && flag == false) {
		if (turn == 1) {
			for (int i = 7; i <= 12; i++) {
				if (relocation(copy, i)) {
					maxValue(copy, turn, depth + 2, depthMAX, &mValue_copy, action, init_turn);

				}
				else {
					minValue(copy, 0, depth + 1, depthMAX, &mValue_copy, action, init_turn);
				}

				all_mvalue[i-7] = mValue_copy;

				if (*mValue <= mValue_copy) {
					*mValue = mValue_copy;
					if (*mValue == mValue_copy) {
						count++;
					}
				}
				else {
				}
			}

			int* same_mvalue = (int*)malloc(sizeof(int) * 6);
			for (int j = 0; j < count; j++) {
				if (all_mvalue[j] == *mValue) {
					same_mvalue[j] = j + 7;
				}
			}

			if (depth == 0) {
				*action = random_action(houses,same_mvalue, count, 1);
			}

		}
		else {
			for (int i = 0; i <= 5; i++) {
				if (relocation(copy, i)) {
					maxValue(copy, turn, depth + 2, depthMAX, &mValue_copy, action, init_turn);
				}
				else {
					minValue(copy, 1, depth + 1, depthMAX, &mValue_copy, action, init_turn);
				}
				all_mvalue[i] = mValue_copy;

				if (*mValue <= mValue_copy) {
					*mValue = mValue_copy;
					if (*mValue == mValue_copy) {
						count++;
					}
				}
				else {
				}
			}


			int* same_mvalue = (int*)malloc(sizeof(int) * 6);
			for (int j = 0; j < count; j++) {
				if (all_mvalue[j] == *mValue) {
					same_mvalue[j] = j;
				}
			}

			if (depth == 0) {
				*action = random_action(houses, same_mvalue, count, 0);
			}
		}
	}
	else {
		compare(houses, mValue, init_turn);
	}


}

static void minValue(int houses[], int turn, int depth, int depthMAX, int* mValue, int* action, int init_turn) {
	int mValue_copy = INT_MAX;
	int copy[14];
	bool flag = check_end(houses, turn);
	int all_mvalue[14] = { 0 };
	int count = 0;
	for (int i = 0; i < 14; i++) {
		copy[i] = houses[i];
	}

	if (flag) {
		final_scoring(houses);
	}


	if (depth < depthMAX && flag == false) {
		if (turn == 1) {
			for (int i = 7; i <= 12; i++) {
				if (relocation(copy, i)) {
					maxValue(copy, turn, depth + 2, depthMAX, &mValue_copy, action, init_turn);
				}
				else {
					minValue(copy, 0, depth + 1, depthMAX, &mValue_copy, action, init_turn);
				}
				all_mvalue[i - 7] = mValue_copy;


				if (*mValue >= mValue_copy) {
					*mValue = mValue_copy;
					if (*mValue == mValue_copy) {
						count++;
					}
				}
				else {
				}
			}


			int* same_mvalue = (int*)malloc(sizeof(int) * 6);
			for (int j = 0; j < count; j++) {
				if (all_mvalue[j] == *mValue) {
					same_mvalue[j] = j + 7;
				}
			}

			if (depth == 0) {
				*action = random_action(houses, same_mvalue, count, 1);
			}
		}
		else {
			for (int i = 0; i <= 5; i++) {
				if (relocation(copy, i)) {
					maxValue(copy, turn, depth + 2, depthMAX, &mValue_copy, action, init_turn);
				}
				else {
					minValue(copy, 1, depth + 1, depthMAX, &mValue_copy, action, init_turn);
				}

				all_mvalue[i] = mValue_copy;

				if (*mValue >= mValue_copy) {
					*mValue = mValue_copy;
					if (*mValue == mValue_copy) {
						count++;
					}
				}
				else {
				}
			}


			int* same_mvalue = (int*)malloc(sizeof(int) * 6);
			for (int j = 0; j < count; j++) {
				if (all_mvalue[j] == *mValue) {
					same_mvalue[j] = j;
				}
			}

			if (depth == 0) {
				*action = random_action(houses, same_mvalue, count, 0);
			}
		}
	}
	else {
		compare(houses, mValue, init_turn);
	}
}

static void compare(int houses[], int* mValue, int turn) {

	if (turn == 1) {
		*mValue = houses[13] - houses[6];
	}
	else {
		*mValue = houses[6] - houses[13];
	}
}

static bool check_end(int houses[], int turn) {

	int count = 0;
	if (turn == 0) {
		for (int i = 0; i < 6; i++) {
			if (houses[i] == 0) {
				count;
			}
		}
	}
	else {
		for (int i = 7; i < 13; i++) {
			if (houses[i] == 0) {
				count;
			}
		}
	}

	if (count == 6) return true;
	else return false;
}

static void final_scoring(int houses[]) {
	int total1 = 0;
	int total2 = 0;
	for (int i = 0; i < 6; i++) {
		total1 += houses[i];
		houses[i] = 0;
	}
	houses[6] += total1;

	for (int i = 7; i < 13; i++) {
		total2 += houses[i];
		houses[i] = 0;
	}
	houses[13] += total2;
}

static bool relocation(int houses[], int pickedHouse) {
	int count = houses[pickedHouse];
	int location = pickedHouse;
	bool result = false;
	int total = 0;
	houses[pickedHouse] = 0;
	bool flag = false;

	while (count--) {
		location++;
		// cannot be sowing in the opponent's bowl
		if ((pickedHouse <= 5 && location == 13) || (pickedHouse >= 7 && location == 6)) {
			count++;
			continue;
		}

		// put
		if (location > 13) {
			location = 0;
			houses[location] += 1;
			flag = true; // no.13 bowl come back to no.1 bowl
		}
		else {
			houses[location] += 1;
		}
	}

	// empty capture
	if (houses[location] == 1 && location != 13 && location != 6 && flag == false) {
		if (pickedHouse <= 5 && location <= 5 && houses[12 - location] != 0) {
			houses[location] -= 1;
			total = 1 + houses[12 - location];
			houses[12 - location] = 0;
			houses[6] += total;
		}
		else if (pickedHouse >= 7 && location >= 7 && houses[12 - location] != 0) {
			houses[location] -= 1;
			total = 1 + houses[12 - location];
			houses[12 - location] = 0;
			houses[13] += total;
		}
	}

	if (location == 6 || location == 13) {
		result = true;
	}
	return result;
}

static void show_house(int houses[]) {

	printf("[ ");
	for (int i = 0; i < 14; i++) {
		if (i == 13) {
			printf("%d ]\n", houses[i]);
		}
		else {
			printf("%d ", houses[i]);
		}
	}
}


int pei_no_system(int houses[], int turn) {
	return minimaxDecision(houses, turn, 8);
}