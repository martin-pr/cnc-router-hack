#pragma once

#include <QWidget>
#include <QImage>

class Canvas : public QWidget {
	Q_OBJECT

	public:
		Canvas(QWidget* parent);
		virtual ~Canvas();

		QImage& image();
		const QImage& image() const;
		void setImage(const QImage& i);

		const float& zoom() const;
		void setZoom(float zoom);

	protected:
		virtual void paintEvent(QPaintEvent* e);

	private:
		QImage m_image;

		float m_zoom;
};
