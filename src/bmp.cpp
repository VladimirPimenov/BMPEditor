#include "bmp.h"
#include <fstream>

int BMP::getWidth()
{
	return bmpInfo.width;
}
int BMP::getHeight()
{
	return bmpInfo.height;
}
int BMP::getFileSize()
{
	return bmpHeader.fileSize;
}
int BMP::getBitsPerPixel()
{
	return bmpInfo.bitsPerPixel;

}
int BMP::getChannelsCount()
{
	return sizeof(Pixel);

}

void BMP::load(std::string loadFilePath)
{
	std::ifstream loadFile(loadFilePath, std::ios::binary);
	
	if(loadFile.is_open())
	{
		loadFile.read((char *)(&bmpHeader), sizeof(BITMAPFILEHEADER));
		loadFile.read((char *)(&bmpInfo), sizeof(BITMAPINFOHEADER));
		loadFile.seekg(bmpHeader.offset, std::ios::beg);
		
		nullBytesCount = 4 - ((bmpInfo.width * 3) % 4);
		if(nullBytesCount == 4)
		{
			nullBytesCount = 0;
		}
		
		Pixel pixel;
		pixels = new Pixel*[bmpInfo.height];
		for(int i = 0; i < bmpInfo.height; i++)
		{
			pixels[i] = new Pixel[bmpInfo.width];
			
			for(int j = 0; j < bmpInfo.width; j++)
			{
				loadFile.read((char *)&pixel, sizeof(Pixel));
				pixels[i][j] = pixel;
			}
			if(nullBytesCount != 0)
			{
				loadFile.read((char *)&nullByte, sizeof(unsigned char));
				loadFile.seekg(nullBytesCount-1, std::ios::cur);
			}
		}
		loadFile.close();
	}
}

void BMP::save(std::string saveFilePath)
{
	std::ofstream saveFile(saveFilePath, std::ios::binary);
	
	if(saveFile.is_open())
	{
		saveFile.write((char *)(&bmpHeader), sizeof(BITMAPFILEHEADER));
		saveFile.write((char *)(&bmpInfo), sizeof(BITMAPINFOHEADER));
		saveFile.seekp(bmpHeader.offset, std::ios::beg);
		
		for(int i = 0; i < bmpInfo.height; i++)
		{
			for(int j = 0; j < bmpInfo.width; j++)
			{
				saveFile.write((char *)&pixels[i][j], sizeof(Pixel));
			}
			if(nullBytesCount != 0)
			{
				saveFile.seekp(nullBytesCount, std::ios::cur);
			}
		}
		if(nullBytesCount != 0)
		{
			for(int i = 0; i < nullBytesCount; i++)
			{
				saveFile.write((char *)&nullByte,std::ios::cur);
			}
		}
		saveFile.close();
	}
}

void BMP::copy(BMP * inputBMP)
{
	this->~BMP();
	this->bmpHeader = inputBMP->bmpHeader;
	this->bmpInfo = inputBMP->bmpInfo;
	this->nullByte = inputBMP->nullByte;
	this->nullBytesCount = inputBMP->nullBytesCount;
	
	this->pixels = new Pixel*[this->bmpInfo.height];
	for(int i = 0; i < this->bmpInfo.height; i++)
	{
		this->pixels[i] = new Pixel[this->bmpInfo.width];
		for(int j = 0; j < this->bmpInfo.width; j++)
		{
			this->pixels[i][j] = inputBMP->pixels[i][j];
		}
	}
}

void BMP::convertToGreyShades(BMP * inputBMP, BMP * outputBMP)
{
	outputBMP->bmpHeader = inputBMP->bmpHeader;
	outputBMP->bmpInfo = inputBMP->bmpInfo;
	outputBMP->nullByte = inputBMP->nullByte;
	outputBMP->nullBytesCount = inputBMP->nullBytesCount;
	
	outputBMP->pixels = new Pixel*[outputBMP->bmpInfo.height];
	for(int i = 0; i < outputBMP->bmpInfo.height; i++)
	{
		outputBMP->pixels[i] = new Pixel[outputBMP->bmpInfo.width];
		for(int j = 0; j < outputBMP->bmpInfo.width; j++)
		{
			unsigned char grey = (inputBMP->pixels[i][j].red + inputBMP->pixels[i][j].green + inputBMP->pixels[i][j].blue) / 3;
			outputBMP->pixels[i][j].red = grey;
			outputBMP->pixels[i][j].green = grey;
			outputBMP->pixels[i][j].blue = grey;
		}
	}
}

BMP::~BMP()
{
	for(int i = 0; i < bmpInfo.height; i++)
	{
		delete[] pixels[i];
	}
	delete[] pixels;
}