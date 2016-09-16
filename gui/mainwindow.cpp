#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QActionGroup* modeGroup = new QActionGroup(this);

    QList<QAction*> modeActions = ui->modeMenu->actions();
    for (auto i = modeActions.begin(); i<modeActions.end(); i++)
        modeGroup->addAction(*i);

    on_singleLevelAction_changed();
    ui->splittingControlGroup->setCurrentIndex(0); // action index magic. Dunno

    ui->stepsChart->legend->setVisible(true);
    ui->edgesChart->legend->setVisible(true);

    ui->edgesChart->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignLeft|Qt::AlignBottom);
    ui->stepsChart->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignLeft|Qt::AlignBottom);

    srand (time(NULL));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::drawLine (QCustomPlot* chart, Line line, QString name, QPen pen, bool drawLabels)
{
    chart->addGraph();
    int i = chart->graphCount()-1;

    chart->graph(i)->setPen(pen);
    chart->graph(i)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 4));

    chart->graph(i)->setData(line.x, line.y);

    chart->xAxis->setRange(-1, line.getMaxX()*1.1);
    chart->yAxis->setRange(0, line.getMaxY()*1.1);

    chart->graph(i)->setName(name);

    if (drawLabels)
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
    if (experimentNumber < 1)
    {
        experimentNumber = 1;
        ui->experimentNumberText->setText("1");
    }

    clearHGraphs();

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
    ui->startButton->setEnabled(true);
}

void MainWindow::clearHGraphs()
{
    hGraph.clear();
    hGraph.resize(experimentNumber);

    for (size_t i=0; i<hGraphHierarchy.size(); i++)
        for (size_t j=0; j<hGraphHierarchy[i].size(); j++)
            for (size_t k=0; k<hGraphHierarchy[i][j].size(); k++)
                delete hGraphHierarchy[i][j][k];

    hGraphHierarchy.clear();
}

void MainWindow::on_randomButton_clicked()
{
    try
    {
        reset();

        int subHGNumber = ui->subHGNumberText->text().toInt();

        ui->progressBar->setMinimum(experimentNumber * 2);
        ui->progressBar->setMaximum(subHGNumber * experimentNumber);
        ui->progressBar->setValue(experimentNumber * 2);

        for (int j=2; j<=subHGNumber; j++)
        {
            ui->statusLabel->setText("Осталось уровней: "+ QString::number(subHGNumber-j));

            double countOfAllFragments = 0;
            double countOfAllExternalEdges = 0;
            for (int i=0; i<experimentNumber; i++)
            {
                hGraph[i]->resetSplitHG();
                hGraph[i]->randomSplitHG(j, 0);
                countOfAllFragments += hGraph[i]->getCountOfFragments();
                countOfAllExternalEdges += hGraph[i]->getCountOfExternalEdges();

                ui->progressBar->setValue(j*experimentNumber+1);
            }
            edges.x.push_back(j);
            edges.y.push_back(100*countOfAllExternalEdges/countOfAllFragments);
        }

        ui->progressBar->setValue(experimentNumber * 2);

        drawLine(ui->edgesChart, edges, " ", QColor(255,0,0));

        std::stringstream str;
        str << "Случайное разбиение\n";

        str  << std::left << std::setw(23) << "Подграфы" << std::setw(20) << "Связи" << "\n";

        for (int i=0; i<edges.x.size(); i++)
            str << std::setw(15) << edges.x[i] << std::setw(15) << edges.y[i] << "\n";

        data = QString::fromStdString(str.str());

    }
    catch (HGraphException &ex)
    {
        QMessageBox::critical(0, "Critical", ex.getError().c_str());
    }

    ui->statusLabel->setText("Готово");
}

void MainWindow::on_seriesButton_clicked()
{
    try
    {
        reset();

        tracingComplexity = ui->tracingComplexityText->text().toDouble();
        deploymentComplexity = ui->deploymentComplexityText->text().toDouble();
        int subHGNumber = ui->subHGNumberText->text().toInt();

        ui->progressBar->setMinimum(experimentNumber * 2);
        ui->progressBar->setMaximum(subHGNumber * experimentNumber);
        ui->progressBar->setValue(experimentNumber * 2);

        double countOfAllFragments = 0;

        for (int i=0; i<experimentNumber; i++)
            countOfAllFragments += hGraph[i]->getCountOfFragments();
        countOfAllFragments /= experimentNumber;
        steps.x.push_back(1);
        steps.y.push_back(pow((double)hGraph[0]->getCountOfVertices(), deploymentComplexity) + pow(countOfAllFragments, tracingComplexity));

        for (int j=2; j<=subHGNumber; j++)
        {
            double countOfAllExternalEdges = 0;

            ui->statusLabel->setText("Осталось уровней: "+ QString::number(subHGNumber-j));

            for (int i=0; i<experimentNumber; i++)
            {
                hGraph[i]->resetSplitHG();
                hGraph[i]->gravitySplitHG(j, 0);
                countOfAllExternalEdges += hGraph[i]->getCountOfExternalEdges();

                ui->progressBar->setValue(j*experimentNumber+1);
            }
            countOfAllExternalEdges /= experimentNumber;

            edges.x.push_back(j);
            edges.y.push_back(100* (countOfAllExternalEdges/countOfAllFragments));

            steps.x.push_back(j);
            steps.y.push_back(hGraph[0]->getCountOfVertices() +
                    pow((double)hGraph[0]->getCountOfVertices()/j, deploymentComplexity) +
                    pow(countOfAllExternalEdges,tracingComplexity) +
                    pow((countOfAllFragments - countOfAllExternalEdges)/j, tracingComplexity));
        }
        ui->progressBar->setValue(experimentNumber * 2);


        drawLine(ui->stepsChart, steps, " ", QColor(255,0,0));
        drawLine(ui->edgesChart, edges, " ", QColor(255,0,0));

        std::stringstream str;
        str << "Последовательное разбиение\n";

        str  << std::left << std::setw(23) << "Подграфы" << std::setw(19) << "Шаги" << std::setw(20) << "Связи" << "\n";
        str << std::setw(15) << steps.x[0] << std::setw(15) << steps.y[0]  << "N/A\n";

        for (int i=0; i<edges.x.size(); i++)
            str << std::setw(15) << steps.x[i+1] << std::setw(15) << steps.y[i+1] << std::setw(15) << edges.y[i] << "\n";

        data = QString::fromStdString(str.str());
    }
    catch (HGraphException &ex)
    {
        QMessageBox::critical(0, "Critical", ex.getError().c_str());
    }
    ui->statusLabel->setText("Готово");
}

void MainWindow::reset()
{
    ui->edgesChart->clearGraphs();
    ui->stepsChart->clearGraphs();

    ui->edgesChart->clearItems();
    ui->stepsChart->clearItems();

    ui->edgesChart->replot();
    ui->stepsChart->replot();

    data.clear();
    steps.clear();
    edges.clear();
}

void MainWindow::on_startButton_clicked()
{
    reset();

    tracingComplexity = ui->tracingComplexityText->text().toDouble();
    deploymentComplexity = ui->deploymentComplexityText->text().toDouble();
    levelNumber = ui->levelNumberText->text().toInt();

    parseSplittingNumbers();

    try
    {
        calculateData(QColor(rand()%255, rand()%255, rand()%255));
    }
    catch (HGraphException &ex)
    {
        QMessageBox::critical(0, "Critical", ex.getError().c_str());
    }

    ui->statusLabel->setText("Готово");
}

void MainWindow::parseSplittingNumbers()
{
    splittingNumbers.clear();

    ui->splittingNumberText->text().remove(' ');

    splittingNumbers.resize(levelNumber);

    QStringList numbers = ui->splittingNumberText->text().split(';', QString::KeepEmptyParts);

    if (numbers.size() < levelNumber)
    {
        for (int i = numbers.size(); i<levelNumber ; i++)
            numbers.push_back("2");
    }

    int i = 0;
    for (QStringList::iterator iter = numbers.begin(); iter != numbers.end(); iter++)
    {
        if ((*iter) == "") (*iter) = "2";
        splittingNumbers[i] = (*iter).toInt();
        i++;
    }
}

void MainWindow::calculateData(QColor graphColor)
{
    ui->statusLabel->setText("Создание ГГ...");

    edges.clear();
    steps.clear();

    initHierarchyHG();
    gatheringData();

    double countAllFragments = 0.0;
    for (int i=0; i<experimentNumber; i++)
        countAllFragments += hGraphHierarchy[0][0][i]->getCountOfFragments();
    countAllFragments /= experimentNumber;
    double nextIncreaseValue = 0.0;

    for (int i=0; i<levelNumber; i++)
    {
        int numberOfComputersOnLevel =  getNumberOfComputersOnLevel(i);
        for (int j=0; j<numberOfComputersOnLevel; j++)
            nextIncreaseValue += increaseOfCountExternalEdges[i][j];

        edges.x.push_back(i+1);
        edges.y.push_back(100*nextIncreaseValue/countAllFragments);
    }

    showData(graphColor);
}

int MainWindow::getNumberOfComputersOnLevel(int level)
{
    int res = 1;
    for (int i=0; i< level; i++)
        res*=splittingNumbers[i];

    return res;
}

void MainWindow::initHierarchyHG()
{
    minNumberSubHG = 0;

    increaseOfCountExternalEdges.clear();
    increaseOfCountExternalEdges.resize(levelNumber);

    for (size_t i=0; i<hGraphHierarchy.size(); i++)
        for (size_t j=0; j<hGraphHierarchy[i].size(); j++)
            for (size_t k=0; k<hGraphHierarchy[i][j].size(); k++)
                if (i || j)
                    delete hGraphHierarchy[i][j][k];

    hGraphHierarchy.clear();
    hGraphHierarchy.resize(levelNumber);

    for (int i=0; i<levelNumber; i++)
    {
        int numberOfComputersOnLevel = getNumberOfComputersOnLevel(i);

        hGraphHierarchy[i].resize(numberOfComputersOnLevel);

        for (int j=0; j<numberOfComputersOnLevel; j++)
            hGraphHierarchy[i][j].resize(experimentNumber);

        increaseOfCountExternalEdges[i].resize(numberOfComputersOnLevel);
    }

    ui->progressBar->setMinimum(0);
    ui->progressBar->setMaximum(experimentNumber);
    ui->progressBar->setValue(0);

    for (int i=0; i<experimentNumber; i++)
    {
        hGraph[i]->resetSplitHG();
        hGraphHierarchy[0][0][i] = hGraph[i];

        ui->progressBar->setValue(i);
    }
}

void MainWindow::gatheringData()
{
    ui->progressBar->setMinimum(0);
    int max = 0;
    for (int i=0; i<levelNumber; i++)
        max += getNumberOfComputersOnLevel(i) * experimentNumber;

    ui->progressBar->setMaximum(max);

    ui->progressBar->setValue(0);

    int countOldExternalEdges = 0;
    for (int i=0; i<levelNumber; i++)
    {
        int numberOfComputersOnLevel = getNumberOfComputersOnLevel(i);

        ui->statusLabel->setText("Осталось уровней: "+ QString::number(levelNumber-i));

        for (int j=0; j<numberOfComputersOnLevel; j++)
        {
            increaseOfCountExternalEdges[i][j] = 0;
            for (int k=0; k<experimentNumber; k++)
            {
                hGraphHierarchy[i][j][k]->gravitySplitHG(splittingNumbers[i], minNumberSubHG);

                increaseOfCountExternalEdges[i][j] += hGraphHierarchy[0][0][k]->getCountOfExternalEdges();

                if (i < levelNumber-1) // Если не последняя итерация - создаю подграфы на основе разбиений
                {
                    for (int l=0; l<splittingNumbers[i]; l++)
                        hGraphHierarchy[i+1][j*splittingNumbers[i] + l][k] = hGraphHierarchy[i][j][k]->createSubHG(minNumberSubHG + l);
                }
            }

            increaseOfCountExternalEdges[i][j] /= experimentNumber;

            // Вычитаю уже имевшиеся связи
            // для получения прироста
            increaseOfCountExternalEdges[i][j] -= countOldExternalEdges;
            countOldExternalEdges += increaseOfCountExternalEdges[i][j];

            minNumberSubHG += splittingNumbers[i];

            ui->progressBar->setValue(ui->progressBar->value()+experimentNumber);
        }
    }
}

void MainWindow::showData (QColor graphColor)
{
    double currentCostOfTracing = 0;
    double currentCountOfInternalEdges = 0;

    for (int i=0; i<experimentNumber; i++)
        currentCountOfInternalEdges += hGraphHierarchy[0][0][i]->getCountOfFragments();
    currentCountOfInternalEdges /= experimentNumber;

    steps.x.push_back(0);
    steps.y.push_back(pow((double)currentCountOfInternalEdges, tracingComplexity)+
                     pow(ui->vertexNumberText->text().toDouble(), deploymentComplexity));

    int levelNumber = ui->levelNumberText->text().toInt();

    for (int i=0; i<levelNumber; i++)
    {
        int maxCountExternalEdges = 0;
        int numberOfComputersOnLevel = getNumberOfComputersOnLevel(i);

        for (int j=0; j< numberOfComputersOnLevel; j++)
        {
            if (maxCountExternalEdges < increaseOfCountExternalEdges[i][j])
                maxCountExternalEdges = increaseOfCountExternalEdges[i][j];

            currentCountOfInternalEdges -= increaseOfCountExternalEdges[i][j];
        }

        currentCostOfTracing += pow((double)maxCountExternalEdges, tracingComplexity);

        double nextValue = hGraphHierarchy[i][0][0]->getCountOfVertices() +
                pow((double)hGraphHierarchy[i][0][0]->getCountOfVertices()/splittingNumbers[i], deploymentComplexity) +
                currentCostOfTracing +
                pow (currentCountOfInternalEdges / getNumberOfComputersOnLevel(i+1),
                     tracingComplexity);

        steps.x.push_back(i+1);
        steps.y.push_back(nextValue);
    }

    drawLine(ui->edgesChart, edges, QString::number(1), graphColor, false);
    drawLine(ui->stepsChart, steps, QString::number(1), graphColor, false);

    std::stringstream str;
    str << "Число разбиений: ";
    for (size_t i = 0; i<splittingNumbers.size(); i++)
        str << splittingNumbers[i] << ((i == splittingNumbers.size()-1) ? "\n" : " ");

    str << "Сложность размещения: " << deploymentComplexity << "\n";
    str << "Сложность трассировки: " << tracingComplexity << "\n";

    str  << std::left << std::setw(23) << "Подграфы" << std::setw(19) << "Шаги" << std::setw(20) << "Связи" << "\n";
    str << std::setw(15) << steps.x[0] << std::setw(15) << steps.y[0]  << "N/A\n";

    for (int i=0; i<edges.x.size(); i++)
        str << std::setw(15) << steps.x[i+1] << std::setw(15) << steps.y[i+1] << std::setw(15) << edges.y[i] << "\n";

    data += QString::fromStdString(str.str());
}

void MainWindow::on_showButton_clicked()
{
    DataWindow dw;
    dw.setData(data);
    dw.exec();
}
