#include "PaintWidget.h"

Fixture::PaintWidget::PaintWidget(QWidget *parent):
    QAbstractScrollArea(parent)
{
    _canvas = new Fixture::Canvas(parent);
    setViewport(_canvas);
}

void
Fixture::PaintWidget::paintEvent(QPaintEvent *event)
{
    _canvas->paintEvent(event);
}
