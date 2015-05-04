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

using namespace std;

class Image {
public:
	Image();
	Image(int, int, string); //width, height and path
	virtual ~Image();
	void PrintRAW() const;
	void PrintIntegral() const;
	void PrintFeature() const;
	int Height() const;
	int Width() const;
	int Size() const;
	int FeatureSize() const;
	void InitFeature();
	int FeatureAt(int) const; //index
private:
	const int height;
	const int width;
	char* content = NULL; //image
	int* integral = NULL; //integral image
	int* feature = NULL; //features
	int featureSize = 0;
	string file;
	int PixelAt(int, int) const; //row and line: (x, y)
	int IntegralAt(int, int) const; //x and y
	void SetIntegralAt(int, int, int) const; //x, y and value
	void SetFeatureAt(int, int, int, int, int, int); //x, y, width, height, type and index
	int PartialSum(int, int, int, int) const; //x, y, width and height
	void FeatureCount();
};

#endif /* IMAGE_H_ */
