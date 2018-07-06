#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMdiSubWindow>
#include <QFileDialog>
#include <QStandardPaths>
#include <QMimeData>
#include <QActionGroup>

#include "dialogs/newdialog.h"
#include "widgets/paintwidget.h"

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

    void on_actionNew_triggered();

    void on_actionOpen_triggered();

    void on_actionImport_triggered();

    void on_actionExit_triggered();

    void setPanTool(bool selected);

private:
    Ui::MainWindow *ui;
    QString _lastFileLoc;
    QActionGroup *_toolsGroup;
    QList<QAction*> _toolsList;

    const QString chooseFile();
    void addChildWindow(PaintWidget *widget, bool isNew);
    void addPaintWidget(PaintWidget *widget, bool isNew = false);
    PaintWidget *createPaintWidget(const QString &imagePath) const;
    void rememberLastPath(const QString& fileName);

    void dragEnterEvent(QDragEnterEvent *e);
    void dropEvent(QDropEvent *e);
};

#endif // MAINWINDOW_H
