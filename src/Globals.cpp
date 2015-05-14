/*
 * Globals.cpp
 *
 *  Created on: May 6, 2015
 *      Author: lyx
 */

#include "Globals.h"

namespace imageUtils {

const string dir = "../";
const int negCount(4415);
const int posCount(818);
const string trainPath = dir + "app/";
const string validationPath = dir + "dev/";
const string testPath = dir + "test/";
const int width(112);
const int height(92);
ofstream error(dir + "err.log");
ofstream journal(dir + "general.log");

} /* namespace imageUtils */
