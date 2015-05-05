/*
 * Images.h
 *
 *  Created on: May 5, 2015
 *      Author: lyx
 */

#ifndef IMAGES_H_
#define IMAGES_H_

#include <vector>
#include "Image.h"

using namespace std;
using namespace Globals;

namespace Images {

void InitSystem();
void ReadImages(vector<Image>, string);
Image GetTrainAt(int);
Image GetValidationAt(int);
Image GetTestAt(int);

} /* namespace Images */

#endif /* IMAGES_H_ */
