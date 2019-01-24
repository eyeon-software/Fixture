#include "FixtureCanvas.h"

Fixture::Canvas::Canvas(QWidget *widget):
    QOpenGLWidget(widget)
{
    setAutoFillBackground(false);
}

void
Fixture::Canvas::paintEvent(QPaintEvent *event)
{
    event->accept();
}
