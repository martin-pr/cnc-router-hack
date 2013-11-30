#pragma once

#include <QMainWindow>

class Canvas;

class Window : public QMainWindow {
	Q_OBJECT

	public:
		Window(QWidget* parent);
		virtual ~Window();

	protected:
	private:
		Canvas* m_content;

	private slots:
		void onOpenFile();

		void onZoomFull();
		void onZoomIn();
		void onZoomOut();
};
