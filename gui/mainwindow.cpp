#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    initMenu();
    initCharts();

    srand(time(NULL));
}

void MainWindow::initMenu()
{
    QActionGroup* modeGroup = new QActionGroup(this);

    QList<QAction*> modeActions = ui->modeMenu->actions();
    for (auto i = modeActions.begin(); i<modeActions.end(); i++)
        modeGroup->addAction(*i);

    on_singleLevelAction_changed();
    ui->splittingControlGroup->setCurrentIndex(0); // This must be done because of unspecified reasons.
}

void MainWindow::initCharts()
{
    ui->stepsChart->setChart(new QChart());
    ui->edgesChart->setChart(new QChart());

    ui->stepsChart->chart()->legend()->setAlignment(Qt::AlignBottom);
    ui->edgesChart->chart()->legend()->setAlignment(Qt::AlignBottom);

    ui->stepsChart->setRenderHint(QPainter::Antialiasing);
    ui->edgesChart->setRenderHint(QPainter::Antialiasing);
}

MainWindow::~MainWindow()
{
    delete ui;
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

void MainWindow::on_createGraphButton_clicked()
{
    ui->statusLabel->setText("Создание ГГ...");

    experimentNumber = ui->experimentNumberText->text().toUInt();
    if (experimentNumber < 1)
    {
        experimentNumber = 1;
        ui->experimentNumberText->setText("1");
    }

    clearGraphs();

    for (size_t i=0; i<experimentNumber; i++)
    {
        hGraph[i] = new HGraph;
        HGraphWorker::generateGraph(hGraph[i], ui->vertexNumberText->text().toUInt(),
                               ui->minContactNumberText->text().toUInt(),
                               ui->maxContactNumberText->text().toUInt(),
                               ui->minEdgeNumberText->text().toUInt(),
                               ui->maxEdgeNumberText->text().toUInt());
    }

    ui->randomButton->setEnabled(true);
    ui->seriesButton->setEnabled(true);
    ui->startButton->setEnabled(true);

    ui->statusLabel->setText("Готово");
}

void MainWindow::clearGraphs()
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
        resetCharts();

        QLineSeries* steps = new QLineSeries();
        QLineSeries* edges = new QLineSeries();

        setSeriesStyle(steps);
        setSeriesStyle(edges);

        steps->setName("1");
        edges->setName("1");

        size_t subGraphsNumber = ui->subGraphsNumberText->text().toUInt();

        ui->progressBar->setMinimum(experimentNumber * 2);
        ui->progressBar->setMaximum(subGraphsNumber * experimentNumber);
        ui->progressBar->setValue(experimentNumber * 2);

        for (size_t j=2; j<=subGraphsNumber; j++)
        {
            ui->statusLabel->setText("Осталось частей: "+ QString::number(subGraphsNumber-j));

            size_t totalFragmentsNumber = 0;
            size_t totalExternalEdgesNumber = 0;

            for (size_t i=0; i<experimentNumber; i++)
            {
                HGraphWorker::resetSplitting(hGraph[i]);
                HGraphWorker::randomSplit(hGraph[i], j, 0);
                totalFragmentsNumber += hGraph[i]->getFragmentsNumber();
                totalExternalEdgesNumber += hGraph[i]->getExternalEdgesNumber();

                ui->progressBar->setValue(j*experimentNumber+1);
            }
            edges->append(j, 100.0 * (double)totalExternalEdgesNumber/(double)totalFragmentsNumber);
        }

        ui->progressBar->setValue(experimentNumber * 2);

        printRandomData(steps, edges);
    }
    catch (HGraphException &ex)
    {
        QMessageBox::critical(0, "Critical", ex.getError().c_str());
    }

    ui->statusLabel->setText("Готово");
}

void MainWindow::setSeriesStyle(QLineSeries *series)
{
    series->setPointLabelsVisible(true);
    series->setPointLabelsClipping(false);
    series->setPointLabelsFormat("@yPoint");

    QFont font;
    font.setWeight(QFont::DemiBold);

    series->setPointLabelsFont(font);

    series->setPointsVisible(true);
}

void MainWindow::printRandomData(QLineSeries* steps, QLineSeries* edges)
{  
    ui->stepsChart->chart()->addSeries(steps);
    ui->edgesChart->chart()->addSeries(edges);

    setAxisStyle(ui->stepsChart);
    setAxisStyle(ui->edgesChart);

    std::stringstream str;
    str << "Случайное разбиение\n";

    str << std::left << std::setw(23) << "Подграфы" << std::setw(20) << "Связи" << "\n";

    for (int i=0; i<edges->count(); i++)
        str << std::setw(15) << edges->at(i).x() << std::setw(15) << edges->at(i).y() << "\n";

    data = QString::fromStdString(str.str());
}

void MainWindow::setAxisStyle(QChartView *view)
{
    view->chart()->createDefaultAxes();

    view->chart()->axisX()->setGridLinePen(QPen(QBrush(Qt::Dense6Pattern), 0.5));
    view->chart()->axisY()->setGridLinePen(QPen(QBrush(Qt::Dense6Pattern), 0.5));
}

void MainWindow::on_seriesButton_clicked()
{
    try
    {
        resetCharts();

        QLineSeries* steps = new QLineSeries();
        QLineSeries* edges = new QLineSeries();

        setSeriesStyle(steps);
        setSeriesStyle(edges);

        steps->setName("1");
        edges->setName("1");

        tracingComplexity = ui->tracingComplexityText->text().toDouble();
        deploymentComplexity = ui->deploymentComplexityText->text().toDouble();
        size_t subHGNumber = ui->subGraphsNumberText->text().toUInt();

        ui->progressBar->setMinimum(experimentNumber * 2);
        ui->progressBar->setMaximum(subHGNumber * experimentNumber);
        ui->progressBar->setValue(experimentNumber * 2);

        double countOfAllFragments = 0;

        for (size_t i=0; i<experimentNumber; i++)
            countOfAllFragments += hGraph[i]->getFragmentsNumber();
        countOfAllFragments /= experimentNumber;

        steps->append(1, pow((double)hGraph[0]->getVerticesNumber(), deploymentComplexity) + pow(countOfAllFragments, tracingComplexity));

        for (size_t j=2; j<=subHGNumber; j++)
        {
            double countOfAllExternalEdges = 0;

            ui->statusLabel->setText("Осталось частей: " + QString::number(subHGNumber-j));

            for (size_t i=0; i<experimentNumber; i++)
            {
                HGraphWorker::resetSplitting(hGraph[i]);
                HGraphWorker::gravitySplit(hGraph[i], j, 0);
                countOfAllExternalEdges += hGraph[i]->getExternalEdgesNumber();

                ui->progressBar->setValue(j*experimentNumber+1);
            }
            countOfAllExternalEdges /= experimentNumber;

            edges->append(j, 100* (countOfAllExternalEdges/countOfAllFragments));
            steps->append(j, hGraph[0]->getVerticesNumber() +
                    pow((double)hGraph[0]->getVerticesNumber()/j, deploymentComplexity) +
                    pow(countOfAllExternalEdges,tracingComplexity) +
                    pow((countOfAllFragments - countOfAllExternalEdges)/j, tracingComplexity));
        }
        ui->progressBar->setValue(experimentNumber * 2);

        printSeriesData(steps, edges);
    }
    catch (HGraphException &ex)
    {
        QMessageBox::critical(0, "Critical", ex.getError().c_str());
    }
    ui->statusLabel->setText("Готово");
}

void MainWindow::printSeriesData(QLineSeries *steps, QLineSeries *edges)
{
    ui->stepsChart->chart()->addSeries(steps);
    ui->edgesChart->chart()->addSeries(edges);

    setAxisStyle(ui->stepsChart);
    setAxisStyle(ui->edgesChart);

    std::stringstream str;
    str << "Последовательное разбиение\n";

    str  << std::left << std::setw(23) << "Подграфы" << std::setw(19) << "Шаги" << std::setw(20) << "Связи" << "\n";
    str << std::setw(15) << steps->at(0).x() << std::setw(15) << steps->at(0).y()  << "N/A\n";

    for (int i=0; i<edges->count(); i++)
        str << std::setw(15) << steps->at(i+1).x() << std::setw(15) << steps->at(i+1).y() << std::setw(15) << edges->at(i).y() << "\n";

    data = QString::fromStdString(str.str());
}

void MainWindow::resetCharts()
{
    ui->stepsChart->chart()->removeAllSeries();
    ui->edgesChart->chart()->removeAllSeries();

    data.clear();
}

void MainWindow::on_startButton_clicked()
{
    resetCharts();

    tracingComplexity = ui->tracingComplexityText->text().toDouble();
    deploymentComplexity = ui->deploymentComplexityText->text().toDouble();
    levelNumber = ui->levelNumberText->text().toInt();

    parseSplittingNumbers();

    try
    {
        for (size_t i=0; i< splittingNumbers.size(); i++)
        {
            calculateData(i);
        }
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
    ui->splittingNumberText->setText(ui->splittingNumberText->toPlainText().remove(' '));

    QStringList lines = ui->splittingNumberText->toPlainText().split('\n', QString::KeepEmptyParts);

    splittingNumbers.resize(lines.length());

    for (size_t i=0; i < static_cast<size_t>(lines.length()); i++)
        parseSplittingNumber(lines[i], i);
}

void MainWindow::parseSplittingNumber(QString string, size_t i)
{
    splittingNumbers[i].resize(levelNumber);

    QStringList numbers = string.split(';', QString::KeepEmptyParts);

    if (numbers.size() < levelNumber)
    {
        for (int i = numbers.size(); i<levelNumber ; i++)
            numbers.push_back("2");
    }

    int j = 0;
    for (QStringList::iterator iter = numbers.begin(); iter != numbers.end(); iter++)
    {
        if ((*iter) == "") (*iter) = "2";
        splittingNumbers[i][j] = (*iter).toUInt();
        j++;
    }
}

void MainWindow::calculateData(size_t index)
{
    QLineSeries* steps = new QLineSeries();
    QLineSeries* edges = new QLineSeries();

    steps->setPointsVisible(true);
    edges->setPointsVisible(true);

    steps->setName(QString::number(index+1));
    edges->setName(QString::number(index+1));

    initGraphHierarchy(index);
    gatheringData(index);

    double countAllFragments = 0.0;
    for (size_t i=0; i<experimentNumber; i++)
        countAllFragments += hGraphHierarchy[0][0][i]->getFragmentsNumber();
    countAllFragments /= experimentNumber;

    double nextIncreaseValue = 0.0;

    for (int i=0; i<levelNumber; i++)
    {
        size_t numberOfComputersOnLevel =  getNumberOfComputersOnLevel(index, i);
        for (size_t j=0; j<numberOfComputersOnLevel; j++)
            nextIncreaseValue += externalEdgesNumberIncreasing[i][j];

        edges->append(i+1, 100 * nextIncreaseValue/countAllFragments);
    }

    showData(index, steps, edges);
}

size_t MainWindow::getNumberOfComputersOnLevel(size_t index, size_t level)
{
    size_t res = 1;
    for (size_t i=0; i< level; i++)
        res*=splittingNumbers[index][i];

    return res;
}

void MainWindow::initGraphHierarchy(size_t index)
{
    minSubGraphsNumber = 0;

    externalEdgesNumberIncreasing.clear();
    externalEdgesNumberIncreasing.resize(levelNumber);

    for (size_t i=0; i<hGraphHierarchy.size(); i++)
        for (size_t j=0; j<hGraphHierarchy[i].size(); j++)
            for (size_t k=0; k<hGraphHierarchy[i][j].size(); k++)
                if (i || j)
                    delete hGraphHierarchy[i][j][k];

    hGraphHierarchy.clear();
    hGraphHierarchy.resize(levelNumber);

    for (int i=0; i<levelNumber; i++)
    {
        int numberOfComputersOnLevel = getNumberOfComputersOnLevel(index, i);

        hGraphHierarchy[i].resize(numberOfComputersOnLevel);

        for (int j=0; j<numberOfComputersOnLevel; j++)
            hGraphHierarchy[i][j].resize(experimentNumber);

        externalEdgesNumberIncreasing[i].resize(numberOfComputersOnLevel);
    }

    ui->progressBar->setMinimum(0);
    ui->progressBar->setMaximum(experimentNumber);
    ui->progressBar->setValue(0);

    copyGraphToHierarchy();
}

void MainWindow::copyGraphToHierarchy()
{
    for (size_t i=0; i<experimentNumber; i++)
    {
        HGraphWorker::resetSplitting(hGraph[i]);
        hGraphHierarchy[0][0][i] = hGraph[i];

        ui->progressBar->setValue(i);
    }
}

void MainWindow::gatheringData(size_t index)
{
    ui->progressBar->setMinimum(0);
    int max = 0;
    for (int i=0; i<levelNumber; i++)
        max += getNumberOfComputersOnLevel(index, i) * experimentNumber;

    ui->progressBar->setMaximum(max);

    ui->progressBar->setValue(0);

    int countOldExternalEdges = 0;
    for (int i=0; i<levelNumber; i++)
    {
        int numberOfComputersOnLevel = getNumberOfComputersOnLevel(index, i);

        ui->statusLabel->setText("Осталось уровней: "+ QString::number(levelNumber-i));

        for (int j=0; j<numberOfComputersOnLevel; j++)
        {
            externalEdgesNumberIncreasing[i][j] = 0;
            for (size_t k=0; k<experimentNumber; k++)
            {
                HGraphWorker::gravitySplit(hGraphHierarchy[i][j][k], splittingNumbers[index][i], minSubGraphsNumber);

                externalEdgesNumberIncreasing[i][j] += hGraphHierarchy[0][0][k]->getExternalEdgesNumber();

                if (i < levelNumber-1) // Если не последняя итерация - создаю подграфы на основе разбиений
                {
                    for (size_t l=0; l<splittingNumbers[index][i]; l++)
                        hGraphHierarchy[i+1][j*splittingNumbers[index][i] + l][k] = hGraphHierarchy[i][j][k]->createSubGraph(minSubGraphsNumber + l);
                }
            }

            externalEdgesNumberIncreasing[i][j] /= experimentNumber;

            // Вычитаю уже имевшиеся связи
            // для получения прироста
            externalEdgesNumberIncreasing[i][j] -= countOldExternalEdges;
            countOldExternalEdges += externalEdgesNumberIncreasing[i][j];

            minSubGraphsNumber += splittingNumbers[index][i];

            ui->progressBar->setValue(ui->progressBar->value()+experimentNumber);
        }
    }
}

void MainWindow::showData(size_t index, QLineSeries *steps, QLineSeries *edges)
{
    double currentCostOfTracing = 0;
    double currentCountOfInternalEdges = 0;

    for (size_t i=0; i<experimentNumber; i++)
        currentCountOfInternalEdges += hGraphHierarchy[0][0][i]->getFragmentsNumber();
    currentCountOfInternalEdges /= experimentNumber;

    steps->append(0, pow((double)currentCountOfInternalEdges, tracingComplexity)+
                 pow(ui->vertexNumberText->text().toDouble(), deploymentComplexity));

    int levelNumber = ui->levelNumberText->text().toInt();

    for (int i=0; i<levelNumber; i++)
    {
        size_t maxCountExternalEdges = 0;
        int numberOfComputersOnLevel = getNumberOfComputersOnLevel(index, i);

        for (int j=0; j< numberOfComputersOnLevel; j++)
        {
            if (maxCountExternalEdges < externalEdgesNumberIncreasing[i][j])
                maxCountExternalEdges = externalEdgesNumberIncreasing[i][j];

            currentCountOfInternalEdges -= externalEdgesNumberIncreasing[i][j];
        }

        currentCostOfTracing += pow((double)maxCountExternalEdges, tracingComplexity);

        double nextValue = hGraphHierarchy[i][0][0]->getVerticesNumber() +
                pow((double)hGraphHierarchy[i][0][0]->getVerticesNumber()/splittingNumbers[index][i], deploymentComplexity) +
                currentCostOfTracing +
                pow (currentCountOfInternalEdges / getNumberOfComputersOnLevel(index, i+1),
                     tracingComplexity);

        steps->append(i+1, nextValue);
    }

    printHierarchicalData(index, steps, edges);
}

void MainWindow::printHierarchicalData(size_t index, QLineSeries *steps, QLineSeries *edges)
{
    ui->stepsChart->chart()->addSeries(steps);
    ui->edgesChart->chart()->addSeries(edges);

    setAxisStyle(ui->stepsChart);
    setAxisStyle(ui->edgesChart);

    std::stringstream str;
    str << "Число разбиений: ";
    for (size_t i = 0; i<splittingNumbers[index].size(); i++)
        str << splittingNumbers[index][i] << ((i == splittingNumbers[index].size()-1) ? "\n" : " ");

    str << "Сложность размещения: " << deploymentComplexity << "\n";
    str << "Сложность трассировки: " << tracingComplexity << "\n";

    str  << std::left << std::setw(23) << "Уровни" << std::setw(19) << "Шаги" << std::setw(20) << "Связи" << "\n";
    str << std::setw(15) << steps->at(0).x() << std::setw(15) << steps->at(0).y()  << "N/A\n";

    for (int i=0; i<edges->count(); i++)
        str << std::setw(15) << steps->at(i+1).x() << std::setw(15) << steps->at(i+1).y() << std::setw(15) << edges->at(i).y() << "\n";

    data += QString::fromStdString(str.str());
}

void MainWindow::on_showButton_clicked()
{
    DataWindow dw;
    dw.setData(data);
    dw.exec();
}
