#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setupMdiArea();
    initTools();
    initSignalsAndSlots();
    initSupportedFileMap();
    initFilterListString();
    Transform *transform = dynamic_cast<Transform*>(_toolsList.value(0));
    setDefaultTool(transform);
    _lastFileLoc = QStandardPaths::writableLocation(QStandardPaths::HomeLocation);

    setAcceptDrops(true);
}

void MainWindow::initSupportedFileMap()
{
    _supportedTypeMap.insert(FXD, "Fixture document (*.fxd *.fxt);;");
    _supportedTypeMap.insert(IMAGE, "Image Files (*.png *.jpg *.jpeg *.gif);;");
    _supportedTypeMap.insert(PNG, "PNG (*.png);;");
    _supportedTypeMap.insert(JPG, "JPEG (*.jpg *.jpeg);;");
    _supportedTypeMap.insert(GIF, "GIF (*.gif);;");
    _supportedTypeMap.insert(TIFF, "TIFF (*.tif *.tiff);;");
    _supportedTypeMap.insert(ICO, "ICO (*.ico);;");
    _supportedTypeMap.insert(ICO, "BMP (*.bmp)");

}

void MainWindow::initFilterListString()
{
    QMap<SupportedTypes, QString>::iterator i;

    for (i = _supportedTypeMap.begin(); i != _supportedTypeMap.end(); ++i) {
        _filterListString += i.value();
    }
}
MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupMdiArea()
{
    ui->mdiArea->setViewMode(QMdiArea::TabbedView);
    ui->mdiArea->setTabsClosable(true);
    ui->mdiArea->setTabsMovable(true);

    QTabBar *bar=ui->mdiArea->findChild<QTabBar*>();
    setupTabBar(bar);
}

void MainWindow::setupTabBar(QTabBar *bar)
{
    bar->setExpanding(false);
    bar->setDrawBase(false);
    bar->setElideMode(Qt::ElideLeft);
}

void MainWindow::initSignalsAndSlots()
{
    connect(ui->mdiArea, SIGNAL(subWindowActivated(QMdiSubWindow*)),
                     this, SLOT(updateWindow(QMdiSubWindow*)));

    connect(ui->mainToolBar,SIGNAL(actionTriggered(QAction*)),
            this,SLOT(changeTool(QAction*)));

    connect(ui->layerView,SIGNAL(itemSelectionChanged()),
             this,SLOT(onSelectionChange()));    
}

void MainWindow::initTools()
{
    _toolsGroup = new QActionGroup(ui->mainToolBar);

    addTool(new Transform());
    addTool(new Pan());

    ui->mainToolBar->addActions(_toolsList);
}

void MainWindow::addTool(Tool *tool)
{
    _toolsGroup->addAction(tool);
    _toolsList.push_back(tool);
}

void MainWindow::setDefaultTool(Tool *tool)
{
    tool->toggle();

    QWidget *menuWidget = tool->getToolMenu();
    activateMenu(menuWidget);
    _currentTool = tool;
}

void MainWindow::setCurrentTool(Tool *tool)
{
    _currentTool = tool;

    QWidget *menuWidget = tool->getToolMenu();
    if (_menu != NULL) {
        _menu->setVisible(false);
    }
    updateMenuFromCache(menuWidget);

    // TODO: Remember preferences
    _menu->setVisible(true);
}

void MainWindow::updateMenuFromCache(QWidget *menuWidget)
{
    if (_toolMenuCache.contains(menuWidget)) {
        _menu = _toolMenuCache.value(menuWidget);
    }
    else {
        activateMenu(menuWidget);
    }
}

void MainWindow::activateMenu(QWidget *menuWidget)
{
    _menu = ui->toolMenuBar->addWidget(menuWidget);
    _toolMenuCache.insert(menuWidget, _menu);
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
        openNewImage(fileName);
    }
}

void MainWindow::openNewImage(const QString &fileName)
{
    if (PaintWidget::isFileValid(fileName)) {
        rememberLastPath(fileName);
        addPaintWidget(new PaintWidget(fileName, _currentTool));
    }
}
void MainWindow::addChildWindow(PaintWidget *widget,bool isNew)
{
    ui->mdiArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    ui->mdiArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    QMdiSubWindow *mdiSubWindow = ui->mdiArea->addSubWindow(widget);

    setupSubWindowTitle(mdiSubWindow, widget->getImagePath(), isNew);
    if (widget->getImagePath() != "") {
        _windowCache.insert(widget->getImagePath(), mdiSubWindow);
    }

    mdiSubWindow->installEventFilter(this);
    mdiSubWindow->show();
}

void MainWindow::setupSubWindowTitle(QMdiSubWindow *mdiSubWindow, const QString& filePath, bool isNew)
{
    QString title = "Untitled[*]";

    if (filePath != "") {
        QFileInfo info(filePath);
        title = info.fileName() + "[*]";
    }
    mdiSubWindow->setWindowTitle(title);
    mdiSubWindow->setWindowModified(isNew);
}

bool MainWindow::eventFilter(QObject *watched, QEvent *event)
{
    switch (event->type()) {
      case QEvent::Close: {
          QMdiSubWindow * subWindow = dynamic_cast<QMdiSubWindow*>(watched);
          Q_ASSERT (subWindow != NULL);

          _windowCache.remove(_windowCache.key(subWindow));
          break;
      }
      default:
          qt_noop();
    }
    return QObject::eventFilter(watched, event);
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

    updateActions(window != NULL);
    setWindowTitle(title);
}

void MainWindow::updateActions(bool val)
{
    ui->actionImport->setEnabled(val);
    ui->actionSave->setEnabled(val);
    ui->actionSaveAs->setEnabled(val);
}

void MainWindow::changeTool(QAction *action)
{
    setCurrentTool(dynamic_cast<Tool*>(action));

    QMdiSubWindow *currentWindow = ui->mdiArea->activeSubWindow();
    if(currentWindow != NULL){
        updateToolForCurrentWindow(currentWindow);
    }
}

void MainWindow::updateToolForCurrentWindow(QMdiSubWindow *currentWindow)
{
    PaintWidget* paintWidget = qobject_cast<PaintWidget*> (currentWindow->widget());
    paintWidget->setTool(_currentTool);
    updateDragMode(paintWidget);
}

void MainWindow::updateDragMode(PaintWidget *paintWidget)
{
    if (_currentTool->getToolGroup() == Tool::PERCEPTION){
        //The cursor needs to be fixed for zoom here.
        paintWidget->setDragMode(QGraphicsView::ScrollHandDrag);
    }
    else {
        paintWidget->setDragMode(QGraphicsView::NoDrag);
    }
}

void MainWindow::onSelectionChange()
{
    QMdiSubWindow *currentWindow = ui->mdiArea->activeSubWindow();

    if(currentWindow != NULL){
        updateLayerSelection();
    }
}

void MainWindow::updateLayerSelection()
{
    QList<QListWidgetItem*> widgetSelectedItems = ui->layerView->selectedItems();
    QList<QListWidgetItem*> widgetItems = ui->layerView->getitems();
    QList<QListWidgetItem*>::iterator itr = widgetItems.begin();

    for(;itr != widgetItems.end();++itr){
        Layer *l = dynamic_cast<Layer*>(*itr);
        l->setSceneSelected(widgetSelectedItems.contains(*itr));
    }
}

void MainWindow::addPaintWidget(PaintWidget *widget,bool isNew)
{
    addChildWindow(widget,isNew);
    ui->mdiArea->setCursor(_currentTool->getToolCursor());
}

PaintWidget *MainWindow::createPaintWidget(const QString &imagePath) const
{
    return new PaintWidget(imagePath, _currentTool);
}

void MainWindow::on_actionOpen_triggered()
{
    const QString fileName = chooseFileForOpening("Open");

    if (_windowCache.contains(fileName)) {
        ui->mdiArea->setActiveSubWindow(_windowCache.value(fileName));
        return;
    }
    openNewFile(fileName);
}

void MainWindow::openNewFile(const QString &fileName)
{
    if (Document::isDocumentValid(fileName)) {
        Document document = readDocument(fileName);
        rememberLastPath(fileName);
        addPaintWidget(new PaintWidget(document, _currentTool));
        return;
    }
    openNewImage(fileName);
}


Document MainWindow::readDocument(const QString &fileName)
{
    QFile file(fileName);

    file.open(QIODevice::ReadOnly);
    QDataStream in(&file);

    Document document(fileName);
    in >> document;
    file.close();

    return document;
}

void MainWindow::rememberLastPath(const QString &fileName)
{
    QFileInfo info(fileName);
    _lastFileLoc = info.absolutePath();
}

const QString MainWindow::chooseFileForOpening(const QString& prompt)
{
    return QFileDialog::getOpenFileName(this, tr(qPrintable(prompt)),
                _lastFileLoc, tr(qPrintable(_filterListString)));
}

const QString MainWindow::chooseFileForSaving(const QString &prompt)
{
   return QFileDialog::getSaveFileName(this, tr(qPrintable(prompt)),
                                           _lastFileLoc, tr(qPrintable(_filterListString)));
}

void MainWindow::on_actionExit_triggered()
{
    QApplication::exit();
}

void MainWindow::on_actionNew_triggered()
{
    NewDialog *newDialog = new NewDialog();
    connect(newDialog, SIGNAL(canvasAvailable(const QSharedDataPointer<Canvas>)),
            this, SLOT(createNewDocument(const QSharedDataPointer<Canvas>)));

    newDialog->show();
}

void MainWindow::createNewDocument(const QSharedDataPointer<Canvas> canvas)
{
    addPaintWidget(new PaintWidget(canvas, _currentTool), true);
}

void MainWindow::on_actionImport_triggered()
{
    const QString fileName = chooseFileForOpening("Import File");
    QMdiSubWindow *currentWindow = ui->mdiArea->activeSubWindow();
    PaintWidget* paintWidget = qobject_cast<PaintWidget*> (currentWindow->widget());

    paintWidget->importPathToLayer(fileName);
    ui->layerView->updateItems(paintWidget->getItems());
}

void MainWindow::on_actionSave_triggered()
{
}

void MainWindow::on_actionSaveAs_triggered()
{
    QMdiSubWindow *currentWindow = ui->mdiArea->activeSubWindow();
    PaintWidget* paintWidget = qobject_cast<PaintWidget*> (currentWindow->widget());

    QString fileName = chooseFileForSaving("Save As");
    // Check with the filter here
    fileName = getFileName(fileName);

    QList<Layer*> layers = paintWidget->getItems();
    QSharedDataPointer<Canvas> canvas = paintWidget->getCanvas();
    Document document(layers, canvas);

    storeDocument(fileName, document);
    updateStateChange(SAVED, fileName, currentWindow);
}

QString MainWindow::getFileName(QString &fileName)
{
    if (!fileName.endsWith(".fxt") && !fileName.endsWith(".fxd")) {
        fileName = fileName + ".fxd";
    }

    return fileName;
}

void MainWindow::storeDocument(const QString &fileName, Document document)
{
    QFile file(fileName);

    file.open(QIODevice::WriteOnly);
    QDataStream out(&file);

    out.setVersion(QDataStream::Qt_5_1);
    out << document;
    file.close();
}

void MainWindow::updateStateChange(State state, const QString &fileName, QMdiSubWindow* subWindow)
{
    switch (state) {
    case SAVED:
        _lastFileLoc = fileName;
        if (!_windowCache.contains(fileName)) {
            _windowCache.insert(fileName, subWindow);
        }
        updateTitleOnSave(fileName, subWindow);
        break;
    default:
        break;
    }
}

void MainWindow::updateTitleOnSave(const QString& title, QMdiSubWindow *window)
{
    QFileInfo info(title);
    window->setWindowTitle(info.fileName());
    window->setWindowModified(false);
}
