#include "../include/imageLabel.h"

ImageLabel::ImageLabel(): QLabel()
{
	// контекстное меню (по нажатию правой кнопкой мыши)
	contextMenu = new QMenu();
	
	imageInfoAction = contextMenu->addAction("Информация");
	
	connect(imageInfoAction, &QAction::triggered, this, &ImageLabel::showImageInfo);
	
	imageInfoLabel = new QLabel();
	imageInfoLabel->setWindowTitle("Информация об изображении");
	imageInfoLabel->setMinimumWidth(250);
}

void ImageLabel::loadBMP(std::string loadPath)
{
	bmpImage.load(loadPath);
}
void ImageLabel::saveBMP(std::string savePath)
{
	bmpImage.save(savePath);
}

void ImageLabel::convertImageToGrey()
{
	BMP greyShapesBMP;
	
	BMP::convertToGreyShades(&bmpImage, &greyShapesBMP);
	bmpImage.copy(&greyShapesBMP);
}

void ImageLabel::updateImage()
{
	image = new QImage(bmpImage.getWidth(), bmpImage.getHeight(), QImage::Format_RGB888);
	
	for(int i = 0; i < bmpImage.getHeight(); i++)
	{
		for(int j = 0; j < bmpImage.getWidth(); j++)
		{
			(*image).setPixel(j,bmpImage.getHeight()-1-i, qRgb(bmpImage.pixels[i][j].red, bmpImage.pixels[i][j].green, bmpImage.pixels[i][j].blue));
		}
	}
	setPixmap(QPixmap::fromImage(*image));

}

void ImageLabel::showImageInfo()
{
	imageInfo = QString::fromStdString("Ширина: " + std::to_string(bmpImage.getWidth()) + '\n'
			   						 + "Высота: " + std::to_string(bmpImage.getHeight()) + '\n'
									 + "Глубина цвета: " + std::to_string(bmpImage.getBitsPerPixel()) + '\n'
									 + "Количество каналов: " + std::to_string(bmpImage.getChannelsCount()) + '\n'
									 + "Размер файла: " + std::to_string(bmpImage.getFileSize()) + " байт");
									 
	imageInfoLabel->setText(imageInfo);
	imageInfoLabel->show();
	
}

bool ImageLabel::hasImage()
{
	if(image != nullptr)
	{
		return true;
	}
	else
	{
		return false;
	}	
}

void ImageLabel::clearImageLabel()
{
    image = nullptr;
	this->clear();
}

void ImageLabel::mouseMoveEvent(QMouseEvent * event)
{
	if(image != nullptr)
	{
		int x = event->pos().rx() - (this->width() - image->width()) / 2;
		int y = event->pos().ry() - (this->height() - image->height()) / 2;
		
		if(x >= 0 && y >= 0 && x < image->width() && y < image->height())
		{
			QRgb pixel = image->pixel(x,y);
			
			coords = QString::fromStdString("Координаты (" 
											+ std::to_string(x) + "; " 
											+ std::to_string(y)) + ")";
			rgb = QString::fromStdString("RGB (" + std::to_string(qRed(pixel)) + " ; " 
										+ std::to_string(qGreen(pixel)) + "; " 
										+ std::to_string(qBlue(pixel))) + ")";
			
			coordsLabel->setText(coords);
			rgbLabel->setText(rgb);	
		}
	}
	else
	{
		coordsLabel->setText("Координаты");
		rgbLabel->setText("RGB");
	}
}
void ImageLabel::mouseReleaseEvent(QMouseEvent * event)
{
	if(image != nullptr)
	{
		if(event->button() == Qt::RightButton)
		{
			QPoint pos = mapToGlobal(event->pos());
			
			contextMenu->setGeometry(pos.rx(), pos.ry(), 130, 32);
			contextMenu->show();
		}
	}
	
}

