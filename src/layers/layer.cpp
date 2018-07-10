#include "layer.h"

/**
 * @brief Layer::layer Constructs a new Layer for storing Raster Images
 * @param name
 * @param image
 * @param x, y
 * @param height, width
 */
Layer::Layer(QString name, LayerType type,
             int x, int y, int height, int width):
    _name(name),_type(type),
    _x(x), _y(y),_width(width), _height(height)
{
    _brightness = 1;
    _contrast = 1;
    _saturation = 1;
    setText(name);
    setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    setSizeHint(QSize(0,40));
}

Layer::~Layer()
{
}

void Layer::setScale(double factor)
{
    _width *= factor;
    _height *= factor;
}

void Layer::setPos(int x, int y)
{
    _x = x;
    _y = y;
}
