#include "Window.h"

#include <iostream>

#include <QMenu>
#include <QMenuBar>
#include <QStyle>
#include <QFileDialog>
#include <QProcess>
#include <QMessageBox>
#include <QLabel>
#include <QScrollArea>
#include <QToolBar>

#include "Canvas.h"

using std::cout;
using std::endl;

namespace {
	const float DENSITY = 300;
}

Window::Window(QWidget* parent) : QMainWindow(parent) {
	QMenu* fileMenu = menuBar()->addMenu("&File");
	QAction* open = fileMenu->addAction(QIcon::fromTheme("document-open"), "&Open...");
	fileMenu->addSeparator();
	QAction* quit = fileMenu->addAction(QIcon::fromTheme("application-exit"), "&Quit");

	QMenu* viewMenu = menuBar()->addMenu("&View");
	QAction* zoomFull = viewMenu->addAction(QIcon::fromTheme("zoom-original"), "Zoom 100%");
	QAction* zoomIn = viewMenu->addAction(QIcon::fromTheme("zoom-in"), "Zoom in");
	QAction* zoomOut = viewMenu->addAction(QIcon::fromTheme("zoom-out"), "Zoom out");

	connect(open, SIGNAL(triggered()), this, SLOT(onOpenFile()));
	connect(quit, SIGNAL(triggered()), this, SLOT(close()));

	connect(zoomFull, SIGNAL(triggered()), this, SLOT(onZoomFull()));
	connect(zoomIn, SIGNAL(triggered()), this, SLOT(onZoomIn()));
	connect(zoomOut, SIGNAL(triggered()), this, SLOT(onZoomOut()));

	////

	QToolBar* toolbar = addToolBar("tools");

	toolbar->addAction(open);

	toolbar->addSeparator();

	toolbar->addAction(zoomOut);
	toolbar->addAction(zoomFull);
	toolbar->addAction(zoomIn);

	toolbar->addSeparator();

	toolbar->addAction(quit);

	////

	m_content = new Canvas(this);

	QScrollArea* scrollArea = new QScrollArea;
	scrollArea->setBackgroundRole(QPalette::Dark);
	scrollArea->setWidget(m_content);
	setCentralWidget(scrollArea);
}

Window::~Window() {
}

void Window::onOpenFile() {
	QString file = QFileDialog::getOpenFileName(this, "Open an EPS file...", "", "EPS files (*.eps)");
	if(file != "") {
		QProcess convertor(this);
		QStringList arguments;
		arguments << "-density" << QString::number(DENSITY) << "+antialias"
			<< file << "png:-";
		convertor.start("convert", arguments);
		convertor.waitForFinished();

		if(convertor.exitCode() != 0) {
			QMessageBox::critical(this, "Error loading file...",
					"error output:\n" + convertor.readAllStandardError());
		}

		QImage image;
		image.loadFromData(convertor.readAllStandardOutput());

		m_content->setImage(image);
	}
}

void Window::onZoomFull() {
	m_content->setZoom(1.0f);
}

void Window::onZoomIn() {
	m_content->setZoom(m_content->zoom() * 2.0f);
}

void Window::onZoomOut() {
	m_content->setZoom(m_content->zoom() / 2.0f);
}
