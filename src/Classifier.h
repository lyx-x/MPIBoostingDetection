/*
 * Classifier.h
 *
 *  Created on: May 5, 2015
 *      Author: lyx
 */

#ifndef CLASSIFIER_H_
#define CLASSIFIER_H_

#include "Images.h"
#include <cstdlib>

using namespace std;
using namespace Globals;

namespace Classifier {

void Init();
int Classify(Image*, int); //function h(x)
void Train(int);
void TrainParallel(int);
double GetW1At(int);
double GetW2At(int);
void SetEpsilon(double);
void Print();
void Delete();

extern double epsilon;
extern double* localW1;
extern double* localW2;
extern double* w1;
extern double* w2;
void Train(int, int); //times and index of feature

}

#endif /* CLASSIFIER_H_ */
