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

using namespace std;
using namespace imageUtils;

namespace Adaboost {

void InitAdaboost(int);
void DropAdaboost();
int Classify(Image*);
void SetTheta(double);
void Iteration(); //N times
void IterationParallel();

}

#endif /* ADABOOST_H_ */
