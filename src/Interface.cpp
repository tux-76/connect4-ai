/*
 * Interface.cpp
 *
 *  Created on: Jul 15, 2024
 *      Author: tux76
 */

#include "Interface.h"

Interface::Interface (Connect4 &_game, AI &_computerX, AI &_computerY) : game(_game), computerX(_computerX), computerY(_computerY) {

}

char Interface::getSpaceChar(int space) {
	if (space==0) return '-';
	else if (space==1) return 'x';
	else if (space==-1) return 'o';
	return '?';
}

void Interface::printGame(bool oneLine) {
	cout << "6 5 4 3 2 1 0" << endl;
	for (int i = 41; i>=0; i--) {
		cout << getSpaceChar(game.board[i]) << " ";
		if (i % 7 == 0 && !oneLine) {
			cout << endl;
		}
	}
	cout << "Next: " << getSpaceChar(game.turn) << endl;
}

void Interface::printState(AI &computer) {
	timer.start();
	game.getState();
	cout << "PV:" << game.totalPiecesScore << " SpV:" << game.totalSpacesScore << endl;
	cout << "Predicted value: " << computer.minimax(game, computer.processingDepth) << endl;
	cout << "Time: ";
	timer.stop();
}
void Interface::printInfo() {
	cout << "Move History: " << endl;
	for (int play : game.moveHistory) {
		cout << play << ',';
	}
	cout << endl;
}

bool Interface::play() {
	string input;
	int play;
	int gameState = 2;
	AI* computer = &computerX;
	while (true) {
		cout << endl;

		gameState = game.getState();
		printGame();

		if (game.turn == 1) {
			computer = &computerX;
		} else {
			computer = &computerY;
		}

		if (gameState == GS_ACTIVE)
			cout << "Enter a move (type 'exit' to quit): ";
		else if (gameState == GS_MAX_WIN)
			cout << "X WINS! (type 'exit' to quit, 'r' to restart): ";
		else if (gameState == GS_MIN_WIN)
			cout << "O WINS! (type 'exit' to quit, 'r' to restart): ";
		else if (gameState == GS_DRAW)
			cout << "DRAW! (type 'exit' to quit, 'r' to restart): ";
		getline(cin, input);

		if (input == "exit") {
			printInfo();
			break;
		}
		else if (input == "-") {
			game.undoPlay();
			continue;
		}
		else if (input == "r") {
			printInfo();
			return 1;
		}
		else if (input == "st") {
			printState(*computer);
			continue;
		}
		else if (input == "i") {
			printInfo();
			continue;
		}
		else if (input[0] == 'a') {
			string movesStr = input.substr(1);
			if (movesStr.find(',') == std::string::npos) {
				game.play(stoi(movesStr));
			} else {
				string moveSubstr = "";
				for (char c : movesStr) {
					if (c == ',') {
						game.play(stoi(moveSubstr));
						moveSubstr = "";
					} else {
						moveSubstr += c;
					}
				}
				if (moveSubstr != "") {
					game.play(stoi(moveSubstr));
				}
			}

			continue;
		}
		else if (input[0] == 'd') {
			computer->processingDepth = stoi(input.substr(1));
			continue;
		}
		else if (gameState == GS_ACTIVE) {
			game.calcPossiblePlaysByColumn();

			try { // PLAYER PLAY
				play = game.possiblePlaysByColumn[stoi(input)];
			}
			catch (const std::invalid_argument& e) {
				if (input == "" /*&& gameState == GS_ACTIVE*/) {
					// COMPUTER PLAY
					play = computer->selectPlayDynamic(game);
				}
				else {
					cout << "Please try again." << endl << endl;
					continue;
				}
			}
		}
		else {
			cout << "Invalid input. This game is no longer active." << endl << endl;
			continue;
		}

		if (play != -1) {
			if (gameState == GS_ACTIVE) game.play(play);
		} else {
			cout << "That column is full! Try again." << endl;
		}

		cout << endl;
	}
	return 0;
}



