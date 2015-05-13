/*
 * Classifier.h
 *
 *  Created on: May 5, 2015
 *      Author: lyx
 */

#ifndef CLASSIFIER_H_
#define CLASSIFIER_H_

#include "Images.h"
#include <limits>

using namespace std;
using namespace Globals;

namespace Classifier {

extern double epsilon;
extern double* w1;
extern double* w2;

void InitClassifier();
int Classify(Image*, int); //function h(x)
int Classify(Image*);
void Train(int);
void TrainParallel(int);
double GetW1At(int);
double GetW2At(int);
void SetEpsilon(double);
void Print();
void DropClassifier();

}

#endif /* CLASSIFIER_H_ */
