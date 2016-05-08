#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    srand (time(NULL));
    hGraph = nullptr;
    hierarhyHG = nullptr;
    increaseOfCountExternalEdges = nullptr;
}

MainWindow::~MainWindow()
{
    delete ui;
}
