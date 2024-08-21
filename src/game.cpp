/*
 * Connect4.cpp
 *
 *  Created on: Jul 15, 2024
 *      Author: tux76
 */

#include "game.h"
using namespace Connect4AI;


void Connect4::getConnectionLines () {
	vector<int> newVector;
	// Horizontal
	for (int row = 0; row < 6; row++) {
		newVector.clear();
		for (int col = 0; col < 7; col++) {
			newVector.push_back(row*7+col);
		}
		connectionLines.push_back(newVector);
	}
	// Vertical
	for (int col = 0; col < 7; col++) {
		newVector.clear();
		for (int row = 0; row < 6; row++) {
			newVector.push_back(row*7+col);
		}
		connectionLines.push_back(newVector);
	}
	// Left diagonals
	unordered_map<int, vector<int>> diagonalsMap;
	for (int row = 0; row < 6; row++) for (int col = 0; col < 7; col++) {
		int key = row-col;
		int space = row*7+col;
		auto mapIt = diagonalsMap.find(key);
		if (mapIt == diagonalsMap.end()) {
			newVector.clear();
			newVector.push_back(space);
			diagonalsMap[key] = newVector;
		} else {
			mapIt->second.push_back(space);
		}
	}
	// Right diagonals (mirror left diagonals)
	for (auto &pair : diagonalsMap) if (pair.second.size() >= 4){
		connectionLines.push_back(pair.second);
		newVector.clear();
		for (int space : pair.second) {
			int x = space % 7; int y = space / 7;
			newVector.push_back(y*7+(6-x));
		}
		connectionLines.push_back(newVector);
	}
}

Connect4::Connect4 (int _board[42]) {
	turn = 1;
	for (int i = 0; i < 42; i++) {
		board[i] = _board[i];
	}
	getConnectionLines();

	scoringPlayer = 0;
	piecesScore = 0;
	spacesScore = 0;
	adjacentPieces = 0;
	adjacentSpaces = 0;
}

void Connect4::calcPossiblePlaysByColumn() {
	int space;
	for (int col = 0; col < 7; col++) {
		possiblePlaysByColumn[col] = -1;
		space = col;
		while (space < 42) {
			if (board[space] == SPACE_EMPTY) {
				possiblePlaysByColumn[col] = space;
				break;
			}
			space += 7;
		}
	}
}

vector<int> Connect4::getPossiblePlays() {
	possiblePlays.clear();
	int space;
//		for (int col = 0; col < 7; col++) {
	for (int col : {3, 2, 4, 1, 5, 0, 6}) {
		space = col;
		while (space < 42) {
			if (board[space] == SPACE_EMPTY) {
				possiblePlays.push_back(space);
				break;
			}
			space += 7;
		}
	}
	return possiblePlays;
}


void Connect4::play(int space) {
	board[space] = turn;
	turn *= -1;
	moveHistory.push_back(space);
}
void Connect4::undoPlay() {
	int lastMove = moveHistory.back();
	moveHistory.pop_back();
	board[lastMove] = SPACE_EMPTY;
	turn *= -1;
}

int Connect4::getState() {
	totalPiecesScore = 0;
	totalSpacesScore = 0;
	for (vector<int> &diagonal : connectionLines) {
		scoringPlayer = 0; piecesScore = 0; spacesScore = 0; adjacentPieces = 0; adjacentSpaces = 0;
		for (int spaceIndex : diagonal) {
			int spaceValue = board[spaceIndex];
			if (spaceValue == 0) {
				spacesScore++;
				adjacentSpaces++;
				adjacentPieces = 0;
			} else {
				if (spaceValue != scoringPlayer) {
					if (spacesScore + piecesScore >= 4) {
						totalPiecesScore += piecesScore*scoringPlayer;
						totalSpacesScore += spacesScore*scoringPlayer;
					}
					scoringPlayer = spaceValue;
					piecesScore = 0;
					adjacentPieces = 0;
					spacesScore = adjacentSpaces;
				}
				piecesScore++;
				adjacentPieces++;
				adjacentSpaces = 0;
				if (adjacentPieces == 4) {
					return scoringPlayer;
				}
			}
		}
		if (spacesScore + piecesScore >= 4) {
			totalPiecesScore += piecesScore*scoringPlayer;
			totalSpacesScore += spacesScore*scoringPlayer;
		}
	}

	bool freeSpace = false;
	for (int space : board) {
		if (space == SPACE_EMPTY) {
			freeSpace = true;
		}
	}
	if (freeSpace == false) return GS_DRAW;
	return GS_ACTIVE;
}



