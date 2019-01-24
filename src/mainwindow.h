#pragma once

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

namespace Fixture {
    class MainWindow : public QMainWindow
    {
        Q_OBJECT

    public:
        explicit MainWindow(QWidget *parent = 0);
        ~MainWindow();

    private slots:

    private:
        Ui::MainWindow *ui;
    };
}
