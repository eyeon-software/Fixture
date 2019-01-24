#include "mainwindow.h"
#include "ui_mainwindow.h"

Fixture::MainWindow::MainWindow(QWidget *parent):
    QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

Fixture::MainWindow::~MainWindow()
{
    delete ui;
}
