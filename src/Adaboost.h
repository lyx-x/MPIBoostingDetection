/*
 * Adaboost.h
 *
 *  Created on: May 5, 2015
 *      Author: lyx
 */

#ifndef ADABOOST_H_
#define ADABOOST_H_

#include "Classifier.h"
#include <cmath>
#include <cfloat>

using namespace std;
using namespace imageUtils;

namespace Adaboost {

class Adaboost {
public:
	Adaboost();
	Adaboost(int);
	virtual ~Adaboost();
	void SetTheta(double);
	double GetTheta();
	int Classify(Image);
private:
	static double theta;

	int n; //image count
	int N; //iteration times
	double* error;
	double* localError;
	double* lambda;
	double* alpha;
	double* w1;
	double* w2;
	int* feature;
	double Error(int);
	void Iteration(); //N times
	void Iteration(int);
	void IterationParallel();
};

}

#endif /* ADABOOST_H_ */
