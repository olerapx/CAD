#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    initMenu();
    initCharts();

    initWorker();

    srand(time(NULL));
}

void MainWindow::closeEvent(QCloseEvent* event)
{
    sendStop();
    ui->statusLabel->setText(tr("Остановка..."));

    workerThread.wait();

    QMainWindow::closeEvent(event);
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

    ui->stepsChart->chart()->legend()->setAlignment(Qt::AlignLeft);
    ui->edgesChart->chart()->legend()->setAlignment(Qt::AlignLeft);

    ui->stepsChart->setRenderHint(QPainter::Antialiasing);
    ui->edgesChart->setRenderHint(QPainter::Antialiasing);
}

void MainWindow::initWorker()
{
    worker.moveToThread(&workerThread);

    connect(&worker, &HGraphWorker::sendGenerated, this, &MainWindow::onSendGenerated);
    connect(&worker, &HGraphWorker::sendRandomCalculated, this, &MainWindow::onSendRandomCalculated);
    connect(&worker, &HGraphWorker::sendSeriesCalculated, this, &MainWindow::onSendSeriesCalculated);
    connect(&worker, &HGraphWorker::sendHierarchicalCalculated, this, &MainWindow::onSendHierarchicalCalculated);
    connect(&worker, &HGraphWorker::sendPrintHierarchicalData, this, &MainWindow::onSendPrintHierarchicalData);

    connect(&worker, &HGraphWorker::sendStopped, this, &MainWindow::onSendStopped);

    connect(&worker, &HGraphWorker::sendCreateNewSeries, this, &MainWindow::onSendCreateNewSeries);
    connect(&worker, &HGraphWorker::sendSetMaxProgress, this, &MainWindow::onSendSetMaxProgress);

    connect(&worker, &HGraphWorker::sendStatus, this, &MainWindow::onSendStatus);
    connect(&worker, &HGraphWorker::sendProgress, this, &MainWindow::onSendProgress);
    connect(&worker, &HGraphWorker::sendAddProgress, this, &MainWindow::onSendAddProgress);

    connect(&worker, &HGraphWorker::sendEdgesAppend, this, &MainWindow::onSendEdgesAppend);
    connect(&worker, &HGraphWorker::sendStepsAppend, this, &MainWindow::onSendStepsAppend);
    connect(&worker, &HGraphWorker::sendSingleStepsAppend, this, &MainWindow::onSendSingleStepsAppend);

    connect(&worker, &HGraphWorker::sendError, this, &MainWindow::onSendError);

    connect(this, &MainWindow::sendStop, &worker, &HGraphWorker::onStopped, Qt::DirectConnection);
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

void MainWindow::onSendGenerated()
{
    ui->randomButton->setEnabled(true);
    ui->seriesButton->setEnabled(true);
    ui->startButton->setEnabled(true);

    ui->statusLabel->setText("Готово");
    ui->progressBar->setValue(ui->progressBar->maximum());

    workerThread.disconnect();
    workerThread.quit();
}

void MainWindow::onSendRandomCalculated()
{
    ui->statusLabel->setText("Готово");
    printRandomData();
    ui->progressBar->setValue(ui->progressBar->maximum());

    workerThread.disconnect();
    workerThread.quit();
}

void MainWindow::onSendSeriesCalculated()
{
    ui->statusLabel->setText("Готово");
    printSeriesData();
    ui->progressBar->setValue(ui->progressBar->maximum());

    workerThread.disconnect();
    workerThread.quit();
}

void MainWindow::onSendHierarchicalCalculated()
{
    ui->statusLabel->setText("Готово");
    ui->progressBar->setValue(ui->progressBar->maximum());

    workerThread.disconnect();
    workerThread.quit();
}

void MainWindow::onSendPrintHierarchicalData(uint i)
{
    ui->statusLabel->setText("Переход на следующую модель");
    printHierarchicalData(i);
}

void MainWindow::onSendStopped()
{
    ui->statusLabel->setText("Остановлено");

    workerThread.disconnect();
    workerThread.quit();
}

void MainWindow::onSendCreateNewSeries(uint index)
{
    steps = new QLineSeries();
    edges = new QLineSeries();
    singleSteps.clear();

    steps->setPointsVisible(true);
    edges->setPointsVisible(true);

    steps->setName(QString::number(index+1));
    edges->setName(QString::number(index+1));
}

void MainWindow::onSendSetMaxProgress(int value)
{
    ui->progressBar->setMaximum(value);
}

void MainWindow::onSendStatus(QString status)
{
    ui->statusLabel->setText(status);
}

void MainWindow::onSendProgress(int progress)
{
    ui->progressBar->setValue(progress);
}

void MainWindow::onSendAddProgress(int progress)
{
    ui->progressBar->setValue(ui->progressBar->value() + progress);
}

void MainWindow::onSendEdgesAppend(QPointF point)
{
    edges->append(point);
}

void MainWindow::onSendStepsAppend(QPointF point)
{
    steps->append(point);
}

void MainWindow::onSendSingleStepsAppend(double value)
{
    singleSteps.push_back(value);
}

void MainWindow::onSendError(QString error)
{
    QMessageBox::critical(0, "Critical", error);
    workerThread.disconnect();
    workerThread.quit();
}

void MainWindow::on_createGraphButton_clicked()
{
    if(!worker.isStopped()) return;

    ui->statusLabel->setText("Создание ГГ...");

    experimentNumber = ui->experimentNumberText->text().toUInt();
    verticesNumber = ui->vertexNumberText->text().toUInt();
    minContactNumber = ui->minContactNumberText->text().toUInt();
    maxContactNumber = ui->maxContactNumberText->text().toUInt();
    minEdgeNumber = ui->minEdgeNumberText->text().toUInt();
    maxEdgeNumber = ui->maxEdgeNumberText->text().toUInt();

    worker.setGenerationParameters(experimentNumber, verticesNumber,
                    minContactNumber, maxContactNumber,
                    minEdgeNumber, maxEdgeNumber);

    connect(&workerThread, &QThread::started, &worker, &HGraphWorker::onGenerate);
    workerThread.start();
}

void MainWindow::on_randomButton_clicked()
{
    if(!worker.isStopped()) return;

    resetCharts();

    steps = new QLineSeries();
    edges = new QLineSeries();
    singleSteps.clear();

    setSeriesStyle(steps);
    setSeriesStyle(edges);

    steps->setName("1");
    edges->setName("1");

    subGraphsNumber = ui->subGraphsNumberText->text().toUInt();

    ui->progressBar->setMinimum(experimentNumber * 2);
    ui->progressBar->setMaximum(subGraphsNumber * experimentNumber);
    ui->progressBar->setValue(experimentNumber * 2);

    worker.setRandomParameters(subGraphsNumber);

    connect(&workerThread, &QThread::started, &worker, &HGraphWorker::onCalculateRandom);
    workerThread.start();

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

void MainWindow::setAxisStyle(QChartView *view)
{
    view->chart()->createDefaultAxes();

    view->chart()->axisX()->setGridLinePen(QPen(QBrush(Qt::Dense6Pattern), 0.5));
    view->chart()->axisY()->setGridLinePen(QPen(QBrush(Qt::Dense6Pattern), 0.5));

    view->chart()->axisX()->setMin(0);
    view->chart()->axisY()->setMin(0);

    int max = 0;
    for(QAbstractSeries* s: view->chart()->series())
    {
        QLineSeries* ls = (QLineSeries*) s;

        for(int i=0; i<ls->count(); i++)
            if(ls->at(i).x() > max) max = ceil(ls->at(i).x());
    }

    QValueAxis* a = (QValueAxis*)view->chart()->axisX();
    a->setTickCount(max + 1);
}

void MainWindow::printRandomData()
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
    dataWindow.setData(data);
}

void MainWindow::on_seriesButton_clicked()
{
    if(!worker.isStopped()) return;

    resetCharts();

    steps = new QLineSeries();
    edges = new QLineSeries();
    singleSteps.clear();

    setSeriesStyle(steps);
    setSeriesStyle(edges);

    steps->setName("1");
    edges->setName("1");

    tracingComplexity = ui->tracingComplexityText->text().toDouble();
    deploymentComplexity = ui->deploymentComplexityText->text().toDouble();
    subGraphsNumber = ui->subGraphsNumberText->text().toUInt();

    ui->progressBar->setMinimum(experimentNumber * 2);
    ui->progressBar->setMaximum(subGraphsNumber * experimentNumber);
    ui->progressBar->setValue(experimentNumber * 2);

    worker.setSeriesParameters(tracingComplexity, deploymentComplexity, subGraphsNumber);

    connect(&workerThread, &QThread::started, &worker, &HGraphWorker::onCalculateSeries);
    workerThread.start();
}

void MainWindow::printSeriesData()
{
    ui->stepsChart->chart()->addSeries(steps);
    ui->edgesChart->chart()->addSeries(edges);  

    setAxisStyle(ui->stepsChart);
    setAxisStyle(ui->edgesChart);

    std::stringstream str;
    str << "Последовательное разбиение\n";

    str  << std::left << std::setw(23) << "Подграфы" << std::setw(24) << "Шаги РСАПР" << std::setw(23) << "Шаги САПР" << std::setw(20) << "Связи" << "\n";

    for (int i=0; i<edges->count(); i++)
        str << std::setw(15) << steps->at(i).x() << std::setw(15) << steps->at(i).y() << std::setw(15) << singleSteps[i] << std::setw(15) << edges->at(i).y() << "\n";

    data = QString::fromStdString(str.str());
    dataWindow.setData(data);
}

void MainWindow::resetCharts()
{
    ui->stepsChart->chart()->removeAllSeries();
    ui->edgesChart->chart()->removeAllSeries();

    data.clear();
}

void MainWindow::on_startButton_clicked()
{
    if(!worker.isStopped()) return;

    resetCharts();

    tracingComplexity = ui->tracingComplexityText->text().toDouble();
    deploymentComplexity = ui->deploymentComplexityText->text().toDouble();
    levelNumber = ui->levelNumberText->text().toInt();

    parseSplittingNumbers();

    ui->progressBar->setMinimum(0);
    ui->progressBar->setMaximum(experimentNumber);

    worker.setHierarchicalParameters(splittingNumbers, tracingComplexity, deploymentComplexity, levelNumber);

    connect(&workerThread, &QThread::started, &worker, &HGraphWorker::onCalculateHierarchical);
    workerThread.start();
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
    int levelNumber = ui->levelNumberText->text().toInt();
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

void MainWindow::printHierarchicalData(size_t index)
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

    str << std::left << std::setw(21) << "Уровни" << std::setw(24) << "Шаги РСАПР" << std::setw(23) << "Шаги САПР" << std::setw(20) << "Связи" << "\n";

    for (int i=0; i<edges->count(); i++)
        str << std::setw(15) << steps->at(i).x() << std::setw(15) << steps->at(i).y() << std::setw(15) << singleSteps[i] << std::setw(15) << edges->at(i).y() << "\n";

    data += QString::fromStdString(str.str());
    dataWindow.setData(data);
}

void MainWindow::on_showButton_clicked()
{
    dataWindow.show();
}

void MainWindow::on_stopButton_clicked()
{
    sendStop();
    ui->statusLabel->setText(tr("Остановка..."));
}
