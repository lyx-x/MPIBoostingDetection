/*
 * Images.h
 *
 *  Created on: May 5, 2015
 *      Author: lyx
 */

#ifndef IMAGES_H_
#define IMAGES_H_

#include <vector>
#include <ctime>
#include "Image.h"

using namespace std;

namespace imageUtils {

void InitImages();
void ReadImages(vector<Image*>*, string);
Image* GetTrainAt(int);
Image* GetValidationAt(int);
Image* GetTestAt(int);
int GetSize(char); //0: trian; 1: validation; 2: test
void DropImages();

} /* namespace Globals */

#endif /* IMAGES_H_ */
