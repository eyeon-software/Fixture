#ifndef LAYER_H
#define LAYER_H

#include <QString>
#include <QImage>
#include <QListWidgetItem>

class Layer : public QListWidgetItem
{
public:
    enum LayerType{
        RasterLayer,
        VectorLayer,
        TextLayer,
        AdjustmentLayer
    };

    Layer(QString name,LayerType type,
          int x, int y,
          int height, int width);

    ~Layer();

    inline QString getName() { return _name; }
    inline int getX() { return _x; }
    inline int getY() { return _y; }
    inline int getWidth() { return _width; }
    inline int getHeight() { return _height; }
    inline LayerType getType() { return _type; }

    inline void setWidth(int width) {  _width = width; }
    inline void setHeight(int height) { _height = height; }
    void setScale(double factor);
    void setPos(int x, int y);

    /*virtual void changeBrightness(double delta) = 0;
    virtual void changeSaturation(double delta) = 0;
    virtual void changeContrast(double delta) = 0;*/

private:
    QString _name;
    LayerType _type;
    int _x,_y,_width,_height;
    double _brightness,_contrast,_saturation;
};

#endif // LAYER_H
