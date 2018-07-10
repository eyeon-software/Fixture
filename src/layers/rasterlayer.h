#ifndef RASTERLAYER_H
#define RASTERLAYER_H

#include "layer.h"

class RasterLayer : public Layer
{
public:
    RasterLayer(QString name,QImage image,
                int x, int y,
                int height, int width);
    ~RasterLayer();

    inline QImage getBitmap() { return _image; }

private:
    QImage _image;
};

#endif // RASTERLAYER_H
