/*
 ============================================================================
 Name        : BoostingDetection.cpp
 Author      : lyx
 Version     :
 Copyright   :
 Description : Boosting Detection
 ============================================================================
 */

#include "mpi.h" 
#include <iostream>
#include "Adaboost.h"
#include "Classifier.h"

using namespace std;
using namespace imageUtils;

void TestFeature();
void TestClassifier(int, int);

int main(int argc, char *argv[]) {
	srand(time(NULL));
	MPI::Init(argc, argv);
	int rank = MPI::COMM_WORLD.Get_rank();

	InitFeatures();
	InitImages();

	Classifier::InitClassifier();
	Classifier::TrainParallel(500);

	clock_t t;
	if (rank == 0) {
		journal << "Testing:" << endl;
		t = clock();
	}
	TestClassifier(500, rank);
	if (rank == 0) {
		t = clock() - t;
		journal << "End of Test: " << ((float)t)/CLOCKS_PER_SEC << "seconds.\n";
	}

	DropImages();
	Classifier::DropClassifier();
	error.close();
	journal.close();
	MPI::Finalize();
	return 0;
}

void TestFeature() {
	journal << "Feature Size: " << featureSize << endl;
	PrintFeaturePos();
}

/*
 * Summary:
 * 		j	0	1
 * 	i		Neg	Pos (real)
 * 	0	Neg
 * 	1	Pos
 */

void TestClassifier(int n, int rank) {
	if (rank == 0) {
		Classifier::Print();
		journal << "\nTest Classifier\n";
	}
	int globalSummary[2][2];
	int summary[2][2];
	for (int i = 0 ; i < 2 ; i++)
		for (int j = 0 ; j < 2 ; j++)
			summary[i][j] = globalSummary[i][j] = 0;
	for (int i = 0 ; i < n ; i++) {
		int choice = RandomImage();
		Image* img = GetTestAt(choice);
		int type = (img->Type() + 1) / 2;
		int answer = (Classifier::Classify(img) + 1) / 2;
		summary[type][answer]++;
	}
	MPI::COMM_WORLD.Reduce(summary, globalSummary, 4, MPI_INT, MPI_SUM, 0);
	if (rank == 0) {
		journal << "\tNeg\tPos\n";
		journal << "Neg\t" << globalSummary[0][0] << '\t' << globalSummary[0][1] << '\n';
		journal << "Pos\t" << globalSummary[1][0] << '\t' << globalSummary[1][1] << '\n';
		journal << "Rate: " << (double)(globalSummary[0][0] + globalSummary[1][1]) / (double)(n * MPI::COMM_WORLD.Get_size()) << endl << endl;
	}
}
