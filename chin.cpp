#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<stdbool.h>  

static int minimaxDecision(int house[], int turn, int depthMAX);
static void maxValue(int house[], int turn, int depth, int depthMAX, int* mValue, int* action, int* alpha, int* beta, int init_turn);
static void minValue(int house[], int turn, int depth, int depthMAX, int* mValue, int* action, int* alpha, int* beta, int init_turn);
bool relocation(int* house, int picked_house);

bool check_end(int* house, int player) {
	bool flag = true;
	if (player == 0) {
		for (int i = 0; i < 6; i++) {
			if (house[i] != 0) flag = false;
		}
	}
	else if (player == 1) {
		for (int i = 7; i < 13; i++) {
			if (house[i] != 0) flag = false;
		}
	}
	return flag;
}

void final_scoring(int* house) {
	int sum = 0;
	for (int i = 0; i < 6; i++) {
		sum += house[i];
		house[i] = 0;
	}
	house[6] += sum;
	sum = 0;
	for (int i = 7; i < 13; i++) {
		sum += house[i];
		house[i] = 0;
	}
	house[13] += sum;
}

bool relocation(int* house, int picked_house) {
	bool flag = false;
	int seed_num = house[picked_house];
	int final_pos = picked_house;
	int player = (picked_house < 6) ? 0 : 1;

	if (picked_house != 6 && picked_house != 13) {//Avoid picked the wrong house  
		house[picked_house] = 0;
		while (seed_num != 0) {
			final_pos = (final_pos + 1) % 14;
			/*--Avoid put the seed into opponent's store--*/
			if ((final_pos == 13 && picked_house < 6) || (final_pos == 6 && picked_house > 6)) continue;
			house[final_pos] += 1;
			seed_num -= 1;
		}
		if (final_pos == 6 || final_pos == 13) { flag = true; }
		else if (house[final_pos] == 1 && final_pos != picked_house && house[12 - final_pos] != 0) {
			/*--Check the empty capture--*/
			if ((player == 0 && final_pos < 6) || (player == 1 && final_pos > 6)) {
				int store_index = (player == 0) ? 6 : 13;
				house[store_index] += (house[final_pos] + house[12 - final_pos]);
				house[final_pos] = house[12 - final_pos] = 0;
			}
		}
		if (check_end(house, 0) || check_end(house, 1)) {
			final_scoring(house);
			flag = false;
		}
	}

	return flag;
}

void show_house(int house[]) {
	printf("[%2d]", house[13]);
	for (int i = 12; i > 6; i--) printf(",%2d", house[i]);
	printf("\n     ");
	for (int i = 0; i < 6; i++) printf("%2d,", house[i]);
	printf("[%2d]\n", house[6]);
}

static void copy_state(int* a, int* b) {
	for (int i = 0; i < 14; i++) {
		a[i] = b[i];
	}
}

static void evaluate(int init_turn, int* mValue, int house[]) {
	if (init_turn == 0) *mValue = house[6] - house[13];
	else *mValue = house[13] - house[6];
}

static void maxValue(int house[], int turn, int depth, int depthMAX, int* mValue, int* action, int* alpha, int* beta, int init_turn) {
	bool end_flag = false;
	if (check_end(house, 0) || check_end(house, 1)) {//check final node
		final_scoring(house);
		end_flag = true;
	}
	if (depth < depthMAX && !end_flag) {//means can produce successor
		int start_index = 0, end_index = 6, temp = *alpha;
		if (turn == 1) { start_index = 7; end_index = 13; }

		for (int i = start_index; i < end_index; i++) {
			int state[14], mValue1 = 1000, alpha1 = *alpha, beta1 = *beta, action1 = (house[i] != 0) ? i : -1;
			copy_state(state, house);

			if (state[i] != 0) {//means it have processors
				if (relocation(state, i)) {//current turn earn one more turn
					mValue1 = -1000;
					maxValue(state, turn, depth + 2, depthMAX, &mValue1, &action1, &alpha1, &beta1, init_turn);//calculate next successor
				}
				else {//change to next turn
					minValue(state, (turn + 1) % 2, depth + 1, depthMAX, &mValue1, &action1, &alpha1, &beta1, init_turn);//calculate next successor
				}
				if (*mValue < mValue1) {
					*mValue = mValue1;
					if (depth == 0) *action = action1;
				}

				if (mValue1 >= *beta) {
					break;
				}
				if (mValue1 > * alpha) {
					*alpha = mValue1;
					temp = *alpha;
				}
			}
		}
	}
	else {//get the mValue
		evaluate(init_turn, mValue, house);
	}
}

static void minValue(int house[], int turn, int depth, int depthMAX, int* mValue, int* action, int* alpha, int* beta, int init_turn) {

	bool end_flag = false;
	if (check_end(house, 0) || check_end(house, 1)) {//check final node
		final_scoring(house);
		end_flag = true;
	}
	if (depth < depthMAX && !end_flag) {//means can produce successor
		int start_index = 0, end_index = 6, temp = *beta, i;
		if (turn == 1) { start_index = 7; end_index = 13; }

		for (i = start_index; i < end_index; i++) {
			int state[14], mValue1 = -1000, alpha1 = *alpha, beta1 = *beta, action1 = (house[i] != 0) ? i : -1;
			copy_state(state, house);

			if (state[i] != 0) {//means it have processors
				if (relocation(state, i)) {//current turn earn one more turn
					mValue1 = 1000;
					minValue(state, turn, depth + 2, depthMAX, &mValue1, &action1, &alpha1, &beta1, init_turn);//calculate next successor
				}
				else {//change to next turn
					maxValue(state, (turn + 1) % 2, depth + 1, depthMAX, &mValue1, &action1, &alpha1, &beta1, init_turn);//calculate next successor
				}
				if (*mValue > mValue1) {
					*mValue = mValue1;
					if (depth == 0) *action = action1;
				}
				if (mValue1 <= *alpha) {
					break;
				}
				if (mValue1 < *beta) {
					*beta = mValue1;
					temp = *beta;
				}
			}
		}
	}
	else {//get the mValue
		evaluate(init_turn, mValue, house);
	}
}

static int minimaxDecision(int house[], int turn, int depthMAX) {

	int depth = 0;
	int mValue = -1000, action = -1, alpha = -1000, beta = 1000;

	maxValue(house, turn, depth, depthMAX, &mValue, &action, &alpha, &beta, turn);

	return action;
}

//return the action of next step
int chin_no_system(int house[], int turn) {
	return minimaxDecision(house, turn, 14);
}