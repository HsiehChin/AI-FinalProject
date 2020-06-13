#pragma once
#ifndef CHIN_H_
#define CHIN_H_

int chin_no_system(int house[], int turn);

bool relocation(int* house, int picked_house);
void show_house(int house[]);
void final_scoring(int* house);
bool check_end(int* house, int player);

#endif // !CHIN_H_