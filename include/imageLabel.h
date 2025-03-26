#pragma once

#include <QLabel>
#include <QImage>
#include <QMouseEvent>
#include <QMenu>
#include <QPoint>

#include <string>
#include <iostream>

#include "bmp.h"

class ImageLabel: public QLabel
{
public:
	ImageLabel();
	
	QImage * image = nullptr;
	QLabel * coordsLabel;
	QLabel * rgbLabel;

	
	void loadBMP(std::string loadPath);
	void saveBMP(std::string loadPath);
	
	void convertImageToGrey();
	
	void updateImage();
	
	bool hasImage();

	void clearImageLabel();
private:

	QMenu * contextMenu;
	QLabel * imageInfoLabel;
	
	QAction * imageInfoAction;

	void showImageInfo();
	
	QString coords;
	QString rgb;
	QString imageInfo;
	
	BMP bmpImage;
	
	void mouseMoveEvent(QMouseEvent * event) override;
	void mouseReleaseEvent(QMouseEvent * event) override;
};