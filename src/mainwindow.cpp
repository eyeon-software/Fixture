#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->mdiArea->setViewMode(QMdiArea::TabbedView);
    ui->mdiArea->setTabsClosable(true);
    ui->mdiArea->setTabsMovable(true);
    QTabBar *bar=ui->mdiArea->findChild<QTabBar*>();
    bar->setExpanding(false);
    bar->setDrawBase(false);
    bar->setElideMode(Qt::ElideLeft);

    setAcceptDrops(true);
    connect(ui->mdiArea, SIGNAL(subWindowActivated(QMdiSubWindow*)),
                     this, SLOT(updateWindow(QMdiSubWindow*)));

    _lastFileLoc = QStandardPaths::writableLocation(QStandardPaths::HomeLocation);

    connect(ui->layerView,SIGNAL(itemschanged()),this,SLOT(updateLayers()));
    connect(ui->panTool,SIGNAL(toggled(bool)),this,SLOT(setPanTool(bool)));

    _toolsGroup = new QActionGroup(ui->mainToolBar);
    _toolsGroup->addAction(ui->selectTool);
    _toolsList.push_back(ui->selectTool);
    _toolsGroup->addAction(ui->panTool);
    _toolsList.push_back(ui->panTool);
    ui->mainToolBar->addActions(_toolsList);

    ui->selectTool->toggle();
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::dragEnterEvent(QDragEnterEvent *e)
{
    if (e->mimeData()->hasUrls()) {
        e->acceptProposedAction();
    }
}

void MainWindow::dropEvent(QDropEvent *e)
{
    foreach (const QUrl &url, e->mimeData()->urls()) {
        QString fileName = url.toLocalFile();

        if (PaintWidget::isFileValid(fileName)) {
            rememberLastPath(fileName);
            addPaintWidget(new PaintWidget(fileName));
        }
    }
}

void MainWindow::updateLayers()
{
    QMdiSubWindow* window = ui->mdiArea->activeSubWindow();
    PaintWidget* wid = qobject_cast<PaintWidget*> (window->widget());
    QList<Layer> updatedLayers = ui->layerView->getitems();
    wid->updateLayers(updatedLayers);
}

void MainWindow::addChildWindow(PaintWidget *widget,bool isNew)
{
    ui->mdiArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    ui->mdiArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    QMdiSubWindow *mdiSubWindow = ui->mdiArea->addSubWindow(widget);

    QString title;
    if (widget->getImagePath() != "") {
        QFileInfo info(widget->getImagePath());
        title = info.fileName() + "[*]";
    } else {
        title = "Untitled[*]";
    }

    mdiSubWindow->setWindowTitle(title);

    if (isNew) {
        mdiSubWindow->setWindowModified(true);
    }

    mdiSubWindow->installEventFilter(this);
    mdiSubWindow->show();
}

/**
 * @brief MainWindow::updateWindow
 * @param window
 */
void MainWindow::updateWindow(QMdiSubWindow *window)
{
    QString title = "Fixture";

    if (window != NULL) {
        title = window->windowTitle() + " - " + title;
        PaintWidget* wid = qobject_cast<PaintWidget*> (window->widget());
        ui->layerView->updateItems(wid->getItems());
    } else {
        ui->layerView->clear();
    }
    ui->actionImport->setEnabled(window != NULL);
    setWindowTitle(title);
}

void MainWindow::addPaintWidget(PaintWidget *widget,bool isNew)
{
    addChildWindow(widget,isNew);
}

PaintWidget *MainWindow::createPaintWidget(const QString &imagePath) const
{
    return new PaintWidget(imagePath);
}

void MainWindow::on_actionOpen_triggered()
{
    const QString fileName = chooseFile();

    if (PaintWidget::isFileValid(fileName)) {
        rememberLastPath(fileName);
        addPaintWidget(new PaintWidget(fileName));
    }
}
 void MainWindow::rememberLastPath(const QString &fileName)
 {
     QFileInfo info(fileName);
     _lastFileLoc = info.absolutePath();
 }

const QString MainWindow::chooseFile()
{

    return QFileDialog::getOpenFileName(this, tr("Open File"),
                _lastFileLoc,tr("Image Files (*.png *.jpg *.jpeg *.gif);;"
                                "PNG(*.png);;"
                                "JPEG(*.jpg *.jpeg);;"
                                "GIF(*.gif);;"
                                "TIFF(*.tif *.tiff);;"
                                "BMP(*.bmp);;"
                                "ICO(*.ico)"));
}

void MainWindow::on_actionExit_triggered()
{
    QApplication::exit();
}

void MainWindow::on_actionNew_triggered()
{
    NewDialog *newDialog = new NewDialog();
    connect(newDialog, SIGNAL(canvasAvailable(const Canvas*)), this, SLOT(createNewDocument(const Canvas*)));

    newDialog->show();
}

void MainWindow::createNewDocument(const Canvas *canvas)
{
    addPaintWidget(new PaintWidget(canvas), true);
    delete canvas;
}

void MainWindow::on_actionImport_triggered()
{
    const QString fileName = chooseFile();


    QMdiSubWindow *currentWindow = ui->mdiArea->activeSubWindow();
    PaintWidget* paintWidget = qobject_cast<PaintWidget*> (currentWindow->widget());
    paintWidget->addNewLayer(fileName);
    ui->layerView->updateItems(paintWidget->getItems());
}

void MainWindow::setPanTool(bool selected)
{
    QMdiSubWindow *currentWindow = ui->mdiArea->activeSubWindow();

    if(currentWindow != NULL){
        PaintWidget* paintWidget = qobject_cast<PaintWidget*> (currentWindow->widget());
        if(selected){
            paintWidget->setDragMode(QGraphicsView::ScrollHandDrag);
        }else{
            paintWidget->setDragMode(QGraphicsView::NoDrag);
        }
    }
}
