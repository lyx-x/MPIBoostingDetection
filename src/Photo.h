/*
 * Photo.h
 *
 *  Created on: May 14, 2015
 *      Author: lyx
 */

#ifndef PHOTO_H_
#define PHOTO_H_

#include "Adaboost.h"

using namespace std;
using namespace imageUtils;

namespace photo {

struct Face{
	int x, y, w, h;
	Face(int _x, int _y, int _w, int _h): x(_x), y(_y), w(_w), h(_h) {}
};

class Photo {
public:
	Photo();
	Photo(int, int, string);
	virtual ~Photo();
	void FindFace();
	Face GetFaceAt(int) const;
	int CountFace() const;
	void Print();
	void PrintFaces();
private:
	int photoWidth;
	int photoHeight;
	string file;
	char* content;
	vector<Face> faces;
	int PixelAt(int, int) const; //x and y
	void SetPixelAt(char*, int, int, char, int);
	Image* Resize(int, int, int, int); //x, y, w and h
};

} /* namespace Globals */

#endif /* PHOTO_H_ */
