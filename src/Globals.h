/*
 * Globals.h
 *
 *  Created on: May 5, 2015
 *      Author: lyx
 */

#ifndef GLOBALS_H_
#define GLOBALS_H_

#include <string>
#include <fstream>

using namespace std;

namespace Globals {

extern const int negCount;
extern const int posCount;
extern const string trainPath;
extern const string validationPath;
extern const string testPath;
extern const int width;
extern const int height;
extern ofstream error;
extern ofstream journal;

}

#endif /* GLOBALS_H_ */

