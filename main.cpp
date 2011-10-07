#include <QApplication>
#include "window.h"

int main(int argc, char ** argv)
{
	QApplication app(argc, argv);
	Window gamewindow;
	gamewindow.show();
	return app.exec();
}
