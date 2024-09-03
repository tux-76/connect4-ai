/*
 * Minimax.cpp
 *
 *  Created on: Jul 15, 2024
 *      Author: tux76
 */

#include "connect4_ai/minimax.h"
using namespace Connect4AI;

AI::AI (int _processingDepth, float _spaceWeight, string _name) {
	nodesProcessed = 0;
	processingDepth = _processingDepth;
	spaceWeight = _spaceWeight;

	nodesPerSecond = 0;
	suggestedDepth = _processingDepth;
	processingTime = 0;
	goalTime = -1;
	name = _name;
}

float AI::positiveSigmoid(float x) {
	return 1-exp(-(x/10));
}
float AI::fullSigmoid(float x) {
	return 2/(1+exp(-(x/10)))-1;
}

float AI::minimax(Connect4 &game, int depth, float alpha, float beta) {
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


int AI::selectPlay(Connect4 &game) {
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
	return bestPlay;
}

int AI::selectPlayDynamic(Connect4 &game) {
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


