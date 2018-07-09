#ifndef DRAWING_H
#define DRAWING_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QImageReader>
#include <QWheelEvent>
#include <QScrollBar>
#include <QObject>
#include <QWidget>
#include <QGraphicsSceneDragDropEvent>
#include <QMimeData>

#include "layer.h"

class Drawing : public QGraphicsScene
{
    Q_OBJECT

public:
    Drawing(QWidget *widget,QImage &image);
    ~Drawing();

    void initialize(const QImage &image);
    void updateImageCanvas(QList<Layer>items);

    QGraphicsPixmapItem *_canvas;
private:
    int _height,_width;
    QImage _image;

    void dragEnterEvent(QGraphicsSceneDragDropEvent *e);
    void dragMoveEvent(QGraphicsSceneDragDropEvent *e);
    void dropEvent(QGraphicsSceneDragDropEvent *e);

signals:
    void importAvailable(const QString &fileName);

};

#endif // DRAWING_H
