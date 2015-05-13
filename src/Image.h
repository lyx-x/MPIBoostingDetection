/*
 * Image.h
 *
 *  Created on: May 4, 2015
 *      Author: lyx
 */

#ifndef IMAGE_H_
#define IMAGE_H_

#include <fstream>
#include <iostream>
#include <cmath>
#include "mpi.h"
#include <sstream>
#include "Globals.h"

using namespace std;
using namespace Globals;

namespace Globals {
	extern int* featurePos;
	extern int featureSize;

	void InitFeatures(); //Find all possible feature
	void PrintFeaturePos();
	int FeatureEncode(int, int, int, int, int);
	void FeatureDecode(int, int&, int&, int&, int&, int&);
	void PrintFeature(int);
	int GetType(string);
	int RandomImage();
}

class Image {
public:
	Image();
	Image(string); //width, height and path
	virtual ~Image();
	void PrintRAW() const;
	void PrintIntegral() const;
	void PrintFeature() const;
	int Height() const;
	int Width() const;
	int Size() const;
	int Type() const;
	int FeatureAt(int) const; //index

private:
	int c;
	char* content = NULL; //image
	int* integral = NULL; //integral image
	string file;
	int PixelAt(int, int) const; //row and line: (x, y)
	int IntegralAt(int, int) const; //x and y
	void SetIntegralAt(int, int, int) const; //x, y and value
	int FeatureAt(int, int, int, int, int) const; //x, y, width, height and type
	int PartialSum(int, int, int, int) const; //x, y, width and height
};

#endif /* IMAGE_H_ */
