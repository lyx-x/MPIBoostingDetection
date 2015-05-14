/*
 * Images.cpp
 *
 *  Created on: May 5, 2015
 *      Author: lyx
 */

#include "Images.h"

namespace imageUtils {

vector<Image*> trainImages;
vector<Image*> validationImages;
vector<Image*> testImages;

void InitImages() {
	clock_t t;
	t = clock();
	ReadImages(&trainImages, trainPath);
	ReadImages(&validationImages, validationPath);
	ReadImages(&testImages, testPath);
	t = clock() - t;
	journal << "Reading Images: " << ((float)t)/CLOCKS_PER_SEC << "seconds.\n";
}

void ReadImages(vector<Image*>* images, string path) {
	for (int i = 0 ; i < negCount ; i++) {
		ostringstream _path;
		_path << path;
		_path << "neg/im";
		_path << i;
		_path << ".raw";
		Image* img = new Image(_path.str());
		images->push_back(img);
	}
	for (int i = 0 ; i < posCount ; i++) {
		ostringstream _path(path);
		_path << path;
		_path << "pos/im";
		_path << i;
		_path << ".raw";
		Image* img = new Image(_path.str());
		images->push_back(img);
	}
}

Image* GetTrainAt(int index) {
	return trainImages[index];
}

Image* GetValidationAt(int index) {
	return validationImages[index];
}

Image* GetTestAt(int index) {
	return testImages[index];
}

int GetSize(char p) {
	switch (p) {
	case 0:
		return trainImages.size();
	case 1:
		return validationImages.size();
	case 2:
		return testImages.size();
	default:
		return -1;
	}
}

void DropImages() {
	for (Image* img : trainImages)
		delete img;
}

} /* namespace Globals */
