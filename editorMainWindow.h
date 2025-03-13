#pragma once

#include <QtPlugin>
Q_IMPORT_PLUGIN(QWindowsIntegrationPlugin);

#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QAction>
#include <QFileDialog>
#include <QLabel>
#include <QMessageBox>
#include <QScrollArea>
#include <QStatusBar>
#include <QHBoxLayout>
#include <QComboBox>

#include <string>

#include "imageLabel.h"
#include "histogramPanel.h"

class EditorMainWindow: public QMainWindow
{
public:
	EditorMainWindow();
	
private:
	QMenuBar * optionsMenu;
	QMenu * fileMenu;
	QAction * openImageAction; 
	QAction * saveImageAction;
	QAction * closeImageAction;
	QAction * exitAction;
	QMenu * imageProcessingMenu;
	QAction * greyGradationsAction;
	
	QWidget * centralWidget;
	QHBoxLayout * centralBox;
	
	ImageLabel * imageViewer;
	QScrollArea * scrollArea;
	
	HistogramPanel * histogramPanel;
	
	QStatusBar * statusBar;
	QLabel * coordsLabel;
	QLabel * rgbLabel;
	
	QString openImagePath;
	QString saveImagePath;
	
	void createMenuBar();
	void createCentralPanel();
	void createImagePanel();
	void createHistogramPanel();
	void createStatusBar();
	
	void openImage();
	void saveImage();
	void closeImage();
	
	void convertToGrey();

	void callError(std::string errorText);
};