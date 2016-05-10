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
    hGraphHierarchy = nullptr;
    increaseOfCountExternalEdges = nullptr;
}

MainWindow::~MainWindow()
{
    delete [] hGraph;
    delete [] hGraphHierarchy;
    delete [] increaseOfCountExternalEdges;

    delete ui;
}

void MainWindow::drawLine (QCustomPlot* chart, Line line, QString name, QPen pen)
{
    chart->addGraph();
    int i = chart->graphCount()-1;

    chart->graph(i)->setPen(pen);
    chart->graph(i)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 4));

    chart->graph(i)->setData(line.x, line.y);

    chart->xAxis->setRange(-1, line.getMaxX()*1.1);
    chart->yAxis->setRange(0, line.getMaxY()*1.1);

    chart->graph(i)->setName(name);

    for (int j=0; j<line.y.size(); j++)
        drawText(chart, QString::number(line.y[j]), pen,
                 line.x[j],
                 line.y[j]+line.getMaxY()/line.y.size()/5);

    chart->replot();
}

void MainWindow::drawText(QCustomPlot *chart, QString text, QPen pen, double x, double y)
{
    QCPItemText *textLabel = new QCPItemText(chart);
    textLabel->setText(text);
    textLabel->setPen(pen);
    chart->addItem(textLabel);

    textLabel->position->setCoords(x, y);
}

void MainWindow::on_singleLevelAction_changed()
{
    ui->splittingControlGroup->setCurrentIndex(1);
    mode = SINGLE_LEVEL;
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

     delete[] hGraph;

    ui->randomButton->setEnabled(true);
    ui->seriesButton->setEnabled(true);

    hGraph = new HGraph* [countHG];

    for (int i=0; i<countHG; i++)
        hGraph[i] = new HGraph;

    for (int i=0; i<countHG; i++)
        hGraph[i]->HGraphGenerator(ui->vertexNumberText->text().toInt(),
                                   ui->minContactNumberText->text().toInt(),
                                   ui->maxContactNumberText->text().toInt(),
                                   ui->minEdgeNumberText->text().toInt(),
                                   ui->maxEdgeNumberText->text().toInt());

    ui->randomButton->setEnabled(true);
    ui->seriesButton->setEnabled(true);
}

void MainWindow::on_randomButton_clicked()
{
    resetGraphs();

    Line edges;

    int subHGNumber = ui->subHGNumberText->text().toInt();

    for (int j=2; j<=subHGNumber; j++)
    {
        double countOfAllFragments = 0;
        double countOfAllExternalEdges = 0;
        for (int i=0; i<countHG; i++)
        {
            hGraph[i]->resetSplitHG();
            hGraph[i]->randomSplitHG(j,0);
            countOfAllFragments += hGraph[i]->getCountOfFragments();
            countOfAllExternalEdges += hGraph[i]->getCountOfExternalEdges();
        }
        edges.x.push_back(j);
        edges.y.push_back(100*countOfAllExternalEdges/countOfAllFragments);
    }

    drawLine(ui->edgesChart, edges, " ", QColor(255,0,0));
}

void MainWindow::on_seriesButton_clicked()
{
    resetGraphs();

    float exponent;
    if (ui->tracingQuadratic->isChecked())
        exponent = 2.0;
    else
        exponent = 3.0;

    int subHGNumber = ui->subHGNumberText->text().toInt();

    ui->progressBar->setMinimum(countHG  *2);
    ui->progressBar->setMaximum(subHGNumber * countHG);
    ui->progressBar->setValue(countHG * 2);

    Line steps, edges;

    float countOfAllFragments = 0;

    for (int i=0; i<countHG; i++)
        countOfAllFragments += hGraph[i]->getCountOfFragments();
    countOfAllFragments /= countHG;

    steps.x.push_back(1);
    steps.y.push_back(pow((double)hGraph[0]->getCountOfVertices(),2.0) + pow((double)countOfAllFragments,exponent));

    for (int j=2; j<=subHGNumber; j++)
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

        edges.x.push_back(j);
        edges.y.push_back(100*countOfAllExternalEdges/countOfAllFragments);

        steps.x.push_back(j);
        steps.y.push_back(hGraph[0]->getCountOfVertices() +
                pow((double)hGraph[0]->getCountOfVertices()/j,2.0) +
                pow((double)countOfAllExternalEdges,exponent) +
                pow((double)(countOfAllFragments-countOfAllExternalEdges)/j, exponent));
    }

    ui->progressBar->setValue(countHG * 2);

    drawLine(ui->stepsChart, steps, " ", QColor(255,0,0));
    drawLine(ui->edgesChart, edges, " ", QColor(255,0,0));
}

void MainWindow::resetGraphs()
{
    ui->edgesChart->clearGraphs();
    ui->stepsChart->clearGraphs();

    ui->edgesChart->clearItems();
    ui->stepsChart->clearItems();

    ui->edgesChart->replot();
    ui->stepsChart->replot();
}

void MainWindow::on_startButton_clicked()
{
    resetGraphs();

    Line edges;

    countHG = ui->HGNumberText->text().toInt();
    if (countHG<1)
        countHG=10;

    ui->statusLabel->setText("Создание ГГ...");

    initHierarhyHG();
    gatheringData();

    ui->statusLabel->setText("Почти все...");

    float countAllFragments = 0;
    for (int i=0; i<countHG; i++)
        countAllFragments += hGraphHierarchy[0][0][i]->getCountOfFragments();
    countAllFragments /= countHG;
    float nextIncreaseValue = 0;

    int levelNumber = ui->levelNumberText->text().toInt();

    for (int i=0; i<levelNumber; i++)
    {
        for (int j=0; j<(int)pow(2.0,i); j++)
            nextIncreaseValue += increaseOfCountExternalEdges[i][j];

        edges.x.push_back(i+1);
        edges.y.push_back(100*nextIncreaseValue/countAllFragments);
    }

    //  Button4->Enabled = false;
    //  RadioGroup1Click(Sender);

    drawLine(ui->edgesChart, edges, " ", QColor(255,0,0));

    if (ui->tracingQuadratic->isChecked())
        showData(2);
    else showData(3);

    ui->statusLabel->setText("Готово.");
}

void MainWindow::showData (int Complexity)
{
    Line steps;

    double currentCostOfTracing = 0;
    double currentCountOfInternalEdges = 0;

    for (int i=0; i<countHG; i++)
        currentCountOfInternalEdges += hGraphHierarchy[0][0][i]->getCountOfFragments();
    currentCountOfInternalEdges /= countHG;

    steps.x.push_back(0);
    steps.y.push_back(pow((double)currentCountOfInternalEdges, Complexity)+
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

        double nextValue = hGraphHierarchy[i][0][0]->getCountOfVertices() +
                pow((double)hGraphHierarchy[i][0][0]->getCountOfVertices()/2,2.0) +
                currentCostOfTracing +
                pow (currentCountOfInternalEdges / pow(2.0,i+1),
                     Complexity);

        steps.x.push_back(i+1);
        steps.y.push_back(nextValue);
    }

    drawLine(ui->stepsChart, steps, " ", QColor(255,0,0));
}

void MainWindow::initHierarhyHG()
{
    int levelNumber = ui->levelNumberText->text().toInt();

    minNumberSubHG = 0;
    increaseOfCountExternalEdges = new int * [levelNumber];
    hGraphHierarchy = new HGraph*** [levelNumber];

    for (int i=0; i<levelNumber; i++)
    {
        hGraphHierarchy[i] = new HGraph** [(int)pow(2.0,i)];

        for (int j=0; j<(int)pow(2.0,i); j++)
            hGraphHierarchy[i][j] = new HGraph* [countHG];

        increaseOfCountExternalEdges[i] = new int [(int)pow(2.0,i)];
    }

    ui->progressBar->setMinimum(0);
    ui->progressBar->setMaximum(countHG);
    ui->progressBar->setValue(0);

    for (int i=0; i<countHG; i++)
    {
        hGraphHierarchy[0][0][i] = new HGraph;
        hGraphHierarchy[0][0][i]->HGraphGenerator(ui->vertexNumberText->text().toInt(),
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
        ui->statusLabel->setText("Осталось уровней: "+levelNumber-i);

        for (int j=0; j<(int)pow(2.0, i); j++)       // Подграфы в уровнях
        {
            increaseOfCountExternalEdges[i][j] = 0;
            for (int k=0; k<countHG; k++)             // Множество экспериментов
            {
                hGraphHierarchy[i][j][k]->gravitySplitHG(2,minNumberSubHG);
                // Разбили

                increaseOfCountExternalEdges[i][j] += hGraphHierarchy[0][0][k]->getCountOfExternalEdges();
                // Плюсанул новые внешние связи
                if (i <levelNumber-1) // Если не последняя итерация -
                    // создаю подграфы на основе разбиений
                {
                    hGraphHierarchy[i+1][j*2][k] = hGraphHierarchy[i][j][k]->createSubHG(minNumberSubHG);
                    hGraphHierarchy[i+1][j*2+1][k] = hGraphHierarchy[i][j][k]->createSubHG(minNumberSubHG+1);
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
