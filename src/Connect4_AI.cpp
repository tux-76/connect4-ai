//============================================================================
// Name        : Connect4_AI.cpp
// Author      : tux76
//============================================================================

#include <iostream>
using namespace std;
#include <vector>
#include <cmath>
#include <cstdlib>
#include <unordered_map>

#define GS_ACTIVE 2
#define GS_DRAW 0
#define GS_MAX_WIN 1
#define GS_MIN_WIN -1

#define SPACE_EMPTY 0
#define SPACE_X 1
#define SPACE_O -1

// I do one higher than max for good measure
#define MAX_GS_VALUE 3

const int L_DIAGONAL_START_POINTS[6]  = {0, 1, 2, 3, 7, 14};
const int R_DIAGONAL_START_POINTS[6] = {3, 4, 5, 6, 13, 20};

class TimeIt {
public:
	time_t startT, endT;
	void start () {
		startT=clock();
	}
	double stop (bool print=true) {
		endT=clock();
		double time = double((endT-startT))/CLOCKS_PER_SEC;
		if (print) cout << time << " sec" << endl;
		return time;
	}
};

// GAME
// =================================
class Connect4 {
private:
	vector<vector<int>> connectionLines;
	void getConnectionLines () {
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

	int scoringPlayer, piecesScore, spacesScore, adjacentPieces, adjacentSpaces;

public:
	int board[42];
	int turn;
	vector<int> moveHistory;

	Connect4(int _board[42]) {
		turn = 1;
		for (int i = 0; i < 42; i++) {
			board[i] = _board[i];
		}
		getConnectionLines();
	}

	int possiblePlaysByColumn[7];
	void calcPossiblePlaysByColumn() {
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

	vector<int> possiblePlays;
	vector<int> getPossiblePlays() {
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


	void play(int space) {
		board[space] = turn;
		turn *= -1;
		moveHistory.push_back(space);
	}
	void undoPlay() {
		int lastMove = moveHistory.back();
		moveHistory.pop_back();
		board[lastMove] = SPACE_EMPTY;
		turn *= -1;
	}

	int totalPiecesScore, totalSpacesScore;
	int getState() {
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
};

// MINIMAX
// =================================
class AI {
private:
	TimeIt timer;
public:
	int nodesProcessed;
	int processingDepth;
	int spaceWeight;
	string name;
	AI (int _processingDepth, float _spaceWeight, string _name = "") {
		nodesProcessed = 0;
		processingDepth = _processingDepth;
		spaceWeight = _spaceWeight;

		nodesPerSecond = 0;
		suggestedDepth = _processingDepth;
		processingTime = 0;
		goalTime = -1;
		name = _name;
	}

	static float positiveSigmoid(float x) {
		return 1-exp(-(x/10));
	}
	static float fullSigmoid(float x) {
		return 2/(1+exp(-(x/10)))-1;
	}

	float minimax(Connect4 &game, int depth, float alpha = MAX_GS_VALUE*-1, float beta = MAX_GS_VALUE) {
		nodesProcessed++;
		// Check if leaf node

		int gameState = game.getState();
		if (gameState != GS_ACTIVE) {
			if (gameState == GS_DRAW) return gameState;
			else return gameState*(1+positiveSigmoid(depth));
		} else if (depth < 1) {
			return fullSigmoid(game.totalPiecesScore + game.totalSpacesScore*spaceWeight);
//			return fullSigmoid(rand()%32);
		}

		// Get best value of all children of possible plays
		vector<int> possiblePlays = game.getPossiblePlays();
//		for (int play : possiblePlays) {
//			cout << play << " ";
//		}
//		cout << endl;
		float bestValue;
		if (game.turn == 1) { // MAXIMIZER
			bestValue = MAX_GS_VALUE*-1;
			float childValue;

			for (int& play : possiblePlays) {
				game.play(play);

				childValue = minimax(game, depth-1, alpha, beta);
				if (childValue > bestValue) bestValue = childValue;
				if (childValue > alpha) alpha = childValue;

				game.undoPlay();
				if (alpha > beta) break;
			}
		}
		else { // MINIMIZER
			bestValue = MAX_GS_VALUE;
			float childValue;
			for (int& play : possiblePlays) {
				game.play(play);

				childValue = minimax(game, depth-1, alpha, beta);
				if (childValue < bestValue) bestValue = childValue;
				if (childValue < beta) beta = childValue;

				game.undoPlay();
				if (beta < alpha) break;
			}
		}
		return bestValue;
	}


	int selectPlay(Connect4 &game) {
		int player = game.turn;
		int bestPlay = -1;
		float bestValue = game.turn*-1 * MAX_GS_VALUE;
		float alpha = MAX_GS_VALUE*-1;
		float beta = MAX_GS_VALUE;

		float value;
		vector<int> possiblePlays = game.getPossiblePlays();
		for (int& play : possiblePlays) {
			game.play(play);

			value = minimax(game, processingDepth-1, alpha, beta);
			if (value > bestValue && player == 1) {
				bestValue = value;
				bestPlay = play;
				if (value > alpha) alpha = value;
				if (alpha > beta) break;
			} else if (value < bestValue && player == -1) {
				bestValue = value;
				bestPlay = play;
				if (value < beta) beta = value;
				if (beta < alpha) break;
			}

//			cout << play << ":" << value << endl;
			game.undoPlay();
		}
		cout << "Selected: " << bestPlay % 7 << endl;
		cout << "Predicted Outcome: " << bestValue << endl;
		return bestPlay;
	}

	double processingTime;
	int suggestedDepth=8;
	int nodesPerSecond;
	float goalTime;
	int selectPlayDynamic(Connect4 &game) {
		cout << name << " (AI) SELECTION" << endl;
		cout << "Depth: " << processingDepth << endl;
		nodesProcessed = 0;
		timer.start();
		int play = selectPlay(game);
		processingTime = timer.stop(false);

		// Depth suggestion
		int lastSuggestedDepth = suggestedDepth;
		goalTime = 1;
		nodesPerSecond = nodesProcessed / processingTime;
		suggestedDepth = round(log(nodesPerSecond*goalTime) / log(pow(nodesProcessed, 1.0/processingDepth)));
		if (suggestedDepth < 0 || suggestedDepth > 9999) suggestedDepth = 9999;

		if (suggestedDepth <= lastSuggestedDepth) {
			processingDepth = suggestedDepth;
		} else {
			processingDepth = lastSuggestedDepth;
		}

		cout << "Time: " << processingTime << endl;
		cout << "Processed: " << nodesProcessed << endl;
		cout << "Nodes per second: " << nodesPerSecond << endl;
		cout << "Suggested depth: " << suggestedDepth << " (Last: " << lastSuggestedDepth << ")" << endl;

		return play;
	}
};

// INTERFACE
// =================================
class Interface {
public:
	static char getSpaceChar(int space) {
		if (space==0) return '-';
		else if (space==1) return 'x';
		else if (space==-1) return 'o';
		return '?';
	}

	static void printGame(Connect4 &game, bool oneLine = 0) {
		cout << "6 5 4 3 2 1 0" << endl;
		for (int i = 41; i>=0; i--) {
			cout << getSpaceChar(game.board[i]) << " ";
			if (i % 7 == 0 && !oneLine) {
				cout << endl;
			}
		}
		cout << "Next: " << getSpaceChar(game.turn) << endl;
	}

	static void printState(Connect4 &game, AI &computer, TimeIt &timer) {
		timer.start();
		game.getState();
		cout << "PV:" << game.totalPiecesScore << " SpV:" << game.totalSpacesScore << endl;
		cout << "Predicted value: " << computer.minimax(game, computer.processingDepth) << endl;
		cout << "Time: ";
		timer.stop();
	}
	static void printInfo(Connect4 &game) {
		cout << "Move History: " << endl;
		for (int play : game.moveHistory) {
			cout << play << ',';
		}
		cout << endl;
	}

	static bool play(Connect4 &game, AI &computerX, AI &computerY) {
		TimeIt timer;

		string input;
		int play;
		int gameState = 2;
		AI* computer = &computerX;
		while (true) {
			cout << endl;

			gameState = game.getState();
			printGame(game);
//			for (int play : game.getPossiblePlays()) {
//				cout << play << " ";
//			}
//			cout << endl;

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

			if (input == "exit") break;
			else if (input == "-") {
				game.undoPlay();
				continue;
			}
			else if (input == "r") {
				printInfo(game);
				return 1;
			}
			else if (input == "st") {
				printState(game, *computer, timer);
				continue;
			}
			else if (input == "i") {
				printInfo(game);
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
};


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

		running = Interface::play(game, computerX, computerY);
	}
}
