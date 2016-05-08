#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);    

    QActionGroup* modeGroup = new QActionGroup(this);

    QList<QAction*> modeActions = ui->modeMenu->actions();
    for (auto i = modeActions.begin();i<modeActions.end();i++)
        modeGroup->addAction(*i);

    on_singleLevelAction_changed();
    ui->splittingControlGroup->setCurrentIndex(0);

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

void MainWindow::on_createHGButton_clicked()
{
    countHG = ui->HGNumberText->text().toInt();
    if (countHG<1)
      countHG=10;

    hGraph = new HGraph* [countHG];
    for (int i=0; i<countHG; i++)
      hGraph[i] = new HGraph;

    for (int i=0; i<countHG; i++)
      hGraph[i]->HGraphGenerator(ui->vertexNumberText->text().toInt(),
                          ui->minContactNumberText->text().toInt(),
                          ui->maxContactNumberText->text().toInt(),
                          ui->minEdgeNumberText->text().toInt(),
                          ui->maxEdgeNumberText->text().toInt());
  //don't know if is it needed
    // ui->createHGButton->setEnabled(false);

    ui->randomButton->setEnabled(true);
    ui->seriesButton->setEnabled(true);
}
