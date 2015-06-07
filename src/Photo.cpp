/*
 * Photo.cpp
 *
 *  Created on: May 14, 2015
 *      Author: lyx
 */

#include "Photo.h"
#include "Image.h"

namespace photo {

//Constructors

Photo::Photo() {
	content = NULL;
	photoWidth = 0;
	photoHeight = 0;
	baseWidth = imageUtils::width;
	baseHeight = imageUtils::height;
	loop = 1;
}

Photo::Photo(int w, int h, string path): photoWidth(w), photoHeight(h) {
	file = path;
	ifstream img (path, ifstream::binary); //read binary 8-bits grayscale image
	content = new char[h * w];
	img.read(content, h * w);
	img.close();
	faces.clear();
	baseWidth = imageUtils::width;
	baseHeight = imageUtils::height;
	loop = 1;
}

Photo::~Photo() {
	delete[] content;
}

//Public Methods

void Photo::FindFace() {
	int count = 0;
	for (int r = 1 ; r <= loop ; r++) {
		int w = baseWidth * r;
		int h = baseHeight * r;
		int deltaX = w / 24;
		int deltaY = h / 24;
		for (int x = 0 ; x < photoWidth - w ; x += deltaX)
			for (int y = 0 ; y < photoHeight - h ; y += deltaY) {
				count++;
				Image* test = Resize(x, y, w, h);
				int result = adaboost::Classify(test);
				if (result == 1) {
					faces.push_back(Face(x, y, w, h));
				}
				delete test;
			}
	}
	cout << count << endl;
	PrintFaces();
}

Face Photo::GetFaceAt(int index) const {
	return faces[index];
}

int Photo::CountFace() const {
	return faces.size();
}

void Photo::Print() {
	ofstream out(file + ".jrl");
	out << file << endl;
	out << "RAW\n";
	for (int i = 0 ; i < photoWidth ; i++) {
		for (int j = 0 ; j < photoHeight ; j++)
			out << PixelAt(i, j) << '\t';
		out << endl;
	}
	out << "EOF" << endl;
}

void Photo::PrintFaces() {
	ofstream out(file + ".res");
	for (Face f : faces) {
		out << f.x << '\t' << f.y << '\t' << f.w << '\t' << f.h << endl;
	}
}

void Photo::SetBaseDimension(int w, int h, int l) {
	baseWidth = w;
	baseHeight = h;
	loop = l;
}

//Private Methods

int Photo::PixelAt(int x, int y) const {
	return (unsigned char)*(content + (x + y * photoWidth));
}

Image* Photo::Resize(int x, int y, int w, int h) {
	char* _content = new char[width * height];
	int rx = w / width;
	int ry = h / height;
	if (rx >=1 && ry >= 1) {
		w = rx * width;
		h = ry * height;
		for (int i = 0 ; i < w ; i += rx)
			for (int j = 0 ; j < h ; j += ry) {
				int tmp = 0;
				for (int _x = 0 ; _x < rx ; _x++)
					for (int _y = 0 ; _y < ry ; _y++)
					{
						if (x + i + _x < photoWidth && y + j + _y < photoHeight)
							tmp += PixelAt(x + i + _x, y + j + _y);
					}
				tmp /= rx * ry;
				SetPixelAt(_content, i / rx, j / rx, char(tmp), width);
			}
	}
	else {
		rx = (width + w - 1) / w;
		ry = (height + h - 1) / h;
		for (int i = 0 ; i < w ; i++)
			for (int _x = 0 ; _x < rx ; _x++) {
				if (i * rx + _x >= width)
					break;
				for (int j = 0 ; j < h ; j++) {
					int tmp = PixelAt(x + i, y + j);
					for (int _y = 0 ; _y < ry ; _y++) {
						if (j * ry + _y >= height)
							break;
						SetPixelAt(_content, i * rx + _x, j * ry + _y, char(tmp), width);
					}
				}
			}
	}
	Image* tmp = new Image(_content);
	return tmp;
}

void Photo::SetPixelAt(char* _tmp, int x, int y, char val, int w) {
	*(_tmp + (x + y * w)) = val;
}

} /* namespace Globals */
