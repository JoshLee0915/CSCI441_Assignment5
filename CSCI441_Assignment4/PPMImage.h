#pragma once
#include <string>

using std::string;

class PPMImage
{
private:
	int width;
	int height;
	int range;
	int maxPixelVal;
	int minPixelVal;
	string img;
public:
	int getWidth();
	int getHeight();
	int getLength();
	int getMinPixel();
	int getMaxPixel();
	float getMin();
	float getMax();
	PPMImage(string file);
	~PPMImage();
	unsigned char* image();
};

