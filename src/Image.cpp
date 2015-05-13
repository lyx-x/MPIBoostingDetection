/*
 * Image.cpp
 *
 *  Created on: May 4, 2015
 *      Author: lyx
 */

#include "Image.h"

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

//Public Static Methods

void Image::Init(){
	int count = 0;
	for (int x = 0 ; x < width ; x += 4)
		for (int y = 0 ; y < height ; y += 4)
			for (int w = 4 ; w <= width ; w += 4)
				for (int h = 4 ; h <= height ; h += 4)
				{
					if (x + w * 2 <= width && y + h <= height)
						count++;
					if (x + w * 2 <= width && y + h <= height)
						count++;
					if (x + w * 3 <= width && y + h * 2 <= height)
						count++;
					if (x + w * 2 <= width && y + h * 2 <= height)
						count++;
				}
	featureSize = count;
	featurePos = new int[featureSize];
	count = 0;
	for (int x = 0 ; x < width ; x += 4)
		for (int y = 0 ; y < height ; y += 4)
			for (int w = 4 ; w <= width ; w += 4)
				for (int h = 4 ; h <= height ; h += 4)
				{
					if (x + w * 2 <= width && y + h <= height)
					{
						featurePos[count] = FeatureEncode(x, y, w, h, 0);
						count++;
					}
					if (x + w * 2 <= width && y + h <= height)
					{
						featurePos[count] = FeatureEncode(x, y, w, h, 1);
						count++;
					}
					if (x + w * 3 <= width && y + h * 2 <= height)
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

int Image::FeatureSize() {
	return featureSize;
}

void Image::PrintFeaturePos() {
	ofstream out("feature.pos");
	out << "Count: " << featureSize << endl;
	for (int i = 0 ; i < featureSize ; i++) {
		out << *(featurePos + i) << endl;
	}
	out.close();
}

int Image::FeatureEncode(int x, int y, int w, int h, int type) {
	int m = max(width, height) / 4 + 1;
	int hash = type;
	hash *= m;
	hash += x / 4;
	hash *= m;
	hash += y / 4;
	hash *= m;
	hash += w / 4;
	hash *= m;
	hash += h / 4;
	return hash;
}

void Image::FeatureDecode(int hash, int& x, int& y, int& w, int& h, int& type) {
	int m = max(width, height) / 4 + 1;
	h = (hash % m) * 4;
	hash /= m;
	w = (hash % m) * 4;
	hash /= m;
	y = (hash % m) * 4;
	hash /= m;
	x = (hash % m) * 4;
	hash /= m;
	type = hash % m;
}

void Image::PrintFeature(int index) {
	int x, y, w, h, type;
	FeatureDecode(featurePos[index], x, y, w, h, type);
	cout << index << '\t' << x << '\t' << y << '\t' << w << '\t' << h << '\t' << type << endl;
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
	InitFeature();
}

Image::~Image() {
	delete[] content;
	delete[] integral;
	delete[] feature;
	delete[] localFeature;
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
	out << file + ".feature" << endl;
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

int Image::Type() const {
	return c;
}

void Image::InitFeature() {
	localFeature = new int[featureSize];
	for (int i = 0 ; i < featureSize ; i++)
		localFeature[i] = -1;
}

void Image::ComputeFeature() {
	//Local method
	localFeature = new int[featureSize];
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

	//Parallel Method
	/*localFeature = new int[featureSize];
	for (int i = 0 ; i < featureSize ; i++)
		SetFeatureAt(i);*/
}

void Image::ComputeFeatureParallel() {
	int rank, size;
	size = MPI::COMM_WORLD.Get_size();
	rank = MPI::COMM_WORLD.Get_rank();
	if (rank == 0)
		cout << size << ' ' << rank << endl;
	feature = new int[featureSize];
	for (int i = 0 ; i < featureSize ; i++)
		localFeature[i] = INT_MIN;
	for (int i = rank ; i < featureSize ; i += size)
		SetFeatureAt(i);
	MPI::COMM_WORLD.Reduce(localFeature, feature, featureSize, MPI::INT, MPI::MAX, 0);
}

int Image::FeatureAt(int i) const {
	return feature[i];
}

//Private Static Methods

int Image::featureSize = 0;
int* Image::featurePos = NULL;

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

void Image::SetFeatureAt(int index) {
	int x, y, w, h, type;
	FeatureDecode(featurePos[index], x, y, w, h, type);
	SetFeatureAt(x, y, w, h, type, index);
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
	localFeature[index] = white - black;
}

int Image::PartialSum(int x, int y, int w, int h) const {
	return IntegralAt(x + w, y + h) + IntegralAt(x, y) - IntegralAt(x + w, y) - IntegralAt(x, y + h);
}

