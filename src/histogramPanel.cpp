#include "../include/histogramPanel.h"
#include <iostream>

int findScale(int max)
{
	int scale = 1;
	
    while(max > 255)
    {
        max /= 10;
        scale *= 10;
    }
    return scale;
}
std::string getExponencialView(double num)
{
    int tenMultiplier = 0;
    
    while(num >= 10)
    {
        num /= 10;
        tenMultiplier++;
    }
    return std::to_string(num).substr(0,3) + "E" + std::to_string(tenMultiplier);
}

HistogramPanel::HistogramPanel() : QWidget()
{
	this->setFixedWidth(345);
	axisOffset = 50;
	Yscale = 1;

	panel = new QVBoxLayout(this);

	createChannelSelector();
	createHistogramPanel();
}

void HistogramPanel::createHistogramPanel()
{
	histogram = new QGraphicsView();
	histogram->setFixedSize(330, 160);
	
	panel->addWidget(histogram);
    
}
void HistogramPanel::reloadHistogramScene()
{
	scene = new QGraphicsScene();
	
	histogram->setScene(scene);
}
void HistogramPanel::createChannelSelector()
{
    channelSelector = new QComboBox();
	channelSelector->addItems({"Красный канал", "Зелёный канал", "Синий канал"});
	channelSelector->setFixedWidth(255);
	channelSelector->setEnabled(false);

	connect(channelSelector, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &HistogramPanel::paintHistogram);

	panel->addWidget(channelSelector);
}

void HistogramPanel::paintX()
{
	QPen pen = Qt::black;
	QBrush brush = Qt::black;
	
    scene->addRect(axisOffset, 0, 260, 1, pen, brush);

	scene->addRect(axisOffset + 51, 0, 1, -5, pen, brush);
	scene->addRect(axisOffset + 102, 0, 1, -5, pen, brush);
	scene->addRect(axisOffset + 153, 0, 1, -5, pen, brush);
	scene->addRect(axisOffset + 204, 0, 1, -5, pen, brush);
	scene->addRect(axisOffset + 255, 0, 1, -5, pen, brush);

	QGraphicsTextItem * x0 = scene->addText("0");
	QGraphicsTextItem * x51 = scene->addText("51");
	QGraphicsTextItem * x102 = scene->addText("102");
	QGraphicsTextItem * x153 = scene->addText("153");
	QGraphicsTextItem * x204 = scene->addText("204");
	QGraphicsTextItem * x255 = scene->addText("255");
	
	x0->setPos(axisOffset + 0,0);
	x51->setPos(axisOffset + 51 - 10,0);
	x102->setPos(axisOffset + 102 - 10,0);
	x153->setPos(axisOffset + 153 - 10,0);
	x204->setPos(axisOffset + 204 - 10,0);
	x255->setPos(axisOffset + 255 - 10,0);
}
void HistogramPanel::paintY()
{
	int scaledMaxY = -maxPixelCount/Yscale;
	
	QPen pen = Qt::black;
	QBrush brush = Qt::black;

	scene->addRect(axisOffset, 0, 1, scaledMaxY, pen, brush);
	
	scene->addRect(axisOffset, scaledMaxY, 5, 1, pen, brush);
	scene->addRect(axisOffset, scaledMaxY / 2, 5, 1, pen, brush);
	
	QGraphicsItem * yMax = scene->addText(QString::fromStdString(getExponencialView(maxPixelCount)));
	QGraphicsItem * yMaxHalf = scene->addText(QString::fromStdString(getExponencialView(maxPixelCount / 2)));
	
	yMax->setPos(0, scaledMaxY - 6);
	yMaxHalf->setPos(0, scaledMaxY / 2 - 6);
}
void HistogramPanel::paintColumns(std::map<int, int> & coordsValues, QPen pen)
{
	int x, y;
    for (auto it = coordsValues.begin(); it != coordsValues.end(); it++)
		{
			x = it->first;
			y = it->second;
			
			scene->addLine(axisOffset + x, 0, axisOffset + x, -y/Yscale, pen);
		}
}

void HistogramPanel::paintHistogram()
{
	if (spectatedImage != nullptr)
	{
		QColor paintColor;
		QPen pen;

		clearHistogram();
		reloadHistogramScene();

		if(!checkGrayScale())
		{
			paintColor = getChannelColor();
			pen.setColor(paintColor);
			channelSelector->setEnabled(true);
		}
		else
		{
			paintColor = Qt::gray;
		    pen.setColor(paintColor);
			channelSelector->setEnabled(false);

		}

		std::map<int, int> coordsValues;
		loadChannelValues(coordsValues, paintColor);
		
		Yscale = findScale(maxPixelCount);
		
		paintX();
		paintY();
		paintColumns(coordsValues, pen);
	}
}

void HistogramPanel::clearHistogram()
{
	if(scene != nullptr)
	{
		delete scene;
		scene = nullptr;
	}
	channelSelector->setEnabled(false);
}

bool HistogramPanel::checkGrayScale()
{
    if(spectatedImage->isGrayscale())
    {
        return true;
    }
	else
	{
	    return false;
	}
}

QColor HistogramPanel::getChannelColor()
{
	if (channelSelector->currentText() == "Красный канал")
	{
		return Qt::red;
	}
	else if (channelSelector->currentText() == "Зелёный канал")
	{
		return Qt::green;
	}
	else if (channelSelector->currentText() == "Синий канал")
	{
		return Qt::blue;
	}
}

void HistogramPanel::loadChannelValues(std::map<int, int> &valuesStorage, QColor color)
{
	QRgb pixel;
	int colorValue;
	maxPixelCount = 0;

	for (int x = 0; x < spectatedImage->width(); x++)
	{
		for (int y = 0; y < spectatedImage->height(); y++)
		{
			pixel = spectatedImage->pixel(x, y);

			if (color == Qt::red)
			{
				colorValue = qRed(pixel);
			}
			else if (color == Qt::green)
			{
				colorValue = qGreen(pixel);
			}
			else if (color == Qt::blue)
			{
				colorValue = qBlue(pixel);
			}
			else
			{
			    colorValue = qGray(pixel);
			}

			if (valuesStorage.count(colorValue))
			{
				valuesStorage[colorValue]++;
				
				maxPixelCount = std::max(maxPixelCount, valuesStorage[colorValue]);
			}
			else
			{
				valuesStorage[colorValue] = 1;
			}
		}
	}
}