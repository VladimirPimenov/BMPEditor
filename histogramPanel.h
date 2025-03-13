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
#include <iostream>

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
	QGraphicsScene * scene;
	
	int axisOffset;
	int maxFrequencyValue;
	double Yscale = 1;

	void paintAxis();

	QColor getChannelColor();
	void getChannelValues(std::map<int, int> & valuesStorage, QColor color);
	
	bool checkGrayScale();
	
	void createChannelSelector();
};


