/*
 * Interface.h
 *
 *  Created on: Jul 15, 2024
 *      Author: tux76
 */

#ifndef INTERFACE_H_
#define INTERFACE_H_

#include <iostream>
using namespace std;

#include "game.h"
#include "minimax.h"
#include "timeit.h"


class Interface {
private:
	Connect4 &game;
	AI &computerX;
	AI &computerY;
	TimeIt timer;
public:
	Interface(Connect4 &_game, AI &_computerX, AI &_computerY);

	static char getSpaceChar(int space);

	void printGame(bool oneLine = 0);

	void printState(AI &computer);
	void printInfo();

	bool play();
};



#endif /* INTERFACE_H_ */
