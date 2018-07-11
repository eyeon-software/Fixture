#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMdiSubWindow>
#include <QFileDialog>
#include <QStandardPaths>
#include <QMimeData>
#include <QActionGroup>
#include <QIcon>

#include "dialogs/newdialog.h"
#include "widgets/paintwidget.h"
#include "tools/pan.h"
#include "tools/transform.h"
#include "tools/tool.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void createNewDocument(const Canvas *canvas);
    void updateLayers();

private slots:
    void updateWindow(QMdiSubWindow *window);
    void changeTool(QAction* action);
    void onSelectionChange();
    void on_actionNew_triggered();
    void on_actionOpen_triggered();
    void on_actionImport_triggered();
    void on_actionExit_triggered();    

private:
    Ui::MainWindow *ui;
    QString _lastFileLoc;
    QActionGroup *_toolsGroup;
    QList<QAction*> _toolsList;
    Transform *_transform;
    Pan *_pan;
    Tool *_currentTool;

    void initTools();
    void initSignalsAndSlots();
    void setDefaultTool(Tool *tool);
    const QString chooseFile();
    void addChildWindow(PaintWidget *widget, bool isNew);
    void addPaintWidget(PaintWidget *widget, bool isNew = false);
    PaintWidget *createPaintWidget(const QString &imagePath) const;
    void rememberLastPath(const QString& fileName);

    void dragEnterEvent(QDragEnterEvent *e);
    void dropEvent(QDropEvent *e);
};

#endif // MAINWINDOW_H
