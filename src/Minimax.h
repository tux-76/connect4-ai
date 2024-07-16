/*
 * Minimax.h
 *
 *  Created on: Jul 15, 2024
 *      Author: tux76
 */

#ifndef MINIMAX_H_
#define MINIMAX_H_

#include <string>
#include <cmath>
using namespace std;

#include "TimeIt.h"
#include "Game.h"

#define MAX_GS_VALUE 3

class AI {
private:
	TimeIt timer;
public:
	int nodesProcessed;
	int processingDepth;
	int spaceWeight;
	string name;
	AI (int _processingDepth, float _spaceWeight, string _name = "");

	static float positiveSigmoid(float x);
	static float fullSigmoid(float x);

	float minimax(Connect4 &game, int depth, float alpha = MAX_GS_VALUE*-1, float beta = MAX_GS_VALUE);

	int selectPlay(Connect4 &game);

	double processingTime;
	int suggestedDepth=8;
	int nodesPerSecond;
	float goalTime;
	int selectPlayDynamic(Connect4 &game);
};


#endif /* MINIMAX_H_ */
