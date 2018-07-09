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

    _currentPaintWidget = NULL;

    _toolsGroup = new QActionGroup(ui->mainToolBar);

    _select = new SelectTool();
    _toolsGroup->addAction(_select);
    _toolsList.push_back(_select);

    _pan = new PanTool();
    _toolsGroup->addAction(_pan);
    _toolsList.push_back(_pan);

    connect(ui->mainToolBar,SIGNAL(actionTriggered(QAction*)),
            this,SLOT(toolChanged(QAction*)));

    connect(ui->layerView,SIGNAL(itemSelectionChanged()),
             this,SLOT(onSelectionChange()));

    ui->mainToolBar->addActions(_toolsList);

    _select->toggle();
    _currentTool = Tool::SelectTool;
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
        _currentPaintWidget = wid;
        ui->layerView->updateItems(wid->getItems());
    } else {
        ui->layerView->clear();
    }
    ui->actionImport->setEnabled(window != NULL);
    setWindowTitle(title);
}

void MainWindow::toolChanged(QAction *action)
{
    QMdiSubWindow *currentWindow = ui->mdiArea->activeSubWindow();
    if(currentWindow != NULL){
        PaintWidget* paintWidget = qobject_cast<PaintWidget*> (currentWindow->widget());
        paintWidget->toolChanged(action);
    }
}

void MainWindow::onSelectionChange()
{
    QMdiSubWindow *currentWindow = ui->mdiArea->activeSubWindow();

    if(currentWindow != NULL){
        PaintWidget* paintWidget = qobject_cast<PaintWidget*> (currentWindow->widget());
        QList<Layer> items = paintWidget->getItems();
        QList<QListWidgetItem*> selectedItems = ui->layerView->selectedItems();
        QList<QListWidgetItem*>::iterator itrSelect = selectedItems.begin();
        QList<Layer>::iterator itrPresent = items.begin();
        QList<int> selected;
        for(int i=0;itrSelect!= selectedItems.end();++itrSelect,i++){
            for(;itrPresent != items.end();++itrPresent) {
                if(*itrSelect == itrPresent->getListItem()){
                    selected.push_back(i);
                }
            }
        }
        paintWidget->setSelectedLayers(selected);
    }
}

void MainWindow::addPaintWidget(PaintWidget *widget,bool isNew)
{
    addChildWindow(widget,isNew);

    if(_select->isChecked()){
        ui->mdiArea->setCursor(_select->getToolCursor());
    }
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


