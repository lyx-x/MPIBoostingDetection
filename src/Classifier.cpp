/*
 * Classifier.cpp
 *
 *  Created on: May 5, 2015
 *      Author: lyx
 */

#include "Classifier.h"

namespace Classifier {

double epsilon(0.5);
double* w1 = NULL;
double* w2 = NULL;

void InitClassifier() {
	w1 = new double[featureSize];
	w2 = new double[featureSize];
	for (int i = 0 ; i < featureSize ; i++) {
		w1[i] = 1;
		w2[i] = 0;
	}
}

int Classify(Image* img, int index) {
	return w1[index] * (img->FeatureAt(index)) + w2[index] >= 0 ? 1 : -1;
}

int Classify(Image* img) {
	int sum = 0;
	for (int i = 0 ; i < featureSize ; i++) {
		sum += Classify(img, i);
	}
	return sum >= 0 ? 1 : -1;
}

void Train(int K, int index) {
	for (int i = 0 ; i < K ; i++) {
		int choice = RandomImage();
		Image* img = GetTrainAt(choice);
		int x = img->FeatureAt(index);
		int h = Classify(img, index);
		w1[index] -= epsilon * (h - img->Type()) * x;
		w2[index] -= epsilon * (h - img->Type());
	}
}

void Train(int K) {
	clock_t t;
	t = clock();
	for (int index = 0 ; index < featureSize ; index++)
		Train(K, index);
	t = clock() - t;
	journal << "Computing classifiers locally " << K << " times: " << ((float)t)/CLOCKS_PER_SEC << "seconds.\n";
}

void TrainParallel(int K) {
	clock_t t;
	t = clock();
	int rank = MPI::COMM_WORLD.Get_rank();
	int size = MPI::COMM_WORLD.Get_size();
	double* globalW1 = new double[featureSize];
	double*	globalW2 = new double[featureSize];
	for (int i = 0 ; i < featureSize ; i++) {
		globalW1[i] = numeric_limits<double>::min();
		globalW2[i] = numeric_limits<double>::min();
	}
	for (int index = rank ; index < featureSize ; index += size)
		Train(K, index);
	MPI::COMM_WORLD.Reduce(w1, globalW1, featureSize, MPI_DOUBLE, MPI_MAX, 0);
	MPI::COMM_WORLD.Reduce(w2, globalW2, featureSize, MPI_DOUBLE, MPI_MAX, 0);
	for (int i = 0 ; i < featureSize ; i++) {
		w1[i] = globalW1[i];
		w2[i] = globalW2[i];
	}
	if (rank != 0) {
		delete[] w1;
		delete[] w2;
	}
	delete[] globalW1;
	delete[] globalW2;
	t = clock() - t;
	journal << "Computing classifiers locally " << K << " times: " << ((float)t)/CLOCKS_PER_SEC << "seconds.\n";
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
	ofstream out("classifier.pos");
	for (int i = 0 ; i < featureSize ; i++) {
		//PrintFeature(i);
		out << i << '\t';
		out << "w1: " << w1[i] << "\t\tw2: " << w2[i] << endl;
	}
	out.close();
}

void DropClassifier() {
	delete[] w1;
	delete[] w2;
}

}
