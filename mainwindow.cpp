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
    delete [] hGraph;
    delete [] hierarhyHG;
    delete [] increaseOfCountExternalEdges;

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

void MainWindow::on_randomButton_clicked()
{
    resetGraphs();

    QVector<double> edgesX, edgesY;

    int HGNumber = ui->HGNumberText->text().toInt();

    for (int j=2; j<=HGNumber; j++)
    {
        float countOfAllFragments = 0;
        float countOfAllExternalEdges = 0;
        for (int i=0; i<countHG; i++)
        {
            hGraph[i]->resetSplitHG();
            hGraph[i]->randomSplitHG(j,0);
            countOfAllFragments += hGraph[i]->getCountOfFragments();
            countOfAllExternalEdges += hGraph[i]->getCountOfExternalEdges();
        }
        edgesX.push_back(j);
        edgesY.push_back(100*countOfAllExternalEdges/countOfAllFragments);
    }

    ui->edgesChart->graph(0)->setData(edgesX, edgesY);
    ui->edgesChart->replot();
}

void MainWindow::on_seriesButton_clicked()
{
    resetGraphs();

    float exponent;
    if (ui->tracingQuadratic->isChecked())
        exponent = 2.0;
    else
        exponent = 3.0;

    int HGNumber = ui->HGNumberText->text().toInt();

    ui->progressBar->setMinimum(countHG  *2);
    ui->progressBar->setMaximum(HGNumber * countHG);
    ui->progressBar->setValue(countHG * 2);

    QVector<double> stepsX, stepsY, edgesX, edgesY;

    float countOfAllFragments = 0;

    for (int i=0; i<countHG; i++)
        countOfAllFragments += hGraph[i]->getCountOfFragments();
    countOfAllFragments /= countHG;

    stepsX.push_back(1);
    stepsY.push_back(pow((double)hGraph[0]->getCountOfVertices(),2.0) + pow((double)countOfAllFragments,exponent));

    for (int j=2; j<=HGNumber; j++)
    {
        float countOfAllExternalEdges = 0;

        for (int i=0; i<countHG; i++)
        {
            hGraph[i]->resetSplitHG();
            hGraph[i]->gravitySplitHG(j,0);
            countOfAllExternalEdges += hGraph[i]->getCountOfExternalEdges();

            ui->progressBar->setValue(j*countHG+1);
        }

        countOfAllExternalEdges /= countHG;

        edgesX.push_back(j);
        edgesY.push_back(100*countOfAllExternalEdges/countOfAllFragments);

        stepsX.push_back(j);
        stepsY.push_back(hGraph[0]->getCountOfVertices() +
                pow((double)hGraph[0]->getCountOfVertices()/j,2.0) +
                pow((double)countOfAllExternalEdges,exponent) +
                pow((double)(countOfAllFragments-countOfAllExternalEdges)/j,exponent));
    }

    ui->stepsChart->graph(0)->setData(stepsX, stepsY);
    ui->stepsChart->replot();

    ui->edgesChart->graph(0)->setData(edgesX, edgesY);
    ui->edgesChart->replot();
}

void MainWindow::resetGraphs()
{
    ui->edgesChart->clearGraphs();
    ui->stepsChart->clearGraphs();

    ui->edgesChart->addGraph();
    ui->stepsChart->addGraph();

    ui->edgesChart->graph(0)->setPen(QColor(255, 0, 0, 255));
    ui->edgesChart->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 4));

    ui->stepsChart->graph(0)->setPen(QColor(255, 0, 0, 255));
    ui->stepsChart->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 4));
}

void MainWindow::on_startButton_clicked()
{
    countHG = ui->HGNumberText->text().toInt();
    if (countHG<1)
        countHG=10;

    QVector<double> edgesX, edgesY;

    this->setWindowTitle("Создание ГГ...");

    initHierarhyHG();

    gatheringData();

    this->setWindowTitle("Почти все...");

    float countAllFragments = 0;
    for (int i=0; i<countHG; i++)
        countAllFragments += hierarhyHG[0][0][i]->getCountOfFragments();
    countAllFragments /= countHG;
    float nextIncreaseValue = 0;

    int levelNumber = ui->levelNumberText->text().toInt();

    for (int i=0; i<levelNumber; i++)
    {
        for (int j=0; j<(int)pow(2.0,i); j++)
            nextIncreaseValue += increaseOfCountExternalEdges[i][j];

        edgesX.push_back(i+1);
        edgesY.push_back(100*nextIncreaseValue/countAllFragments);
    }

    //  Button4->Enabled = false;
    //  RadioGroup1Click(Sender);

    ui->edgesChart->graph(0)->setData(edgesX, edgesY);
    ui->edgesChart->replot();
}

void MainWindow::showData (int Complexity)
{
    resetGraphs();

    QVector<double> stepsX, stepsY;

    double currentCostOfTracing = 0;
    double currentCountOfInternalEdges = 0;

    for (int i=0; i<countHG; i++)
        currentCountOfInternalEdges += hierarhyHG[0][0][i]->getCountOfFragments();
    currentCountOfInternalEdges /= countHG;

    stepsX.push_back(0);
    stepsY.push_back(pow((double)currentCountOfInternalEdges, Complexity)+
                     pow(ui->vertexNumberText->text().toDouble(),2));

    int levelNumber = ui->levelNumberText->text().toInt();

    for (int i=0; i<levelNumber; i++)
    {
        int maxCountExternalEdges = 0;
        for (int j=0; j<(int)pow(2.0,i); j++)
        {
            if (maxCountExternalEdges < increaseOfCountExternalEdges[i][j])
                maxCountExternalEdges = increaseOfCountExternalEdges[i][j];

            currentCountOfInternalEdges -= increaseOfCountExternalEdges[i][j];
        }

        currentCostOfTracing += pow((double)maxCountExternalEdges, Complexity);

        double nextValue = hierarhyHG[i][0][0]->getCountOfVertices() +
                pow((double)hierarhyHG[i][0][0]->getCountOfVertices()/2,2.0) +
                currentCostOfTracing +
                pow (currentCountOfInternalEdges / pow(2.0,i+1),
                     Complexity);

        stepsX.push_back(i+1);
        stepsY.push_back(nextValue);
    }

    ui->stepsChart->graph(0)->setData(stepsX, stepsY);
    ui->stepsChart->replot();
}

void MainWindow::initHierarhyHG()
{
    int levelNumber = ui->levelNumberText->text().toInt();

    minNumberSubHG = 0;
    increaseOfCountExternalEdges = new int * [levelNumber];
    hierarhyHG = new HGraph*** [levelNumber];

    for (int i=0; i<levelNumber; i++)
    {
        hierarhyHG[i] = new HGraph** [(int)pow(2.0,i)];

        for (int j=0; j<(int)pow(2.0,i); j++)
            hierarhyHG[i][j] = new HGraph* [countHG];

        increaseOfCountExternalEdges[i] = new int [(int)pow(2.0,i)];
    }

    ui->progressBar->setMinimum(0);
    ui->progressBar->setMaximum(countHG);
    ui->progressBar->setValue(0);

    for (int i=0; i<countHG; i++)
    {
        hierarhyHG[0][0][i] = new HGraph;
        hierarhyHG[0][0][i]->HGraphGenerator(ui->vertexNumberText->text().toInt(),
                                             ui->minContactNumberText->text().toInt(),
                                             ui->maxContactNumberText->text().toInt(),
                                             ui->minEdgeNumberText->text().toInt(),
                                             ui->maxEdgeNumberText->text().toInt());
        ui->progressBar->setValue(i);
    }
}

void MainWindow::gatheringData()
{
    int levelNumber = ui->levelNumberText->text().toInt();

    ui->progressBar->setMinimum(countHG);
    ui->progressBar->setMaximum((int)pow(2.0, levelNumber*countHG));

    ui->progressBar->setValue(countHG);

    int countOldExternalEdges = 0;

    for (int i=0; i<levelNumber; i++)
    {
        this->setWindowTitle("Осталось уровней: "+levelNumber-i);

        for (int j=0; j<(int)pow(2.0, i); j++)       // Подграфы в уровнях
        {
            increaseOfCountExternalEdges[i][j] = 0;
            for (int k=0; k<countHG; k++)             // Множество экспериментов
            {
                hierarhyHG[i][j][k]->gravitySplitHG(2,minNumberSubHG);
                // Разбили

                increaseOfCountExternalEdges[i][j] += hierarhyHG[0][0][k]->getCountOfExternalEdges();
                // Плюсанул новые внешние связи
                if (i <levelNumber-1) // Если не последняя итерация -
                    // создаю подграфы на основе разбиений
                {
                    hierarhyHG[i+1][j*2][k] = hierarhyHG[i][j][k]->createSubHG(minNumberSubHG);
                    hierarhyHG[i+1][j*2+1][k] = hierarhyHG[i][j][k]->createSubHG(minNumberSubHG+1);
                }

            }
            // Вычисляю среднее значение по серии
            increaseOfCountExternalEdges[i][j] /= countHG;

            // Вычитаю уже имевшиеся связи
            // для получения прироста
            increaseOfCountExternalEdges[i][j] -= countOldExternalEdges;
            countOldExternalEdges += increaseOfCountExternalEdges[i][j];

            // Увеличиваю счетчик собственных
            // номеров подграфов
            minNumberSubHG += 2;

            ui->progressBar->setValue(ui->progressBar->value()+countHG);
        }
    }
}
