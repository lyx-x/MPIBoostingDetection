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

using namespace std;

class Image {
public:
	static void Init(int, int); //Find all possible feature
	static int FeatureSize();
	static void PrintFeaturePos();
	static int FeatureEncode(int, int, int, int, int);
	static void FeatureDecode(int, int&, int&, int&, int&, int&);

	Image();
	Image(int, int, string); //width, height and path
	virtual ~Image();
	void PrintRAW() const;
	void PrintIntegral() const;
	void PrintFeature() const;
	int Height() const;
	int Width() const;
	int Size() const;
	void InitFeature();
	void InitFeatureParallel();
	int FeatureAt(int) const; //index, read feature and not localFeature

private:
	static int gHeight;
	static int gWidth;
	static int* featurePos;
	static int featureSize;

	const int height; //Not really useful, just a generic method to read an image
	const int width;
	char* content = NULL; //image
	int* integral = NULL; //integral image
	int* feature = NULL; //features
	int* localFeature = NULL;
	string file;
	int PixelAt(int, int) const; //row and line: (x, y)
	int IntegralAt(int, int) const; //x and y
	void SetIntegralAt(int, int, int) const; //x, y and value
	void SetFeatureAt(int); //index of feature
	void SetFeatureAt(int, int, int, int, int, int); //x, y, width, height, type and index
	int PartialSum(int, int, int, int) const; //x, y, width and height
};

#endif /* IMAGE_H_ */
