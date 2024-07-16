/*
 * TimeIt.h
 *
 *  Created on: Jul 15, 2024
 *      Author: tux76
 */

#ifndef TIMEIT_H_
#define TIMEIT_H_

#include <iostream>
#include <ctime>

class TimeIt {
public:
	time_t startT, endT;
	void start ();
	double stop (bool print=true);
};


#endif /* TIMEIT_H_ */
