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
#include "model/document.h"
#include "tools/tooloptions/transform_menu.h"

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
    void createNewDocument(const QSharedDataPointer<Canvas> canvas);

private slots:
    void updateWindow(QMdiSubWindow *window);
    void changeTool(QAction* action);
    void onSelectionChange();
    void on_actionNew_triggered();
    void on_actionOpen_triggered();
    void on_actionImport_triggered();
    void on_actionSave_triggered();
    void on_actionSaveAs_triggered();
    void on_actionExit_triggered();    

private:
    Ui::MainWindow *ui;
    QString _lastFileLoc;
    QActionGroup *_toolsGroup;
    QList<QAction*> _toolsList;
    Tool *_currentTool;
    QAction *_menu;
    QMap<QString, QMdiSubWindow*> _windowCache;
    QMap<QWidget*, QAction*> _toolMenuCache;
    enum State {
        SAVED, MODIFIED
    };
    enum SupportedTypes {
        FXD, IMAGE, PNG, JPG, GIF, TIFF, ICO, BMP
    };
    QMap<SupportedTypes, QString> _supportedTypeMap;
    QString _filterListString;
    void initSupportedFileMap();
    void initFilterListString();
    void openNewImage(const QString &);
    void setupMdiArea();
    void setupTabBar(QTabBar* bar);
    void updateActions(bool val);
    void initTools();
    void initSignalsAndSlots();
    void addTool(Tool *tool);
    void setDefaultTool(Tool *tool);
    void setCurrentTool(Tool *tool);
    void activateMenu(QWidget *menuWidget);
    void updateMenuFromCache(QWidget *widget);
    const QString chooseFileForOpening(const QString &prompt);
    const QString chooseFileForSaving(const QString &prompt);
    void addChildWindow(PaintWidget *widget, bool isNew);
    void setupSubWindowTitle(QMdiSubWindow*, const QString&, bool isNew);
    void addPaintWidget(PaintWidget *widget, bool isNew = false);
    void updateLayerSelection();
    void updateToolForCurrentWindow(QMdiSubWindow*);
    void updateDragMode(PaintWidget*);
    void openNewFile(const QString&);
    PaintWidget *createPaintWidget(const QString &imagePath) const;
    void rememberLastPath(const QString& fileName);
    QString getFileName(QString &name);
    void storeDocument(const QString &fileName, Document document);
    Document readDocument(const QString&);
    void updateStateChange(State state, const QString &fileName, QMdiSubWindow *subWindow);
    void updateTitleOnSave(const QString&, QMdiSubWindow *window);

    void dragEnterEvent(QDragEnterEvent *e);
    void dropEvent(QDropEvent *e);
    bool eventFilter(QObject *watched, QEvent *event);
};

#endif // MAINWINDOW_H
