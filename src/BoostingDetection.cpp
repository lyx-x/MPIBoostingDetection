/*
 ============================================================================
 Name        : BoostingDetection.cpp
 Author      : lyx
 Version     :
 Copyright   :
 Description : Boosting Detection
 ============================================================================
 */

#include "Photo.h"

using namespace std;
using namespace imageUtils;
using namespace photo;

void TestFeature();
void TestClassifier(int);
void TestAdaboost(int);
void TestPhoto();

int main(int argc, char *argv[]) {
	srand(time(NULL));
	MPI::Init(argc, argv);

	InitFeatures();
	InitImages();

	mpiUtils::rank = MPI::COMM_WORLD.Get_rank();
	mpiUtils::size = MPI::COMM_WORLD.Get_size();
	mpiUtils::InitGather(featureSize);

	classifier::InitClassifier();
	//classifier::TrainParallel();
	classifier::ReadClassifier();

	adaboost::InitAdaboost(150);
	//adaboost::Iteration();
	adaboost::ReadAdaboost();
	adaboost::theta = 0.15;

	clock_t t;
	if (mpiUtils::rank == 0) {
		journal << "Testing:" << endl;
		t = clock();
	}
	TestAdaboost(negCount + posCount);
	//TestClassifier(100);
	if (mpiUtils::rank == 0) TestPhoto();
	if (mpiUtils::rank == 0) {
		t = clock() - t;
		journal << "End of Test: " << ((float)t)/CLOCKS_PER_SEC << "seconds.\n";
	}

	adaboost::DropAdaboost();

	classifier::DropClassifier();

	mpiUtils::DropGather();

	DropImages();
	DropFeatures();

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
	clock_t t = clock();
	if (mpiUtils::rank == 0) {
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
		int answer = (classifier::Classify(img) + 1) / 2;
		summary[type][answer]++;
	}
	MPI::COMM_WORLD.Reduce(summary, globalSummary, 4, MPI_INT, MPI_SUM, 0);
	if (mpiUtils::rank == 0) {
		journal << "\tNeg\tPos\n";
		journal << "Neg\t" << globalSummary[0][0] << '\t' << globalSummary[1][0] << '\n';
		journal << "Pos\t" << globalSummary[0][1] << '\t' << globalSummary[1][1] << '\n';
		journal << "Rate: " << (double)(globalSummary[0][0] + globalSummary[1][1]) / (double)(n * mpiUtils::size) << endl;
		t = clock() - t;
		journal << "End of Classifier Test: " << ((float)t)/CLOCKS_PER_SEC << "seconds.\n\n";
	}
}

void TestAdaboost(int n) {
	ofstream out(dir + "adaboost.theta");
	adaboost::theta = -1;
	while (adaboost::theta <= 1) {
		clock_t t = clock();
		if (mpiUtils::rank == 0) {
			journal << "\nTest Adaboost: " << adaboost::theta << endl;
		}
		int globalSummary[2][2];
		int summary[2][2];
		for (int i = 0 ; i < 2 ; i++)
			for (int j = 0 ; j < 2 ; j++)
				summary[i][j] = globalSummary[i][j] = 0;
		for (int i = mpiUtils::rank ; i < n ; i += mpiUtils::size) {
			Image* img = GetTestAt(i);
			int type = (img->Type() + 1) / 2;
			int answer = (adaboost::Classify(img) + 1) / 2;
			summary[type][answer]++;
		}
		MPI::COMM_WORLD.Reduce(summary, globalSummary, 4, MPI_INT, MPI_SUM, 0);
		if (mpiUtils::rank == 0) {
			journal << "\tNeg\tPos\n";
			double correct = (double)(globalSummary[0][0] + globalSummary[1][1]) / (double)n;
			double tPrecision = (double)globalSummary[1][1] / (double)(globalSummary[0][1] + globalSummary[1][1]);
			double tRappel = (double)globalSummary[1][1] / (double)(globalSummary[1][0] + globalSummary[1][1]);
			double fScore = 2 / (1 / tPrecision + 1 / tRappel);
			journal << "Neg\t" << globalSummary[0][0] << '\t' << globalSummary[1][0] << '\n';
			journal << "Pos\t" << globalSummary[0][1] << '\t' << globalSummary[1][1] << '\n';
			journal << "Rate: " << correct << endl;
			journal << "Misclassification: " << 1 - correct << endl;
			journal << "Precision: " << tPrecision << endl;
			journal << "Recall: " << tRappel << endl;
			journal << "F-Score: " << fScore << endl;
			t = clock() - t;
			journal << "End of Adaboost Test: " << ((float)t)/CLOCKS_PER_SEC << "seconds.\n\n";
			double fauxNeg = globalSummary[1][0] / (double)(globalSummary[0][0] + globalSummary[1][0]);
			double fauxPos = globalSummary[0][1] / (double)(globalSummary[0][1] + globalSummary[1][1]);
			out << adaboost::theta << '\t' << correct << '\t' << fauxNeg << '\t' << fauxPos << endl;
		}
		adaboost::theta += 0.1;
	}
}

void TestPhoto() {
	clock_t t = clock();
	adaboost::theta = 0.05;
	//Photo p(3636, 2425, photoPath + "photo1.raw");
	//p.SetBaseDimension(224, 184, 1); // photo1;
	//Photo p(1320, 712, photoPath + "photo2.raw");
	//p.SetBaseDimension(56, 46, 1);
	Photo p(544, 432, photoPath + "photo3.raw");
	p.SetBaseDimension(56, 46, 1);
	//p.Print();
	p.FindFace();
	t = clock() - t;
	if (mpiUtils::rank == 0) {
		journal << "End of Photo Test: " << ((float)t)/CLOCKS_PER_SEC << "seconds.\n\n";
	}
}
