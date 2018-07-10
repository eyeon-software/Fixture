#include "paintwidget.h"

Tool::ToolType PaintWidget::CurrentTool = Tool::Transform;

/**
 * @brief PaintWidget::PaintWidget Constructs a new PaintWidget for a new document
 * Creates a new canvas based on image path
 * @param imagePath
 * @param parent
 */
PaintWidget::PaintWidget(const QString &imagePath, QWidget *parent):
    _curMousex(0), _curMousey(0), _prevMousex(0),_prevMousey(0),
    _leftClick(false),_firstMove(false), QGraphicsView(parent)
{
    _imagePath = imagePath;
    QImage image = getImageFromPath(imagePath);

    addStyleSheet();
    setupCanvas(image);
    pushLayer(image, "Background");

    d->updateImageCanvas(_items);

    connect(d, SIGNAL(importAvailable(QString)),
            this, SLOT(addNewLayer(QString)));
}

/**
 * @brief PaintWidget::PaintWidget Constructs a new PaintWidget for a new document
 * Creates a new canvas based on Document
 * @param document
 * @param parent
 */
PaintWidget::PaintWidget(const Canvas *canvas, QWidget *parent):
    _curMousex(0), _curMousey(0), _prevMousex(0),_prevMousey(0),
    _leftClick(false),_firstMove(false), QGraphicsView(parent)
{
    setImagePath(canvas->getName());
    
    QSize imageSize(canvas->getWidth(), canvas->getHeight());

    QImage image(imageSize, QImage::Format_ARGB32_Premultiplied);
    image.fill(Qt::white);

    addStyleSheet();
    setupCanvas(image);

    pushLayer(image, "Background");
    d->updateImageCanvas(_items);

    connect(d, SIGNAL(importAvailable(QString)),
            this, SLOT(addNewLayer(QString)));
}

/**
 * @brief PaintWidget::add3NewLayer Adds a new layer based on an image
 * TODO: Support adding documents as layer
 * @param imagePath
 */
void PaintWidget::addNewLayer(const QString &imagePath)
{
    if (isFileValid(imagePath)) {
        QImage image = getImageFromPath(imagePath);
        QFileInfo info(imagePath);
        pushLayer(image, info.fileName());
        d->updateImageCanvas(_items);
     }
}

QImage PaintWidget::getImageFromPath(const QString &imagePath)
{
    if(isRaw(imagePath)){
        QImageReader reader(imagePath);
        QSize size = reader.size();

        int w = size.width() -1;
        int h = size.height() -1;

        QSize newSize = QSize(w,h);
        reader.setScaledSize(newSize);
        return reader.read();
    }
        return QImage(imagePath);
}

bool PaintWidget::isRaw(const QString &imagePath)
{
    QStringList list = imagePath.split(".");
    QString fileNameNoExt = list[1];

     QString rawExtensions[] = {
         "ARW",
         "BAY",
         "CR2",
         "DCS",
         "MOS",
         "NEF",
         "RAW"
    };

    return std::find(std::begin(rawExtensions), std::end(rawExtensions),
                            fileNameNoExt.toUpper()) != std::end(rawExtensions);
}

bool PaintWidget::isFileValid(const QString& fileName)
{
    return PaintWidget::isImageSupported(fileName);
}

bool PaintWidget::isImageSupported(const QString &fileName)
{
    QImageReader reader(fileName);
    return reader.format() != "";
}

void PaintWidget::addStyleSheet()
{
    QFile styleFile( ":/styles/paintwidget.qss" );
    styleFile.open( QFile::ReadOnly );

    QString style( styleFile.readAll() );
    setStyleSheet(style);
}

/**
 * @brief PaintWidget::setupCanvas
 * Sets up the canvas and the drawing environment to place layers
 * @param image The target QImage layer to be placed.
 */
void PaintWidget::setupCanvas(QImage image)
{
    setSceneRect(image.rect());
    d = new Drawing(this, image);

    setScene(d);
    fitInView(d->sceneRect(), Qt::KeepAspectRatio);
}

/**
 * @brief PaintWidget::pushLayer
 * @param image
 * @param name
 */
void PaintWidget::pushLayer(QImage image, const QString& name)
{
    // Needs smarter naming based on positions on the stack
    RasterLayer* l = new RasterLayer(name,image,0,0,image.width(),image.height());
    _items.push_back(l);
}

/**
 * @brief PaintWidget::wheelEvent Overrides the wheelEvent of QGraphicsView
 * Implements zooming in and out according to the position of the cursor
 * @param document
 * @param parent
 */
void PaintWidget::wheelEvent(QWheelEvent *event)
{
    const QPointF p0scene = mapToScene(event->pos());

    qreal factor = std::pow(1.001, event->delta());
    scale(factor, factor);

    const QPointF p1mouse = mapFromScene(p0scene);
    const QPointF move = p1mouse - event->pos(); // The move
    horizontalScrollBar()->setValue(move.x() + horizontalScrollBar()->value());
    verticalScrollBar()->setValue(move.y() + verticalScrollBar()->value());
}

void PaintWidget::mousePressEvent(QMouseEvent *event)
{
    _curMousex = event->x();
    _curMousey = event->y();

    if(event->button() == Qt::LeftButton){
        _leftClick = true;
    }else{
        _leftClick = false;
    }
    _firstMove = true;
    QGraphicsView::mousePressEvent(event);
}

void PaintWidget::mouseReleaseEvent(QMouseEvent *event)
{
    _firstMove = false;
    QGraphicsView::mouseReleaseEvent(event);
}

void PaintWidget::mouseMoveEvent(QMouseEvent *event)
{
    if(_leftClick){
        if(CurrentTool == Tool::Transform){
            QList<Layer*>::iterator itr = _selectedLayers.begin();
            int diffx = _curMousex - _prevMousex;
            int diffy = _curMousey - _prevMousey;
            if(!_firstMove){
                for(;itr!=_selectedLayers.end();++itr){
                    Layer* temp = *itr;
                    int x = temp->getX() + diffx;
                    int y = temp->getY() + diffy;
                    temp->setPos(x,y);
                }
            }else{
                //Check if the mouse is over any of the selected layers
                //if not then don't do anything.
            }
            _firstMove = false;
            _prevMousex = _curMousex;
            _prevMousey = _curMousey;
            _curMousex = event->x();
            _curMousey = event->y();
            d->updateImageCanvas(_items);
        }
    }

    QGraphicsView::mouseMoveEvent(event);
}
