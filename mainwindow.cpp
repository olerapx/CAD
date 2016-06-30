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
    ui->splittingControlGroup->setCurrentIndex(0); // action index magic. Dunno

    srand (time(NULL));
}

MainWindow::~MainWindow()
{
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
    experimentNumber = ui->experimentNumberText->text().toInt();
    if (experimentNumber<1)
        experimentNumber=10;

    for (size_t i=0;i<hGraph.size();i++)
            delete hGraph[i];

     hGraph.clear();
     hGraph.resize(experimentNumber);

    for (int i=0; i<experimentNumber; i++)
    {
        hGraph[i] = new HGraph;
        hGraph[i]->HGraphGenerator(ui->vertexNumberText->text().toInt(),
                               ui->minContactNumberText->text().toInt(),
                               ui->maxContactNumberText->text().toInt(),
                               ui->minEdgeNumberText->text().toInt(),
                               ui->maxEdgeNumberText->text().toInt());
    }

    ui->randomButton->setEnabled(true);
    ui->seriesButton->setEnabled(true);
}

void MainWindow::on_randomButton_clicked()
{
    resetCharts();

    Line edges;

    int subHGNumber = ui->subHGNumberText->text().toInt();

    for (int j=2; j<=subHGNumber; j++)
    {
        double countOfAllFragments = 0;
        double countOfAllExternalEdges = 0;
        for (int i=0; i<experimentNumber; i++)
        {
            hGraph[i]->resetSplitHG();
            hGraph[i]->randomSplitHG(j, 0);
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
    resetCharts();

    double exponent;
    if (ui->tracingQuadratic->isChecked())
        exponent = 2.0;
    else
        exponent = 3.0;

    int subHGNumber = ui->subHGNumberText->text().toInt();

    ui->progressBar->setMinimum(experimentNumber * 2);
    ui->progressBar->setMaximum(subHGNumber * experimentNumber);
    ui->progressBar->setValue(experimentNumber * 2);

    Line steps, edges;

    float countOfAllFragments = 0;

    for (int i=0; i<experimentNumber; i++)
        countOfAllFragments += hGraph[i]->getCountOfFragments();
    countOfAllFragments /= experimentNumber;
    steps.x.push_back(1);
    steps.y.push_back(pow((double)hGraph[0]->getCountOfVertices(),2.0) + pow((double)countOfAllFragments,exponent));

    for (int j=2; j<=subHGNumber; j++)
    {
        float countOfAllExternalEdges = 0;

        for (int i=0; i<experimentNumber; i++)
        {
            hGraph[i]->resetSplitHG();
            hGraph[i]->gravitySplitHG(j, 0);
            countOfAllExternalEdges += hGraph[i]->getCountOfExternalEdges();

            ui->progressBar->setValue(j*experimentNumber+1);
        }
        countOfAllExternalEdges /= experimentNumber;

        edges.x.push_back(j);
        edges.y.push_back(100*countOfAllExternalEdges/countOfAllFragments);

        steps.x.push_back(j);
        steps.y.push_back(hGraph[0]->getCountOfVertices() +
                pow((double)hGraph[0]->getCountOfVertices()/j,2.0) +
                pow((double)countOfAllExternalEdges,exponent) +
                pow((double)(countOfAllFragments-countOfAllExternalEdges)/j, exponent));
    }
    ui->progressBar->setValue(experimentNumber * 2);

    drawLine(ui->stepsChart, steps, " ", QColor(255,0,0));
    drawLine(ui->edgesChart, edges, " ", QColor(255,0,0));
}

void MainWindow::resetCharts()
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
    resetCharts();

    Line edges;

    experimentNumber = ui->experimentNumberText->text().toInt();
    if (experimentNumber<1)
        experimentNumber=1;

    splittingNumber = ui->splittingNumberText->text().toInt();
    if (splittingNumber < 2)
        splittingNumber = 2;

    ui->statusLabel->setText("Создание ГГ...");

    initHierarchyHG();
    gatheringData();

    ui->statusLabel->setText("Почти все...");

    double countAllFragments = 0.0;
    for (int i=0; i<experimentNumber; i++)
        countAllFragments += hGraphHierarchy[0][0][i]->getCountOfFragments();
    countAllFragments /= experimentNumber;
    double nextIncreaseValue = 0.0;

    int levelNumber = ui->levelNumberText->text().toInt();

    for (int i=0; i<levelNumber; i++)
    {
        for (int j=0; j<(int)pow(splittingNumber, i); j++)
            nextIncreaseValue += increaseOfCountExternalEdges[i][j];

        edges.x.push_back(i+1);
        edges.y.push_back(100*nextIncreaseValue/countAllFragments);
    }

    drawLine(ui->edgesChart, edges, " ", QColor(255,0,0));

    if (ui->tracingQuadratic->isChecked())
        showData(2);
    else showData(3);

    ui->statusLabel->setText("Готово.");
}

void MainWindow::initHierarchyHG()
{
    int levelNumber = ui->levelNumberText->text().toInt();

    minNumberSubHG = 0;

    increaseOfCountExternalEdges.clear();
    increaseOfCountExternalEdges.resize(levelNumber);

    for (size_t i=0; i<hGraphHierarchy.size(); i++)
        for (size_t j=0; j<hGraphHierarchy[i].size(); j++)
                for (size_t k=0; k<hGraphHierarchy[i][j].size(); k++)
                            if (hGraphHierarchy[i][j][k]!=nullptr)
                                delete hGraphHierarchy[i][j][k];

    hGraphHierarchy.clear();

    hGraphHierarchy.resize(levelNumber);

    for (int i=0; i<levelNumber; i++)
    {
        int splittingNumberOnLevel = (int)pow(splittingNumber, i);

        hGraphHierarchy[i].resize(splittingNumberOnLevel);

        for (int j=0; j<splittingNumberOnLevel; j++)
            hGraphHierarchy[i][j].resize(experimentNumber);

        increaseOfCountExternalEdges[i].resize(splittingNumberOnLevel);
    }

    ui->progressBar->setMinimum(0);
    ui->progressBar->setMaximum(experimentNumber);
    ui->progressBar->setValue(0);

    for (int i=0; i<experimentNumber; i++)
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

    ui->progressBar->setMinimum(0);
    int max = 0;
    for (int i=0; i<levelNumber; i++)
        max += (int)pow(splittingNumber, i) * experimentNumber;

    ui->progressBar->setMaximum(max);

    ui->progressBar->setValue(0);

    int countOldExternalEdges = 0;
    for (int i=0; i<levelNumber; i++)
    {
        int splittingNumberOnLevel = (int) pow(splittingNumber,i);

        ui->statusLabel->setText("Осталось уровней: "+levelNumber-i);

        for (int j=0; j<splittingNumberOnLevel; j++)       // Подграфы в уровнях
        {
            increaseOfCountExternalEdges[i][j] = 0;
            for (int k=0; k<experimentNumber; k++)             // Множество экспериментов
            {
                hGraphHierarchy[i][j][k]->gravitySplitHG(splittingNumber, minNumberSubHG);
                // Разбили

                increaseOfCountExternalEdges[i][j] += hGraphHierarchy[0][0][k]->getCountOfExternalEdges();
                // Плюсанул новые внешние связи
                if (i < levelNumber-1) // Если не последняя итерация -
                    // создаю подграфы на основе разбиений
                {
                    for (int l=0; l<splittingNumber; l++)
                        hGraphHierarchy[i+1][j*splittingNumber + l][k] = hGraphHierarchy[i][j][k]->createSubHG(minNumberSubHG + l);
                }
            }
            // Вычисляю среднее значение по серии
            increaseOfCountExternalEdges[i][j] /= experimentNumber;

            // Вычитаю уже имевшиеся связи
            // для получения прироста
            increaseOfCountExternalEdges[i][j] -= countOldExternalEdges;
            countOldExternalEdges += increaseOfCountExternalEdges[i][j];

            // Увеличиваю счетчик собственных
            // номеров подграфов
            minNumberSubHG += splittingNumber;

            ui->progressBar->setValue(ui->progressBar->value()+experimentNumber);
        }
    }
}

void MainWindow::showData (int complexity)
{
    Line steps;

    double currentCostOfTracing = 0;
    double currentCountOfInternalEdges = 0;

    for (int i=0; i<experimentNumber; i++)
        currentCountOfInternalEdges += hGraphHierarchy[0][0][i]->getCountOfFragments();
    currentCountOfInternalEdges /= experimentNumber;

    steps.x.push_back(0);
    steps.y.push_back(pow((double)currentCountOfInternalEdges, complexity)+
                     pow(ui->vertexNumberText->text().toDouble(), splittingNumber)); ///TODO: check here

    int levelNumber = ui->levelNumberText->text().toInt();

    for (int i=0; i<levelNumber; i++)
    {
        int maxCountExternalEdges = 0;
        for (int j=0; j<(int)pow(splittingNumber, i); j++)
        {
            if (maxCountExternalEdges < increaseOfCountExternalEdges[i][j])
                maxCountExternalEdges = increaseOfCountExternalEdges[i][j];

            currentCountOfInternalEdges -= increaseOfCountExternalEdges[i][j];
        }

        currentCostOfTracing += pow((double)maxCountExternalEdges, complexity);

        double nextValue = hGraphHierarchy[i][0][0]->getCountOfVertices() +
                pow((double)hGraphHierarchy[i][0][0]->getCountOfVertices()/splittingNumber, splittingNumber) +
                currentCostOfTracing +
                pow (currentCountOfInternalEdges / pow(splittingNumber,i+1),
                     complexity); ///TODO: check here

        steps.x.push_back(i+1);
        steps.y.push_back(nextValue);
    }

    drawLine(ui->stepsChart, steps, " ", QColor(255,0,0));
}
