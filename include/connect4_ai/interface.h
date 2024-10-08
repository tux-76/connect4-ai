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

#include "connect4_ai/game.h"
#include "connect4_ai/minimax.h"
#include "connect4_ai/timeit.h"

namespace Connect4AI {

class Interface {
private:
	Connect4 &game;
	AI &computerX;
	AI &computerY;
	TimeIt timer;
	bool aiIsDynamic;
public:
	Interface(Connect4 &_game, AI &_computerX, AI &_computerY, bool _aiIsDynamic = true);

	static char getSpaceChar(int space);

	static void printGame(Connect4 &_game, bool oneLine = 0);

	void printState(AI &computer);
	void printInfo();

	bool play();
};

} /* Connect4AI */
#endif /* INTERFACE_H_ */
