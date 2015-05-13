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
using namespace Globals;

void TestFeature();
void TestClassifier(int);

int main(int argc, char *argv[]) {
	srand(time(NULL));
	MPI::Init(argc, argv);
	int rank = MPI::COMM_WORLD.Get_rank();

	InitFeatures();
	InitImages();

	Classifier::InitClassifier();
	Classifier::TrainParallel(200);

	if (rank == 0)
	{
		journal << "Testing:" << endl;
		clock_t t;
		t = clock();
		TestClassifier(200);
		t = clock() - t;
		journal << "End of Test: " << ((float)t)/CLOCKS_PER_SEC << "seconds.\n";
	}

	DropImages();
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

void TestClassifier(int n) {
	Classifier::Print();
	journal << "\nTest Classifier\n";
	int summary[2][2];
	for (int i = 0 ; i < 2 ; i++)
		for (int j = 0 ; j < 2 ; j++)
			summary[i][j] = 0;
	for (int i = 0 ; i < n ; i++) {
		int choice = RandomImage();
		Image* img = GetTestAt(choice);
		int type = (img->Type() + 1) / 2;
		int answer = (Classifier::Classify(img) + 1) / 2;
		summary[type][answer]++;
	}
	journal << "\tNeg\tPos\n";
	journal << "Neg\t" << summary[0][0] << '\t' << summary[0][1] << '\n';
	journal << "Pos\t" << summary[1][0] << '\t' << summary[1][1] << '\n';
	journal << "Rate: " << (double)(summary[0][0] + summary[1][1]) / (double)n << endl << endl;
}
