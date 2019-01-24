#include <QApplication>
#include "mainwindow.h"

int
main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    Fixture::MainWindow w(nullptr);
    w.show();
    return app.exec();
}
