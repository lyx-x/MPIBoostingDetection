/*
 * Adaboost.cpp
 *
 *  Created on: May 5, 2015
 *      Author: lyx
 */

#include "Adaboost.h"

namespace Adaboost {

int Dirac(int h, int c) {
	return h == c ? 0 : 1;
}

void Normalize(double* lambda, int n) {
	double norm = 0;
	for (int i = 0 ; i < n ; i++)
		norm += lambda[i] * lambda[i];
	norm = sqrt(norm);
	for (int i = 0 ; i < n ; i++)
		lambda[i] /= norm;
}

//Constructors

Adaboost::Adaboost() {
	n = negCount + posCount;
	N = 0;
	lambda = NULL;
	alpha = NULL;
	w1 = NULL;
	w2 = NULL;
	feature = NULL;
	error = NULL;
	localError = NULL;
}

Adaboost::Adaboost(int i) {
	n = negCount + posCount;
	N = i;
	lambda = new double[n];
	alpha = new double[N];
	w1 = new double[N];
	w2 = new double[N];
	feature = new int[N];
	error = new double[Image::FeatureSize()];
	localError = new double[Image::FeatureSize()];
}

Adaboost::~Adaboost() {
	delete[] lambda;
	delete[] alpha;
	delete[] w1;
	delete[] w2;
	delete[] feature;
	delete[] error;
	delete[] localError;
}

//Public Methods

void Adaboost::SetTheta(double t) {
	theta = t;
}

double Adaboost::GetTheta() {
	return theta;
}

int Adaboost::Classify(Image img) {
	double sum = 0;
	double sumAlpha = 0;
	for (int i = 0 ; i < N ; i++) {
		sum += alpha[i] * (w1[i] * img.FeatureAt(feature[i]) + w2[i]);
		sumAlpha += alpha[i];
	}
	return sum * theta >= sumAlpha ? 1 : -1;
}

//Private Static Mathods

double Adaboost::theta = 0;

//Private Methods

double Adaboost::Error(int k) {
	double sum = 0;
	for (int j = 0 ; j < n ; j++)
		sum += lambda[j] * Dirac(Classifier::Classify(Images::GetValidationAt(j), k), Images::GetValidationAt(j).Type());
	return sum;
}

void Adaboost::Iteration() {
	for (int i = 0 ; i < N ; i++)
		Iteration(i);
}

void Adaboost::Iteration(int k) {
	double initLambda = 1 / n;
	for (int j = 0 ; j < n ; j++)
		lambda[j] = initLambda;
	localError = new double[Image::FeatureSize()];
	int minIndex = -1;
	double error = DBL_MAX;
	for (int i = 0 ; i < Image::FeatureSize() ; i++) {
		localError[i] = Error(i);
		if (localError[i] < error) {
			minIndex = i;
			error = localError[i];
		}
	}
	w1[k] = Classifier::GetW1At(minIndex);
	w2[k] = Classifier::GetW2At(minIndex);
	alpha[k] = 0.5 * log(1 / error - 1);
	for (int j = 0 ; j < n ; j++)
		lambda[k] *= exp(-Images::GetValidationAt(j).Type() * alpha[k] * Classifier::Classify(Images::GetValidationAt(j), k));
	Normalize(lambda, n);
}

void Adaboost::IterationParallel() {

}

}
