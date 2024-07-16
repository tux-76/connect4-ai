//============================================================================
// Name        : Connect4_AI.cpp
// Author      : tux76
//============================================================================


#include "Game.h"
#include "TimeIt.h"
#include "Minimax.h"
#include "Interface.h"

// MAIN
// =================================
int main() {

//	Connect4 game;
////	computer.selectPlay(game);
//	for (int play : game.getPossiblePlays()) {
//		cout << play << " ";
//	}
//	cout << endl;

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
