#pragma once

#include <string>

struct Pixel
{
	unsigned char blue;
	unsigned char green;
	unsigned char red;
};

#pragma pack(push, 2)
struct BITMAPFILEHEADER
{
	unsigned char type[2];
	unsigned int fileSize;
	unsigned char reserved[4];
	unsigned int offset;
};
struct BITMAPINFOHEADER
{
	unsigned int headerSize;
	unsigned int width;
	unsigned int height;
	unsigned short planes;
	unsigned short bitsPerPixel;
};
#pragma pack(pop)

class BMP
{
	BITMAPFILEHEADER bmpHeader;
	BITMAPINFOHEADER bmpInfo;
	
	int nullBytesCount;
	unsigned char nullByte;
	
public:
	Pixel ** pixels;

	int getWidth();
	int getHeight();
	int getFileSize();
	int getBitsPerPixel();
	int getChannelsCount();

	void load(std::string loadFilePath);
	void save(std::string saveFilePath);
	
	void copy(BMP * inputBMP);
	
	static void convertToGreyShades(BMP * inputBMP, BMP * outputBMP);
	~BMP();
};

