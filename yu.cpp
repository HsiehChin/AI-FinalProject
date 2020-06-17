#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdbool.h>
#include<limits.h>
#include<stdlib.h>
#define INT_MIN -32767
#define INT_MAX +32767
int first_turn;
static void maxValue(int house[], int turn, int depth, int depthMAX, int* mValue, int* action);
static void minValue(int house[], int turn, int depth, int depthMAX, int* mValue, int* action);

static void finalScoring(int house[]) {
	for (int i = 0; i < 6; i++) {
		house[6] += house[i];
		house[i] = 0;
	}
	for (int i = 7; i < 13; i++) {
		house[13] += house[i];
		house[i] = 0;
	}
}

static bool relocation(int house[], int pick) {
	int tmp = 0, last, count;
	int flag = 0, cnt = 0;
	tmp = pick + 1;
	count = house[pick];
	house[pick] = 0;
	for (int i = 0; i < count; i++, tmp++) {
		if (tmp == 14) tmp -= 14;
		if (pick < 6) {
			if (tmp != 13) house[tmp]++;
			else i--;
		}
		else {
			if (tmp != 6) house[tmp]++;
			else i--;
		}
		last = tmp;
	}
	if (house[last] == 1 && last != pick) {
		if (last != 13 && last != 6) {
			if (house[12 - last] != 0) {
				if (pick < 6 && last < 6) {
					house[6] = house[6] + house[12 - last] + 1;
					house[last] = house[12 - last] = 0;
				}
				else if (pick > 6 && last > 6) {
					house[13] = house[13] + house[12 - last] + 1;
					house[last] = house[12 - last] = 0;
				}
			}
		}
	}
	if ((house[0] == 0 && house[1] == 0 && house[2] == 0 && house[3] == 0 && house[4] == 0 && house[5] == 0) || (house[7] == 0 && house[8] == 0 && house[9] == 0 && house[10] == 0 && house[11] == 0 && house[12] == 0)) {
		finalScoring(house);
		flag = 1;
	}
	if (flag == 0) {
		if (pick < 6 && last == 6 || pick > 6 && last == 13) return true;
		else return false;
	}
	else return false;
}

static int minimaxDecision(int house[], int turn, int depthMAX) {
	int mValue = INT_MIN, action = -1, depth = 0;
	maxValue(house, turn, depth, depthMAX, &mValue, &action);
	return action;
}

static void maxValue(int house[], int turn, int depth, int depthMAX, int* mValue, int* action) {
	if ((house[0] == 0 && house[1] == 0 && house[2] == 0 && house[3] == 0 && house[4] == 0 && house[5] == 0) || (house[7] == 0 && house[8] == 0 && house[9] == 0 && house[10] == 0 && house[11] == 0 && house[12] == 0)) {
		finalScoring(house);
		if (first_turn == 1) *mValue = house[13] - house[6];
		else *mValue = house[6] - house[13];
	}
	else {
		if (depth < depthMAX) {
			int i;
			if (turn == 0) i = 0;
			else i = 7;
			for (int j = i; j < i + 6; j++) {
				int copy[14], t_mValue = INT_MAX, t_action = -1, t_turn = turn, tmp = 0;
				for (int x = 0; x < 14; x++) copy[x] = house[x];//clone house to copy 
				if (copy[j] != 0) {
					if (relocation(copy, j) == true) {//can do again
						t_mValue = INT_MIN;
						t_action = j;
						maxValue(copy, turn, depth + 2, depthMAX, &t_mValue, &t_action);
					}
					else {//another player's turn
						if (t_turn == 0) t_turn = 1;
						else t_turn = 0;
						t_action = j;
						minValue(copy, t_turn, depth + 1, depthMAX, &t_mValue, &t_action);
					}
					if (*mValue < t_mValue) {//find the bigger number
						*mValue = t_mValue;
						if (depth == 0) *action = t_action;
						tmp = t_action;
					}
					else if (*mValue == t_mValue) {
						if (t_action < tmp) *mValue = t_mValue;
						if (depth == 0) *action = t_action;
						tmp = t_action;
					}
				}
			}
		}
		else {
			if (first_turn == 1) *mValue = house[13] - house[6];
			else *mValue = house[6] - house[13];
		}
	}
}

static void minValue(int house[], int turn, int depth, int depthMAX, int* mValue, int* action) {
	if ((house[0] == 0 && house[1] == 0 && house[2] == 0 && house[3] == 0 && house[4] == 0 && house[5] == 0) || (house[7] == 0 && house[8] == 0 && house[9] == 0 && house[10] == 0 && house[11] == 0 && house[12] == 0)) {
		finalScoring(house);
		if (first_turn == 1) *mValue = house[13] - house[6];
		else *mValue = house[6] - house[13];
	}
	else {
		if (depth < depthMAX) {
			int i;
			if (turn == 0) i = 0;
			else i = 7;
			for (int j = i; j < i + 6; j++) {
				int copy[14], t_mValue = INT_MIN, t_action = -1, t_turn = turn;
				for (int x = 0; x < 14; x++) copy[x] = house[x];//clone house to copy
				if (copy[j] != 0) {
					if (relocation(copy, j) == true) {//can do again
						t_mValue = INT_MAX;
						t_action = j;
						minValue(copy, turn, depth + 2, depthMAX, &t_mValue, &t_action);
					}
					else {//another player's turn
						if (t_turn == 0) t_turn = 1;
						else t_turn = 0;
						t_action = j;
						maxValue(copy, t_turn, depth + 1, depthMAX, &t_mValue, &t_action);
					}
					if (*mValue > t_mValue) {//find the smaller number
						*mValue = t_mValue;
						if (depth == 0) *action = t_action;
					}
				}
			}
		}
		else {
			if (first_turn == 1) *mValue = house[13] - house[6];
			else *mValue = house[6] - house[13];
		}
	}
}

int yu_no_system(int houses[], int turn) {
	return minimaxDecision(houses, turn, 8);
}