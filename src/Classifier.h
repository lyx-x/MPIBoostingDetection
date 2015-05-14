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
#include "mpi.h"

using namespace std;
using namespace imageUtils;
using namespace mpiUtils;

namespace Classifier {

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
