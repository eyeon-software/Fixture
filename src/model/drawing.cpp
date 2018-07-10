#include "drawing.h"

Drawing::Drawing(QWidget *widget,QImage &image) :
    QGraphicsScene(widget)
{
    _canvas = addPixmap(QPixmap::fromImage(image));
    _image = image;
    _width = image.width();
    _height = image.height();

}

Drawing::~Drawing()
{

}

void Drawing::updateImageCanvas(QList<Layer *> items)
{
    QImage surface = QImage(_width,_height, QImage::Format_ARGB32_Premultiplied);
    QPainter painter(&surface);

    QBrush brush;
    brush.setTextureImage(QImage(":/brushes/checkers.png"));
    painter.setBrush(brush);
    painter.setCompositionMode(QPainter::CompositionMode_Source);
    painter.fillRect(surface.rect(), brush);
    painter.setCompositionMode(QPainter::CompositionMode_SourceOver);

    QList<Layer*>::iterator itr = items.begin();
    for(;itr != items.end();++itr){
        switch((*itr)->getType()){
        case Layer::RasterLayer:
            RasterLayer *l = dynamic_cast<RasterLayer*>(*itr);
            QImage img = l->getBitmap();
            img.scaled(l->getWidth(),l->getHeight());
            painter.drawImage(l->getX(), l->getY(), img);
            break;
        }
    }

    painter.end();
    _canvas->setPixmap(QPixmap::fromImage(surface));
}

void Drawing::dragEnterEvent(QGraphicsSceneDragDropEvent *e)
{
    if (e->mimeData()->hasUrls()) {
        e->acceptProposedAction();
    }
}

void Drawing::dragMoveEvent(QGraphicsSceneDragDropEvent *e)
{
    if (e->mimeData()->hasUrls()) {
        e->acceptProposedAction();
    }
}

void Drawing::dropEvent(QGraphicsSceneDragDropEvent *e)
{
    foreach (const QUrl &url, e->mimeData()->urls()) {
        QString fileName = url.toLocalFile();
        emit importAvailable(fileName);
    }
}
