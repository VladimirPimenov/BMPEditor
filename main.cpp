#include <QApplication>
#include "editorMainWindow.h"

int main(int argc, char * argv[])
{
	QApplication app(argc, argv);

	EditorMainWindow * window = new EditorMainWindow();
	
	window->setWindowTitle("BMPEditor");
	window->resize(720, window->height());
	window->resize(1080, window->width());
	window->show();
	
	return app.exec();
}