#include "PPMImage.h"
#include <fstream>
#include <climits>

using std::ifstream;

int PPMImage::getWidth()
{
	return width;
}

int PPMImage::getHeight()
{
	return height;
}

int PPMImage::getLength()
{
	return img.length();
}

int PPMImage::getMinPixel()
{
	return minPixelVal;
}

int PPMImage::getMaxPixel()
{
	return maxPixelVal;
}

float PPMImage::getMin()
{
	return (float)minPixelVal / (float)range;
}

float PPMImage::getMax()
{
	return (float)maxPixelVal / (float)range;
}

PPMImage::PPMImage(string file)
{
	int pxCnt;
	char tmp[50];
	ifstream fileStream(file.c_str(), std::ios::in);

	if(!fileStream.good())
		throw std::exception("[ERR]PPMImage: Unable to read passed file");

	// get the size of the file
	fileStream.seekg(0, std::ios::end);
	img.resize((unsigned int)fileStream.tellg());
	fileStream.seekg(0, std::ios::beg);

	// remove the first line
	fileStream.getline(tmp, sizeof(tmp));
	// get the witdh and height
	fileStream >> width;
	fileStream >> height;
	// read the last line
	fileStream >> range;
	fileStream.getline(tmp, sizeof(tmp));

	// read in the contents
	pxCnt = fileStream.read(&img[0], img.size()).gcount();
	fileStream.close();
	img.resize(pxCnt);

	// find the min and max pixel value
	maxPixelVal = INT_MIN;
	minPixelVal = INT_MAX;

	for (int index = 0; index < pxCnt; index++)
	{
		if (img[index] > maxPixelVal)
			maxPixelVal = img[index];
		if (img[index] <= minPixelVal)
			minPixelVal = img[index];
	}
}

PPMImage::~PPMImage()
{}

unsigned char * PPMImage::image()
{
	return (unsigned char*)img.c_str();
}
