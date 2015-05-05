/*
 * Classifier.cpp
 *
 *  Created on: May 5, 2015
 *      Author: lyx
 */

#include "Classifier.h"

namespace Classifier {

double epsilon(0.5);
double* localW1 = new double[Image::FeatureSize()];;
double* localW2 = new double[Image::FeatureSize()];;
double* w1 = new double[Image::FeatureSize()];;
double* w2 = new double[Image::FeatureSize()];;

void Init() {
	for (int i = 0 ; i < Image::FeatureSize() ; i++) {
		localW1[i] = 1;
		localW2[i] = 0;
		w1[i] = 1;
		w2[i] = 0;
	}
}

int Classify(Image img, int index) {
	return w1[index] * img.FeatureAt(index) + w2[index] >= 0 ? 1 : -1;
}

void Train(int K) {
	for (int index = 0 ; index < Image::FeatureSize() ; index++)
		Train(K, index);
}

void TrainParallel(int K) {

}

double GetW1At(int index) {
	return w1[index];
}

double GetW2At(int index) {
	return w2[index];
}

void SetEpsilon(double e) {
	epsilon = e;
}

void Print() {
	for (int i = 0 ; i < Image::FeatureSize() ; i++) {
		Image::PrintFeature(i);
		cout << "w1: " << w1[i] << "\tw2: " << w2[i] << endl;
	}
}

void Delete() {
	delete[] localW1;
	delete[] localW2;
	delete[] w1;
	delete[] w2;
}

void Train(int K, int index) {
	for (int i = 0 ; i < K ; i++) {
		int choice = rand() % (posCount + negCount);
		Image img = Images::GetTrainAt(choice);
		int x = img.FeatureAt(index);
		int h = Classify(img, index);
		localW1[index] -= epsilon * (h - img.Type()) * x;
		localW2[index] -= epsilon * (h - img.Type());
	}
}

}
