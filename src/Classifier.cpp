/*
 * Classifier.cpp
 *
 *  Created on: May 5, 2015
 *      Author: lyx
 */

#include "Classifier.h"

namespace classifier {

double epsilon(0.5);
double* w1 = NULL;
double* w2 = NULL;

void InitClassifier() {
	w1 = new double[featureSize];
	w2 = new double[featureSize];
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

void Train(int index) {
	w1[index] = 1;
	w2[index] = 0;
	for (int i = 0 ; i < negCount + posCount ; i++) {
		int choice = i; //No longer random
		Image* img = GetTrainAt(choice);
		int x = img->FeatureAt(index);
		int h = Classify(img, index);
		w1[index] -= epsilon * (h - img->Type()) * x;
		w2[index] -= epsilon * (h - img->Type());
	}
}

void Train() {
	clock_t t;
	t = clock();
	for (int index = 0 ; index < featureSize ; index++)
		Train(index);
	t = clock() - t;
	journal << "Computing classifiers locally " << negCount + posCount << " times: " << ((float)t)/CLOCKS_PER_SEC << "seconds.\n";
}

void TrainParallel() {
	clock_t t;
	t = clock();
	double* globalW1 = new double[featureSize];
	double*	globalW2 = new double[featureSize];
	/*
	 * Reduce Method
	 */
	/*
	for (int i = 0 ; i < featureSize ; i++) {
		w1[i] = numeric_limits<double>::max();
		w2[i] = numeric_limits<double>::max();
	}
	for (int index = rank ; index < featureSize ; index += size)
		Train(K, index);
	MPI::COMM_WORLD.Reduce(w1, globalW1, featureSize, MPI_DOUBLE, MPI_MIN, 0);
	MPI::COMM_WORLD.Reduce(w2, globalW2, featureSize, MPI_DOUBLE, MPI_MIN, 0);
	*/
	/*
	 * Gather Method
	 */
	for (int i = 0 ; i < recvCounts[mpiUtils::rank] ; i++)
		Train(i + displs[mpiUtils::rank]);
	MPI::COMM_WORLD.Gatherv(w1 + displs[mpiUtils::rank], recvCounts[mpiUtils::rank], MPI_DOUBLE, globalW1, recvCounts, displs, MPI_DOUBLE, 0);
	MPI::COMM_WORLD.Gatherv(w2 + displs[mpiUtils::rank], recvCounts[mpiUtils::rank], MPI_DOUBLE, globalW2, recvCounts, displs, MPI_DOUBLE, 0);
	if (mpiUtils::rank == 0)
		for (int i = 0 ; i < featureSize ; i++) {
			w1[i] = globalW1[i];
			w2[i] = globalW2[i];
		}
	MPI::COMM_WORLD.Bcast(w1, featureSize, MPI_DOUBLE, 0);
	MPI::COMM_WORLD.Bcast(w2, featureSize, MPI_DOUBLE, 0);
	delete[] globalW1;
	delete[] globalW2;
	PrintClassifier();
	t = clock() - t;
	journal << "Computing classifiers parallelly " << posCount + negCount << " times: " << ((float)t)/CLOCKS_PER_SEC << "seconds.\n";
}

void ReadClassifier() {
	ifstream in(dir + "classifier.jrl");
	for (int i = 0 ; i < featureSize ; i++) {
		int tmp;
		in >> tmp >> w1[i] >> w2[i];
	}
	in.close();
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

void PrintClassifier() {
	ofstream out(dir + "classifier.jrl");
	for (int i = 0 ; i < featureSize ; i++) {
		out << i << '\t';
		out << w1[i] << '\t' << w2[i] << endl;
	}
	out.close();
}

void DropClassifier() {
	delete[] w1;
	delete[] w2;
}

}
