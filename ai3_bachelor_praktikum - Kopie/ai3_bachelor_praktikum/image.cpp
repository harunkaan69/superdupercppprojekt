#include "image.h"

image::image(const TGA_HEADER& pHeader, const std::vector<unsigned char>& pData)
{
	if (pHeader.imageWidth <= 0 || pHeader.imageHeight <= 0) {
		throw corruptDataError("Image size invalid");
	}
	if (pData.size() != pHeader.imageWidth * pHeader.imageHeight * 4) {
		throw corruptDataError("Image size does not match data");
	}

	header = pHeader;
	pixelData = pData;
}

std::unique_ptr<image> image::load(const std::string& pImagePath) {
	/*
		ONLY 32BIT PIXEL DEPTH SUPPORTED
	*/

	std::ifstream input(pImagePath, std::ifstream::binary);

	if (!input.is_open()) {
		throw fileOperationError(pImagePath);
	}

	/*
		Read first 18 bytes as the header of the image in one go
		------------------------------------+-----------------------------------
		[00] = idLength			(1 byte)	|	[01] = colourmapType	(1 byte)
		[02] = imageType		(1 byte)	|	[03] = colourmapStart	(2 byte)
		[04] = colourmapLength	(2 byte)	|	[05] = colourmapDepth	(1 byte)
		[06] = xOrigin			(2 byte)	|	[07] = yOrigin			(2 byte)
		[08] = imageWidth		(2 byte)	|	[09] = imageHeight		(2 byte)
		[10] = bitsPerPixel		(1 byte)	|	[11] = imageDescriptor	(1 byte)
	*/
	image::TGA_HEADER tmpHeader;
	input.read((char*)&tmpHeader, sizeof(tmpHeader));

	// If bitdepth is not 32 bits (4 byte), throw
	if (tmpHeader.bitsPerPixel/8 != 4) {
		throw corruptDataError("TGA bitdepth");
	}

	/*	
		Prepare both vectors for total data to read 
		and one for each row to work on
	*/
	std::vector<unsigned char> tempData;
	std::vector<unsigned char> tempRow;
	tempData.resize(tmpHeader.imageWidth * tmpHeader.imageHeight * (tmpHeader.bitsPerPixel / 8));
	tempRow.resize(tmpHeader.imageWidth * (tmpHeader.bitsPerPixel / 8));

	/* 
		Loop through each y-coord of the image and
		load pixels row by row. Since '.tga' files
		are stored upside down change the order
		from bottom up to top down
	*/
	for (int y = 0; y < tmpHeader.imageHeight; y++)
	{
		input.read(reinterpret_cast<char*>(&tempRow.front()), 
			tmpHeader.imageWidth * (tmpHeader.bitsPerPixel / 8));

		unsigned char* row = 
			&tempData[(tmpHeader.imageHeight - 1 - y) * tmpHeader.imageWidth * (tmpHeader.bitsPerPixel / 8)];

		for (int i = 0; i < tmpHeader.imageWidth; i++)
		{
			/*
				Data alignment TGA:
						[blue][green][red][alpha]
			*/
			row[i * 4	 ] = tempRow[i * 4    ];	// blue
			row[i * 4 + 1] = tempRow[i * 4 + 1];	// green
			row[i * 4 + 2] = tempRow[i * 4 + 2];	// red
			row[i * 4 + 3] = tempRow[i * 4 + 3];	// alpha
		}
	}

	input.close();
	return std::unique_ptr<image>(new image(tmpHeader, tempData));
}

const std::vector<unsigned char>& image::getPixelData() const
{
	return pixelData;
}

const image::TGA_HEADER& image::getHeader() const
{
	return header;
}