/*
 * Adaboost.cpp
 *
 *  Created on: May 5, 2015
 *      Author: lyx
 */

#include "Adaboost.h"

namespace Adaboost {

double theta = 0;
int n = 0;
int N = 0;
double* dist = NULL;
double* lambda = NULL;
double* alpha = NULL;
double* w1 = NULL;
double* w2 = NULL;
int* feature = NULL;

int Dirac(int h, int c) {
	return h == c ? 0 : 1;
}

void Normalize(double* lambda, int n) {
	double norm = 0;
	for (int i = 0 ; i < n ; i++)
		norm += lambda[i];
	for (int i = 0 ; i < n ; i++)
		lambda[i] /= norm;
}

double ErrorParallel(int k) {
	double sum = 0;
	double localSum = 0;
	for (int j = mpiUtils::rank ; j < n ; j += mpiUtils::size) {
		Image* img = GetValidationAt(j);
		localSum += lambda[j] * Dirac(Classifier::Classify(img, k), img->Type());
	}
	MPI::COMM_WORLD.Allreduce(&localSum, &sum, 1, MPI_DOUBLE, MPI_SUM);
	return sum;
}

double Error(int k) {
	double sum = 0;
	for (int j = 0 ; j < n ; j++) {
		Image* img = GetValidationAt(j);
		sum += lambda[j] * Dirac(Classifier::Classify(img, k), img->Type());
	}
	return sum;
}

void InitAdaboost(int i) {
	n = negCount + posCount;
	N = i;
	lambda = new double[n];
	dist = new double[featureSize];
	alpha = new double[N];
	w1 = new double[N];
	w2 = new double[N];
	feature = new int[N];
}

void ReadAdaboost() {
	ifstream in(dir + "adaboost.pos");
	double tmp;
	for (int i = 0 ; i < N ; i++)
		in >> feature[i] >> tmp >> w1[i] >> w2[i] >> alpha[i];
	journal << "Read Adaboost " << N << " times\n";
}

void DropAdaboost() {
	delete[] lambda;
	delete[] dist;
	delete[] alpha;
	delete[] w1;
	delete[] w2;
	delete[] feature;
}

int Classify(Image* img) {
	double sum = 0;
	double sumAlpha = 0;
	for (int i = 0 ; i < N ; i++) {
		sum += alpha[i] * (w1[i] * img->FeatureAt(feature[i]) + w2[i]);
		sumAlpha += alpha[i];
	}
	return sum >= sumAlpha * theta ? 1 : -1;
}

void SetTheta(double t) {
	theta = t;
}

void Iteration(int k) {
	double initLambda = (double)1 / (double)n;
	for (int j = 0 ; j < n ; j++)
		lambda[j] = initLambda;
	int minIndex = -1;
	double errorLimit = numeric_limits<double>::max();
	for (int i = 0 ; i < recvCounts[mpiUtils::rank] ; i++)
		dist[i + displs[mpiUtils::rank]] = Error(i + displs[mpiUtils::rank]);
	double* globalDist = new double[featureSize];
	MPI::COMM_WORLD.Gatherv(dist + displs[mpiUtils::rank], recvCounts[mpiUtils::rank], MPI_DOUBLE, globalDist, recvCounts, displs, MPI_DOUBLE, 0);
	if (mpiUtils::rank == 0)
		for (int i = 0 ; i < featureSize ; i++)
			dist[i] = globalDist[i];
	delete[] globalDist;
	MPI::COMM_WORLD.Bcast(dist, featureSize, MPI_DOUBLE, 0);
	for (int i = 0 ; i < featureSize ; i++)
		if (dist[i] < errorLimit) {
			minIndex = i;
			errorLimit = dist[i];
		}
	w1[k] = Classifier::GetW1At(minIndex);
	w2[k] = Classifier::GetW2At(minIndex);
	feature[k] = minIndex;
	alpha[k] = 0.5 * log(1.0 / errorLimit - 1);
	for (int j = 0 ; j < n ; j++)
		lambda[j] *= exp(-GetValidationAt(j)->Type() * alpha[k] * Classifier::Classify(GetValidationAt(j), k));
	Normalize(lambda, n);
}

void Iteration() {
	clock_t t;
	t = clock();
	for (int i = 0 ; i < N ; i++) {
		Iteration(i);
	}
	t = clock() - t;
	journal << "Computing Adaboost locally " << N << " times: " << ((float)t)/CLOCKS_PER_SEC << "seconds.\n";
}

void PrintAdaboost() {
	ofstream out(dir + "adaboost.pos");
	for (int i = 0 ; i < N ; i++)
		out << feature[i] << '\t' << featurePos[feature[i]] << '\t' << w1[i] << '\t' << w2[i] << '\t' << alpha[i] << endl;
}

}
