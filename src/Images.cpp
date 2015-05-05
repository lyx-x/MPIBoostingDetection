/*
 * Images.cpp
 *
 *  Created on: May 5, 2015
 *      Author: lyx
 */

#include "Images.h"

namespace Images {

vector<Image> trainImages;
vector<Image> validationImages;
vector<Image> testImages;

void InitSystem() {
	ReadImages(trainImages, trainPath);
	ReadImages(validationImages, validationPath);
	ReadImages(testImages, testPath);
}

void ReadImages(vector<Image> images, string path) {
	for (int i = 0 ; i < negCount ; i++) {
		string _path = path;
		_path += "neg/im";
		_path += i;
		_path += ".raw";
		Image img(defaultWidth, defaultHeight, _path);
		images.push_back(img);
	}
	for (int i = 0 ; i < posCount ; i++) {
		string _path = path;
		_path += "pos/im";
		_path += i;
		_path += ".raw";
		Image img(defaultWidth, defaultHeight, _path);
		images.push_back(img);
	}
}

Image GetTrainAt(int index) {
	return trainImages[index];
}

Image GetValidationAt(int index) {
	return validationImages[index];
}

Image GetTestAt(int index) {
	return testImages[index];
}

} /* namespace Images */
