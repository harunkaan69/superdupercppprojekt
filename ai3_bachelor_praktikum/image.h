#pragma once
#include <string>
#include <memory>
#include <fstream>
#include <iostream>
#include <vector>
#include "exception.h"

/*
	Class to manage images
*/
class image
{
public:
// Push current allignment setting on stack and set new one to 1 byte (char)
#pragma pack(push, 1)
	// struct to save the image header information
	struct TGA_HEADER {
		char	idLength;
		char	colourmapType;
		char	imageType;
		short	colourmapStart;
		short	colourmapLength;
		char	colourmapDepth;
		short	xOrigin;
		short	yOrigin;
		short	imageWidth;
		short	imageHeight;
		char	bitsPerPixel;
		char	imageDescriptor;
	};
// Restore saved allignment setting from the internal stack
#pragma pack(pop)

private:
	image(const TGA_HEADER& pHeader, const std::vector<unsigned char>& pData);
	TGA_HEADER header;
	std::vector<unsigned char> pixelData;
public:
	/*
		Get header/raw pixel data/width/heigth/individual pixel
	*/
	const std::vector<unsigned char>& getPixelData() const;
	const image::TGA_HEADER& getHeader() const;

	/*
		Load a given image (as parameter), disassemble it into header and pixel data
		and return a pointer pointing to an image object (ONLY 32 BIT PIXELDEPTH)
	*/
	static std::unique_ptr<image> load(const std::string& pImagePath);
};