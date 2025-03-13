#include "histogramPanel.h"

int findMaxPixelCount(std::map<int, int> map)
{
    int max = 0;
    
    for (auto it = map.begin(); it != map.end(); it++)
	{
		if(it->second > max)
		{
		    max = it->second;
		}
	}
    
    return max;
}

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

HistogramPanel::HistogramPanel() : QWidget()
{
	this->setFixedWidth(310);
	axisOffset = 5;

	panel = new QVBoxLayout(this);

	createChannelSelector();

	scene = new QGraphicsScene();
	histogram = new QGraphicsView();
	histogram->setScene(scene);
	histogram->setFixedSize(300, 160);
	panel->addWidget(histogram);
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

void HistogramPanel::paintAxis()
{
	QPen coordsPen(Qt::black);
	QBrush brush(Qt::black);

	scene->addRect(axisOffset, 0, 260, 1, coordsPen, brush);

	scene->addRect(axisOffset + 51, 0, 1, -5, coordsPen, brush);
	scene->addRect(axisOffset + 102, 0, 1, -5, coordsPen, brush);
	scene->addRect(axisOffset + 153, 0, 1, -5, coordsPen, brush);
	scene->addRect(axisOffset + 204, 0, 1, -5, coordsPen, brush);
	scene->addRect(axisOffset + 255, 0, 1, -5, coordsPen, brush);

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
void HistogramPanel::paintHistogram()
{
	if (spectatedImage != nullptr)
	{
		QColor paintColor;
		QPen pen;

		this->clearHistogram();

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
		getChannelValues(coordsValues, paintColor);

		int x;
		int y;
		
		int maxPixelCount = findMaxPixelCount(coordsValues);
		int yScale = findScale(maxPixelCount);
		
		for (auto it = coordsValues.begin(); it != coordsValues.end(); it++)
		{
			x = it->first;
			y = it->second;
			
			if(x > 0)
			{
				scene->addLine(axisOffset + x, 0, axisOffset + x, -y/yScale, pen);
			}
		}

		paintAxis();
		
		scene->addRect(axisOffset, 0, 1, -maxPixelCount/yScale, QPen(Qt::black), QBrush(Qt::black));
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

void HistogramPanel::clearHistogram()
{
	scene->clear();
	
	channelSelector->setEnabled(false);
}

void HistogramPanel::getChannelValues(std::map<int, int> &valuesStorage, QColor color)
{
	QRgb pixel;
	int colorValue;
	maxFrequencyValue = 0;

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
				
				if(valuesStorage[colorValue] > maxFrequencyValue)
				{
				    maxFrequencyValue = valuesStorage[colorValue];
				}
			}
			else
			{
				valuesStorage[colorValue] = 1;
			}
		}
	}
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