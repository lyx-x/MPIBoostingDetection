/*
 * Globals.cpp
 *
 *  Created on: May 6, 2015
 *      Author: lyx
 */

#include "Globals.h"

namespace imageUtils {

const int dim = 8;
const string dir = "../";
const int negCount(4415);
const int posCount(818);
const string trainPath = dir + "app/";
const string validationPath = dir + "dev/";
const string testPath = dir + "test/";
const string photoPath = dir + "photo/";
const int width(112);
const int height(92);
ofstream error(dir + "err.log");
ofstream journal(dir + "general.jrl");

} /* namespace imageUtils */

namespace mpiUtils {

int rank(0);
int size(1);
int* recvCounts = NULL;
int* displs = NULL;

void InitGather(int featureSize) {
	recvCounts = new int[mpiUtils::size];
	displs = new int[mpiUtils::size];
	int length = (featureSize + mpiUtils::size - 1) / mpiUtils::size;
	for (int i = 0 ; i < mpiUtils::size ; i++) {
		recvCounts[i] = length;
		displs[i] = length * i;
	}
	recvCounts[mpiUtils::size - 1] = featureSize - displs[mpiUtils::size - 1];
}

void DropGather() {
	delete[] recvCounts;
	delete[] displs;
}

} /* namespace mpiUtils */
