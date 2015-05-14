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
	w1[index] = 1;
	w2[index] = 0;
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
	int* recvCounts = new int[size];
	int* displs = new int[size];
	int length = (featureSize + size - 1) / size;
	for (int i = 0 ; i < size ; i++) {
		recvCounts[i] = length;
		displs[i] = length * i;
	}
	recvCounts[size - 1] = featureSize - displs[size - 1];
	for (int i = 0 ; i < recvCounts[rank] ; i++)
		Train(K, i + displs[rank]);
	MPI::COMM_WORLD.Gatherv(w1 + displs[rank], recvCounts[rank], MPI_DOUBLE, globalW1, recvCounts, displs, MPI_DOUBLE, 0);
	MPI::COMM_WORLD.Gatherv(w2 + displs[rank], recvCounts[rank], MPI_DOUBLE, globalW2, recvCounts, displs, MPI_DOUBLE, 0);
	if (rank == 0)
		for (int i = 0 ; i < featureSize ; i++) {
			w1[i] = globalW1[i];
			w2[i] = globalW2[i];
		}
	MPI::COMM_WORLD.Bcast(w1, featureSize, MPI_DOUBLE, 0);
	MPI::COMM_WORLD.Bcast(w2, featureSize, MPI_DOUBLE, 0);
	delete[] recvCounts;
	delete[] displs;
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
	ofstream out(dir + "classifier.pos");
	for (int i = 0 ; i < featureSize ; i++) {
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
