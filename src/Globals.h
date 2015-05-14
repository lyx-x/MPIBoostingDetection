/*
 * Globals.h
 *
 *  Created on: May 5, 2015
 *      Author: lyx
 */

#ifndef GLOBALS_H_
#define GLOBALS_H_

#include <fstream>

using namespace std;

namespace imageUtils {

extern const int dim;
extern const int negCount;
extern const int posCount;
extern const string dir;
extern const string trainPath;
extern const string validationPath;
extern const string testPath;
extern const int width;
extern const int height;
extern ofstream error;
extern ofstream journal;

}

namespace mpiUtils {

extern int rank;
extern int size;
extern int* recvCounts;
extern int* displs;

void InitGather(int);
void DropGather();

}

#endif /* GLOBALS_H_ */

