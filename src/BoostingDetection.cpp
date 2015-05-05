/*
 ============================================================================
 Name        : BoostingDetection.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Compute Pi in MPI C++
 ============================================================================
 */
#include "mpi.h" 
#include <iostream>
#include "Image.h"
using namespace std;

const int width = 112;
const int height = 92;

int main(int argc, char *argv[]) {
	MPI::Init(argc, argv);
	int rank = MPI::COMM_WORLD.Get_rank();
	cout << "PID: " << rank << endl;
	Image::Init(width, height);
	//cout << Image::FeatureSize() << endl;
	//Image::PrintFeaturePos();
	Image img(width, height, "../app/neg/im0.raw"); //don't launch the application with Eclipse, wrong path
	if (rank == 0)
	{
		img.PrintRAW();
		cout << Image::FeatureSize() << endl;
		img.PrintFeature();
	}
	MPI::Finalize();
	return 0;
}

