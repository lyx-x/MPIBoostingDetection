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
#include "Images.h"

using namespace std;
using namespace Globals;

int main(int argc, char *argv[]) {
	MPI::Init(argc, argv);
	int rank = MPI::COMM_WORLD.Get_rank();
	cout << "PID: " << rank << endl;
	InitFeatures();
	cout << featureSize << endl;
	//PrintFeaturePos();

	InitImages();
	if (rank == 0)
	{
		GetTestAt(4416)->PrintRAW();
		GetTestAt(4416)->PrintFeature();
	}
	DropImages();

	MPI::Finalize();
	return 0;
}

