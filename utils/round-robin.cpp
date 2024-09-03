/*
 * round-robin.cpp
 *
 *  Created on: Aug 30, 2024
 *      Author: tux76
 */

#include "connect4_ai.h"
#include "connect4_ai/interface.h"
using namespace Connect4AI;

#include <fstream>

int battleAI(Connect4 game, AI &computerX, AI &computerY) {
	cout << computerX.name << " VS " << computerY.name << endl;

	int gameState = GS_ACTIVE;
	while (gameState == GS_ACTIVE) {
		// Move
		int play;
		if (game.turn == 1) {
			play = computerX.selectPlay(game);
		} else {
			play = computerY.selectPlay(game);
		}
		game.play(play);
		// Get state
		gameState = game.getState();
	}

	return gameState;
}

int main() {
	// Make starter game
	int board[42] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	Connect4 game(board);

	// Initialize
	std::ofstream file("data/space-weight-table.csv", std::ios::trunc);

	const float minValue = -0.3;
	const float maxValue = -0.1;
	const float step = 0.01;
	const int numPlayers = (maxValue-minValue) / step + 1;
	cout << minValue << '>' << maxValue << ':' << step << endl;
	file << minValue << '_' << maxValue << '_' << step <<',';
	cout << numPlayers << " Players:" << endl;

	// Make players
	vector<AI> players;
	for (int i = 0; i < numPlayers; i++) {
		float variable = minValue+i*step;
		AI computer(6, variable, "Player " + to_string(i));
		cout << '\t' << computer.name << endl
				<< "\t  Depth: " << computer.processingDepth << endl
				<< "\t  SpaceWeight: " << computer.spaceWeight << endl;
		players.push_back(computer);

//		if (i < numPlayers-1) file << i << ',';
//		else file << i << endl;
		file << variable << ',';
	}
	file << endl;

	// Proceed?
	char proceed;
	cout << "Continue? (y/n)" << endl;
	cin >> proceed;
	if (proceed != 'y') {
		cerr << "Abort" << endl;
		return 1;
	}

	// Loop games
	for (int y = 0; y < numPlayers; y++) {
		file << minValue+y*step << ',';
		for (int x = 0; x < numPlayers; x++) {
			cout << endl << x << " VS " << y << endl;

			int outcome = battleAI(game, players[x], players[y]);

			file << outcome << ',';
		}
		file << endl;
	}

	cout << "Done!" << endl;
	file.close();
	return 0;
}


