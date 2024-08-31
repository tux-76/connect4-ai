/*
 * TimeIt.cpp
 *
 *  Created on: Jul 15, 2024
 *      Author: tux76
 */

#include "connect4_ai/timeit.h"
using namespace Connect4AI;

void TimeIt::start () {
	startT=clock();
}
double TimeIt::stop (bool print) {
	endT=clock();
	double time = double((endT-startT))/CLOCKS_PER_SEC;
	if (print) std::cout << time << " sec" << std::endl;
	return time;
}
