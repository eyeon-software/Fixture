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
#include "../tools/transform.h"
#include "../tools/pan.h"
#include "../tools/tool.h"
#include "../layers/rasterlayer.h"

class PaintWidget : public QGraphicsView
{
    Q_OBJECT
public:
    PaintWidget(const QString &imagePath, Tool *tool, QWidget *parent=0);
    PaintWidget(const Canvas *document, Tool *tool, QWidget *parent=0);
    
    inline void setImagePath(QString path) { _imagePath = path; }
    inline QString getImagePath() const { return _imagePath; }
    inline QList<Layer*> getItems() const { return _items; }
    // This has to be extended to accomodate new documents
    inline void updateLayers(QList<Layer*> items){ d->updateImageCanvas(items); }
    void setSelectedLayers(QList<Layer*> layers);
    void setTool(Tool *tool);
    static bool isFileValid(const QString& fileName);

    static Tool::ToolType CurrentTool;

public slots:
    void addNewLayer(const QString &imagePath);

signals:
    void layersSelected(QList<Layer*> layers);

protected:
    void wheelEvent(QWheelEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

private:
    int _curMousex, _curMousey,_prevMousex,_prevMousey;
    bool _leftClick,_firstMove;

    Drawing *d;
    QString _imagePath;
    QList<Layer*> _items;
    QList<Layer*> _selectedLayers;

    QImage getImageFromPath(const QString &imagePath);
    void setupCanvas(QImage image);
    void pushLayer(QImage image, const QString &name);
    bool isRaw(const QString &imagePath);
    void addStyleSheet();

    static bool isImageSupported(const QString& fileName);
    Tool  *_currentTool;
};

#endif // PAINTWIDGET_H
