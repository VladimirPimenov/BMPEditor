#pragma once

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsTextItem>
#include <QLabel>
#include <QComboBox>
#include <QImage>
#include <QPushButton>

#include <map>
#include <string>
#include <cmath>

class HistogramPanel: public QWidget
{
public:
	HistogramPanel();


	QImage * spectatedImage = nullptr;

	void paintHistogram();

	void clearHistogram();
private:
	QComboBox * channelSelector;
	
	QVBoxLayout * panel;

	QGraphicsView * histogram;
	QGraphicsScene * scene = nullptr;
	
	int axisOffset;
	int maxPixelCount;
	double Yscale;

	void paintX();
	void paintY();
	void paintColumns(std::map<int, int> & coordsValues, QPen pen);

	QColor getChannelColor();
	void loadChannelValues(std::map<int, int> & valuesStorage, QColor color);
	
	bool checkGrayScale();
	
	void createHistogramPanel();
	void reloadHistogramScene();
	void createChannelSelector();
};


