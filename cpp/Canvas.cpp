#include "Canvas.h"

#include <QWheelEvent>
#include <QPainter>

#include <iostream>
#include <cassert>

using std::cout;
using std::endl;

Canvas::Canvas(QWidget* parent) : QWidget(parent), m_zoom(1.0f) {
	setMinimumSize(500, 500);
	setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}

Canvas::~Canvas() {
}

QImage& Canvas::image() {
	return m_image;
}

const QImage& Canvas::image() const {
	return m_image;
}

void Canvas::setImage(const QImage& i) {
	m_image = i;

	m_zoom = 1;
	setMinimumSize(m_image.size());
	resize(m_image.size());

	repaint();
}

void Canvas::paintEvent(QPaintEvent* e) {
	QPainter painter(this);

	// background
	painter.fillRect(e->rect(), QColor(255, 255, 255));

	// the default image
	if(m_image.isNull()) {
		painter.setPen(QColor(0,0,0));
		for(int y=e->rect().top() / 10 * 10; y <= e->rect().bottom() / 10 * 10; y += 10)
		 	for(int x=e->rect().left() / 10 * 10; x <= e->rect().right() / 10 * 10; x += 10)
		 		painter.drawPoint(x,y);
	}

	else {
		QRectF target((float)e->rect().x(), (float)e->rect().y(), (float)e->rect().width(), (float)e->rect().height());
		QRectF source((float)e->rect().x() / m_zoom, (float)e->rect().y() / m_zoom, (float)e->rect().width() / m_zoom, (float)e->rect().height() / m_zoom);

		painter.drawImage(target, m_image, source);
	}
}

const float& Canvas::zoom() const {
	return m_zoom;
}

void Canvas::setZoom(float zoom) {
	assert(zoom > 0.0f);

	m_zoom = zoom;
	setMinimumSize(m_image.size() * m_zoom);
	resize(m_image.size() * m_zoom);

	repaint();
}
