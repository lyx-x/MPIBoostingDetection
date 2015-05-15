/*
 * Image.cpp
 *
 *  Created on: May 4, 2015
 *      Author: lyx
 */

#include "Image.h"

namespace imageUtils {

int* featurePos = NULL;
int featureSize = 0;

void InitFeatures(){
	int count = 0;
	for (int x = 0 ; x < width ; x += dim)
		for (int y = 0 ; y < height ; y += dim)
			for (int w = dim ; w <= width ; w += dim)
				for (int h = dim ; h <= height ; h += dim)
				{
					if (x + w * 2 <= width && y + h <= height)
						count++;
					if (x + w <= width && y + h * 2 <= height)
						count++;
					if (x + w * 3 <= width && y + h <= height)
						count++;
					if (x + w * 2 <= width && y + h * 2 <= height)
						count++;
				}
	featureSize = count;
	featurePos = new int[featureSize];
	count = 0;
	for (int x = 0 ; x < width ; x += dim)
		for (int y = 0 ; y < height ; y += dim)
			for (int w = dim ; w <= width ; w += dim)
				for (int h = dim ; h <= height ; h += dim)
				{
					if (x + w * 2 <= width && y + h <= height)
					{
						featurePos[count] = FeatureEncode(x, y, w, h, 0);
						count++;
					}
					if (x + w <= width && y + h * 2 <= height)
					{
						featurePos[count] = FeatureEncode(x, y, w, h, 1);
						count++;
					}
					if (x + w * 3 <= width && y + h <= height)
					{
						featurePos[count] = FeatureEncode(x, y, w, h, 2);
						count++;
					}
					if (x + w * 2 <= width && y + h * 2 <= height)
					{
						featurePos[count] = FeatureEncode(x, y, w, h, 3);
						count++;
					}
				}
}

void PrintFeaturePos() {
	ofstream out(dir + "feature.jrl");
	out << "Count: " << featureSize << endl;
	for (int i = 0 ; i < featureSize ; i++) {
		int x, y, w, h, type;
		FeatureDecode(featurePos[i], x, y, w, h, type);
		out << i << '\t' << *(featurePos + i) << '\t' << FeatureEncode(x, y, w, h, type) << '\t' << x << '\t' << y << '\t' << w << '\t' << h << '\t' << type << endl;
	}
	out.close();
}

int FeatureEncode(int x, int y, int w, int h, int type) {
	int m = max(width, height) / dim + 1;
	int hash = type;
	hash *= m;
	hash += x / dim;
	hash *= m;
	hash += y / dim;
	hash *= m;
	hash += w / dim;
	hash *= m;
	hash += h / dim;
	return hash;
}

void FeatureDecode(int hash, int& x, int& y, int& w, int& h, int& type) {
	int m = max(width, height) / dim + 1;
	h = (hash % m) * dim;
	hash /= m;
	w = (hash % m) * dim;
	hash /= m;
	y = (hash % m) * dim;
	hash /= m;
	x = (hash % m) * dim;
	hash /= m;
	type = hash % m;
}

void PrintFeature(int index) {
	int x, y, w, h, type;
	FeatureDecode(featurePos[index], x, y, w, h, type);
	cout << index << '\t' << x << '\t' << y << '\t' << w << '\t' << h << '\t' << type << endl;
}

int GetType(string path) {
	istringstream ss(path);
	string type;
	while (getline(ss, type, '/'))
	{
		if (type == "pos")
			return 1;
		if (type == "neg")
			return -1;
	}
	return 0;
}

int RandomImage() {
	int type = rand() % 2;
	int choice;
	if (type == 0)
		choice = rand() % posCount + negCount;
	else
		choice = rand() % negCount;
	return choice;
}

int RandomFeature() {
	return rand() * featureSize;
}

void DropFeatures() {
	delete[] featurePos;
}

}

//Constructors

Image::Image() {
	file = "NO FILE";
	c = 0;
}

Image::Image(string path) {
	file = path;
	ifstream img (path, ifstream::binary); //read binary 8-bits grayscale image
	content = new char[height * width];
	img.read(content, height * width);
	img.close();
	c = GetType(file);
	integral = new int[height * width];
	SetIntegralAt(0, 0, PixelAt(0, 0));
	for (int i = 1 ; i < width ; i++)
		SetIntegralAt(i, 0, IntegralAt(i - 1, 0) + PixelAt(i, 0));
	for (int j = 1 ; j < height ; j++)
		SetIntegralAt(0, j, IntegralAt(0, j - 1) + PixelAt(0, j));
	for (int i = 1 ; i < width ; i++)
		for (int j = 1 ; j < height ; j++)
			SetIntegralAt(i, j, IntegralAt(i - 1, j) + IntegralAt(i, j - 1) + PixelAt(i, j) - IntegralAt(i - 1, j - 1));
}

Image::Image(char* cont) {
	file = "TMP";
	content = cont;
	integral = new int[height * width];
	SetIntegralAt(0, 0, PixelAt(0, 0));
	for (int i = 1 ; i < width ; i++)
		SetIntegralAt(i, 0, IntegralAt(i - 1, 0) + PixelAt(i, 0));
	for (int j = 1 ; j < height ; j++)
		SetIntegralAt(0, j, IntegralAt(0, j - 1) + PixelAt(0, j));
	for (int i = 1 ; i < width ; i++)
		for (int j = 1 ; j < height ; j++)
			SetIntegralAt(i, j, IntegralAt(i - 1, j) + IntegralAt(i, j - 1) + PixelAt(i, j) - IntegralAt(i - 1, j - 1));
	c = 0;
}

Image::~Image() {
	delete[] content;
	delete[] integral;
}

//Public Methods

void Image::PrintRAW() const {
	journal << "File: " << file << ' ' << width << " * " << height << endl;
	journal << "RAW" << endl;
	for (int i = 0 ; i < width ; i++) {
		for (int j = 0 ; j < height ; j++)
			journal << PixelAt(i, j) << '\t';
		journal << endl;
	}
	journal << "EOF" << endl;
}

void Image::PrintIntegral() const {
	journal << "File: " << file << ' ' << width << " * " << height << endl;
	journal << "Integral" << endl;
	for (int i = 0 ; i < width ; i++) {
		for (int j = 0 ; j < height ; j++)
			journal << IntegralAt(i, j) << '\t';
		journal << endl;
	}
	journal << "EOF" << endl;
}

void Image::PrintFeature() const {
	ofstream out(file + ".feature");
	out << file + ".feature" << endl;
	out << "File: " << file << ' ' << width << " * " << height << endl;
	out << "Count: " << featureSize << endl;
	for (int i = 0 ; i < featureSize ; i++) {
		int x, y, w, h, type;
		FeatureDecode(featurePos[i], x, y, w, h, type);
		out << FeatureAt(i) << endl;
		out << i << '\t' << *(featurePos + i) << '\t' << x << '\t' << y << '\t' << w << '\t' << h << '\t' << type << endl;
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

int Image::Type() const {
	return c;
}

int Image::FeatureAt(int index) const {
	int hash = featurePos[index];
	int x, y, w, h, type;
	FeatureDecode(hash, x, y, w, h, type);
	return FeatureAt(x, y, w, h, type);
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

int Image::FeatureAt(int x, int y, int w, int h, int type) const {
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
	return white - black;
}

int Image::PartialSum(int x, int y, int w, int h) const {
	return IntegralAt(x + w - 1, y + h - 1) + IntegralAt(x, y) - IntegralAt(x + w - 1, y) - IntegralAt(x, y + h - 1);
}

