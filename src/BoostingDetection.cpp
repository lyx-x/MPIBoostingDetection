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
#include "Image.h"

using namespace std;
using namespace Globals;

int main(int argc, char *argv[]) {
	MPI::Init(argc, argv);
	int rank = MPI::COMM_WORLD.Get_rank();
	cout << "PID: " << rank << endl;
	Image::Init(defaultWidth, defaultHeight);
	//cout << Image::FeatureSize() << endl;
	//Image::PrintFeaturePos();
	string _path = "../app/neg/im0.raw";
	Image img(defaultWidth, defaultHeight, _path); //don't launch the application with Eclipse, wrong path
	if (rank == 0)
	{
		img.PrintRAW();
		cout << Image::FeatureSize() << endl;
		img.PrintFeature();
	}
	MPI::Finalize();
	return 0;
}

