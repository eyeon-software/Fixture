#include "PaintWidget.h"

Fixture::PaintWidget::PaintWidget(QWidget *parent):
    QAbstractScrollArea(parent)
{
    _canvas = new Fixture::Canvas(parent);
    setViewport(_canvas);
}
