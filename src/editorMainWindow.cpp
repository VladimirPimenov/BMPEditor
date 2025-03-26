#include "../include/editorMainWindow.h"

EditorMainWindow::EditorMainWindow():QMainWindow()
{
	createMenuBar();
	
	createCentralPanel();
	
	createImagePanel();
	createHistogramPanel();
	
	createStatusBar();	
}

void EditorMainWindow::createMenuBar()
{
	fileMenu = new QMenu("Файл");
	
	openImageAction = fileMenu->addAction("Открыть");
	saveImageAction = fileMenu->addAction("Сохранить");
	closeImageAction = fileMenu->addAction("Закрыть");
	exitAction = fileMenu->addAction("Выход",this, SLOT(close()));
	
	connect(openImageAction, &QAction::triggered, this, &EditorMainWindow::openImage);
	connect(saveImageAction, &QAction::triggered, this, &EditorMainWindow::saveImage);
	connect(closeImageAction, &QAction::triggered, this, &EditorMainWindow::closeImage);
	
	menuBar()->addMenu(fileMenu);
	
	imageProcessingMenu = new QMenu("Обработка");
	greyGradationsAction = imageProcessingMenu->addAction("Градации серого");
	
	connect(greyGradationsAction, &QAction::triggered, this, &EditorMainWindow::convertToGrey);
	
	menuBar()->addMenu(imageProcessingMenu);
}
void EditorMainWindow::createCentralPanel()
{
	centralWidget = new QWidget();
	this->setCentralWidget(centralWidget);
	centralBox = new QHBoxLayout(centralWidget);
}
void EditorMainWindow::createImagePanel()
{
	scrollArea = new QScrollArea();
	scrollArea->setWidgetResizable(true);
	imageViewer = new ImageLabel();
	scrollArea->setWidget(imageViewer);
	imageViewer->setAlignment(Qt::AlignCenter);
	imageViewer->setMouseTracking(true);
	
	centralBox->addWidget(scrollArea);
}
void EditorMainWindow::createHistogramPanel()
{	
	histogramPanel = new HistogramPanel();

	centralBox->addWidget(histogramPanel);
}
void EditorMainWindow::createStatusBar()
{
	statusBar = new QStatusBar();
	this->setStatusBar(statusBar);
	
	coordsLabel = new QLabel("Координаты");
	coordsLabel->setAlignment(Qt::AlignRight);
	imageViewer->coordsLabel = coordsLabel;
	
	rgbLabel = new QLabel("RGB");
	rgbLabel->setAlignment(Qt::AlignRight);
	imageViewer->rgbLabel = rgbLabel;
	
	statusBar->addWidget(coordsLabel, 1);
	statusBar->addWidget(rgbLabel, 1);
}

void EditorMainWindow::callError(std::string errorText)
{
	QMessageBox errorWindow;
	errorWindow.setText(QString::fromStdString(errorText));
	errorWindow.setWindowTitle("BMPEditor - ERROR");
	errorWindow.exec();
}

void EditorMainWindow::openImage()
{
	openImagePath = QFileDialog::getOpenFileName(this, "Открыть файл", "./", "BMP (*.bmp)");
	
	imageViewer->loadBMP(openImagePath.toStdString());

	imageViewer->updateImage();

	histogramPanel->spectatedImage = imageViewer->image;
	histogramPanel->paintHistogram();
}
void EditorMainWindow::saveImage()
{
	if(imageViewer->hasImage())
	{
		saveImagePath = QFileDialog::getSaveFileName(this, "Сохранить файл", "./output.bmp", "BMP (*.bmp)");
	
		imageViewer->saveBMP(saveImagePath.toStdString());
	}
	else{callError("Не выбрано изображение!");}
}
void EditorMainWindow::closeImage()
{
	imageViewer->clearImageLabel();
	histogramPanel->clearHistogram();
}

void EditorMainWindow::convertToGrey()
{
	if(imageViewer->hasImage())
	{
		imageViewer->convertImageToGrey();
		imageViewer->updateImage();

		histogramPanel->spectatedImage = imageViewer->image;
		histogramPanel->paintHistogram();
	}
	else{callError("Не выбрано изображение!");}
}