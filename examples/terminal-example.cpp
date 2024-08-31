//============================================================================
// Name        : Connect4_AI.cpp
// Author      : tux76
//============================================================================

#include "connect4_ai.h"
using namespace Connect4AI;

// MAIN
// =================================
int main() {
	bool running = 1;
	while (running) {
		int board[42] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
		Connect4 game(board);

		AI computerX(8, 0, "ComputerX");
		AI computerY(8, 0, "ComputerY");
		Interface interface(game, computerX, computerY);

		running = interface.play();
	}
}
