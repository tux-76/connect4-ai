/*
 * Connect4.h
 *
 *  Created on: Jul 15, 2024
 *      Author: tux76
 */

#ifndef GAME_H_
#define GAME_H_

#include <vector>
using namespace std;
#include <unordered_map>

#define GS_ACTIVE 2
#define GS_DRAW 0
#define GS_MAX_WIN 1
#define GS_MIN_WIN -1
#define SPACE_EMPTY 0
#define SPACE_X 1
#define SPACE_O -1

namespace Connect4AI {

const int L_DIAGONAL_START_POINTS[6]  = {0, 1, 2, 3, 7, 14};
const int R_DIAGONAL_START_POINTS[6] = {3, 4, 5, 6, 13, 20};

class Connect4 {
private:
	vector< vector< int > > connectionLines;
	void getConnectionLines ();

	int scoringPlayer, piecesScore, spacesScore, adjacentPieces, adjacentSpaces;

public:
	int board[42];
	int turn;
	vector<int> moveHistory;

	Connect4(int _board[42]);

	int possiblePlaysByColumn[7];
	void calcPossiblePlaysByColumn();

	vector<int> possiblePlays;
	vector<int> getPossiblePlays();


	void play(int space);
	void undoPlay();

	int totalPiecesScore, totalSpacesScore;
	int getState();
};

} /* Connect4AI */
#endif /* GAME_H_ */
