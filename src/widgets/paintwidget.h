#ifndef PAINTWIDGET_H
#define PAINTWIDGET_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QLabel>
#include <QGraphicsItem>
#include <QImageReader>
#include <QWheelEvent>
#include <QScrollBar>
#include <QObject>
#include <QFileInfo>
#include <QMimeData>
#include <QGraphicsSceneDragDropEvent>
#include <QAction>

#include <iostream>
#include <cmath>

#include "../model/drawing.h"
#include "../widgets/paintwidget.h"
#include "../model/canvas.h"
#include "../tools/abstractselection.h"
#include "../tools/tool.h"
#include "../layers/rasterlayer.h"
#include "../tools/transform.h"
#include "../model/document.h"

class PaintWidget : public QGraphicsView
{
    Q_OBJECT
public:

    PaintWidget(const QString &imagePath, Tool *tool, QWidget *parent=0);
    PaintWidget(const QSharedDataPointer<Canvas> canvas, Tool *tool, QWidget *parent=0);
    PaintWidget(Document &document, Tool *tool, QWidget *parent=0);

    void pushLayer(Layer *layer);

    inline QSharedDataPointer<Canvas> getCanvas() { return _canvas; }
    inline void setImagePath(QString path) { _imagePath = path; }
    inline QString getImagePath() const { return _imagePath; }
    inline QList<Layer*> getItems() const { return _items; }
    // This has to be extended to accomodate new documents
    void setSelectedLayers(QList<Layer*> layers);
    void setTool(Tool *tool);
    static bool isFileValid(const QString& fileName);
    static RasterLayer *getLayerFromImage(const QImage &image, const QString &name);

public slots:
    void importPathToLayer(const QString &fileName);
    void setSelectedLayers();

signals:
    void layersSelected(QList<Layer*> layers);

protected:
    void wheelEvent(QWheelEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

private:
    Drawing *d;
    QString _imagePath;
    QList<Layer*> _items;


    QImage getImageFromPath(const QString &imagePath);
    void createBgLayer(const QImage &image);
    QImage drawEmptyImage(const QSharedDataPointer<Canvas> canvas);
    void prepareDocument(Tool *tool, QRect rect);
    void setupCanvas(QRect rect);
    bool isRaw(const QString &imagePath);
    void addStyleSheet();

    static bool isImageSupported(const QString& fileName);
    Tool  *_currentTool;
    QSharedDataPointer<Canvas> _canvas;
};

#endif // PAINTWIDGET_H
