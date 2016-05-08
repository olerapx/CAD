#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);    

    QActionGroup* modeGroup = new QActionGroup(this);

    QList<QAction*> modeActions = ui->modeMenu->actions();
    for (auto i = modeActions.begin();i<modeActions.end();i++)
        modeGroup->addAction(*i);

    ui->splittingControlGroup->setCurrentIndex(0);
    mode = SINGLE_LEVEL;

    srand (time(NULL));
    hGraph = nullptr;
    hierarhyHG = nullptr;
    increaseOfCountExternalEdges = nullptr;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_singleLevelAction_changed()
{
    ui->splittingControlGroup->setCurrentIndex(1);
    mode = SINGLE_LEVEL;

    ui->randomButton->setEnabled(false);
    ui->seriesButton->setEnabled(false);
}

void MainWindow::on_hierarchicalAction_changed()
{
    ui->splittingControlGroup->setCurrentIndex(0);
    mode = HIERARCHICAL;
}
