#include <QApplication>

#include "Window.h"

int main(int argc, char **argv) {
	QApplication app (argc, argv);

	Window win(NULL);
	win.showMaximized();

	return app.exec();
}
