/*
 * Image.cpp
 *
 *  Created on: May 4, 2015
 *      Author: lyx
 */

#include "Image.h"

using namespace std;

//Constructors

Image::Image(): height(0), width(0) {
	file = "NO FILE";
}

Image::Image(int w, int h, string path): height(h), width(w) {
	file = path;
	ifstream img (path, ifstream::binary); //read binary 8-bits grayscale image
	content = new char[height * width];
	img.read(content, height * width);
	img.close();
	integral = new int[height * width];
	SetIntegralAt(0, 0, PixelAt(0, 0));
	for (int i = 1 ; i < width ; i++)
		SetIntegralAt(i, 0, IntegralAt(i - 1, 0) + PixelAt(i, 0));
	for (int j = 1 ; j < height ; j++)
		SetIntegralAt(0, j, IntegralAt(0, j - 1) + PixelAt(0, j));
	for (int i = 1 ; i < width ; i++)
		for (int j = 1 ; j < height ; j++)
			SetIntegralAt(i, j, IntegralAt(i - 1, j) + IntegralAt(i, j - 1) + PixelAt(i, j) - IntegralAt(i - 1, j - 1));
	InitFeature();
}

Image::~Image() {
	delete[] content;
	delete[] integral;
	delete[] feature;
}

//Public Methods

void Image::PrintRAW() const {
	cout << "File: " << file << ' ' << width << " * " << height << endl;
	cout << "RAW" << endl;
	for (int i = 0 ; i < width ; i++) {
		for (int j = 0 ; j < height ; j++)
			cout << PixelAt(i, j) << '\t';
		cout << endl;
	}
	cout << "EOF" << endl;
}

void Image::PrintIntegral() const {
	cout << "File: " << file << ' ' << width << " * " << height << endl;
	cout << "Integral" << endl;
	for (int i = 0 ; i < width ; i++) {
		for (int j = 0 ; j < height ; j++)
			cout << IntegralAt(i, j) << '\t';
		cout << endl;
	}
	cout << "EOF" << endl;
}

void Image::PrintFeature() const {
	ofstream out(file + ".feature");
	out << "File: " << file << ' ' << width << " * " << height << endl;
	out << "Count: " << featureSize << endl;
	for (int i = 0 ; i < featureSize ; i++) {
		out << FeatureAt(i) << endl;
	}
	out << "End" << endl;
	out.close();
}

int Image::Height() const {
	return height;
}

int Image::Width() const {
	return width;
}

int Image::Size() const {
	return height * width;
}

int Image::FeatureSize() const {
	return featureSize;
}

void Image::InitFeature() {
	FeatureCount();
	int count = 0;
	for (int x = 0 ; x < width ; x += 4)
		for (int y = 0 ; y < height ; y += 4)
			for (int w = 4 ; w < width ; w += 4)
				for (int h = 4 ; h < height ; h += 4)
				{
					if (x + w * 2 < width && y + h < height)
					{
						SetFeatureAt(x, y, w, h, 0, count);
						count++;
					}
					if (x + w * 2 < width && y + h < height)
					{
						SetFeatureAt(x, y, w, h, 1, count);
						count++;
					}
					if (x + w * 3 < width && y + h * 2 < height)
					{
						SetFeatureAt(x, y, w, h, 2, count);
						count++;
					}
					if (x + w * 2 < width && y + h * 2 < height)
					{
						SetFeatureAt(x, y, w, h, 3, count);
						count++;
					}
				}
}

int Image::FeatureAt(int i) const {
	return *(feature + i);
}

//Private Methods

int Image::PixelAt(int x, int y) const {
	return (unsigned char)*(content + (x + y * width));
}

int Image::IntegralAt(int x, int y) const {
	return *(integral + (x + y * width));
}

void Image::SetIntegralAt(int x, int y, int val) const {
	*(integral + (x + y * width)) = val;
}

void Image::SetFeatureAt(int x, int y, int w, int h, int type, int index) {
	int white = 0;
	int black = 0;
	switch (type) {
	case 0:
		white = PartialSum(x, y, w, h);
		black = PartialSum(x + w, y, w, h);
		break;
	case 1:
		white = PartialSum(x, y, w, h);
		black = PartialSum(x, y + h, w, h);
		break;
	case 2:
		white = PartialSum(x, y, w, h);
		black = PartialSum(x + w, y, w, h);
		white += PartialSum(x + w * 2, y, w, h);
		break;
	case 3:
		white = PartialSum(x, y, w, h);
		black = PartialSum(x + w, y, w, h);
		white += PartialSum(x + w, y + h, w, h);
		black += PartialSum(x, y + h, w, h);
		break;
	default:
		cerr << "Error: Undefined type of feature" << endl;
	}
	*(feature + index) = white - black;
}

int Image::PartialSum(int x, int y, int w, int h) const {
	return IntegralAt(x + w, y + h) + IntegralAt(x, y) - IntegralAt(x + w, y) - IntegralAt(x, y + h);
}

void Image::FeatureCount() {
	int count = 0;
	for (int x = 0 ; x < width ; x += 4)
		for (int y = 0 ; y < height ; y += 4)
			for (int w = 4 ; w < width ; w += 4)
				for (int h = 4 ; h < height ; h += 4)
				{
					if (x + w * 2 < width && y + h < height)
						count++;
					if (x + w * 2 < width && y + h < height)
						count++;
					if (x + w * 3 < width && y + h * 2 < height)
						count++;
					if (x + w * 2 < width && y + h * 2 < height)
						count++;
				}
	featureSize = count;
	feature = new int[featureSize];
}
