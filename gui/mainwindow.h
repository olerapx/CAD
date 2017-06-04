#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QChartView>
#include <QLineSeries>
#include <QMessageBox>
#include <QChart>
#include <QValueAxis>
#include <QThread>
#include <ctime>
#include <sstream>
#include <iomanip>

#include "hgraph/hgraphworker.h"
#include "datawindow.h"


using namespace QtCharts;

namespace Ui
{
    class MainWindow;
}

enum Mode
{
    SINGLE_LEVEL,
    HIERARCHICAL
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_singleLevelAction_changed();

    void on_hierarchicalAction_changed();

    void on_createGraphButton_clicked();

    void on_randomButton_clicked();

    void on_seriesButton_clicked();

    void on_startButton_clicked();

    void on_showButton_clicked();

    void onSendGenerated();
    void onSendRandomCalculated();
    void onSendSeriesCalculated();
    void onSendHierarchicalCalculated();
    void onSendPrintHierarchicalData(uint i);

    void onSendStopped();

    void onSendCreateNewSeries(uint index);
    void onSendSetMaxProgress(int value);

    void onSendStatus(QString status);
    void onSendProgress(int progress);
    void onSendAddProgress(int progress);

    void onSendEdgesAppend(QPointF point);
    void onSendStepsAppend(QPointF point);
    void onSendSingleStepsAppend(double value);

    void onSendError(QString error);

    void on_stopButton_clicked();

protected:
     void closeEvent(QCloseEvent *event);

signals:
     void sendStop();

private:
    Ui::MainWindow *ui;

    QString data;

    Mode mode;
    HGraphWorker worker;
    DataWindow dataWindow;

    QThread workerThread;

    size_t experimentNumber;
    size_t verticesNumber;

    size_t minContactNumber, maxContactNumber;
    size_t minEdgeNumber, maxEdgeNumber;

    size_t subGraphsNumber;
    size_t levelNumber;

    size_t tracingComplexity, deploymentComplexity;

    vector<vector<size_t>> splittingNumbers;

    QLineSeries *steps, *edges;
    vector<double> singleSteps;

    void initMenu();
    void initCharts();
    void initWorker();

    void resetCharts();

    void printRandomData();
    void printSeriesData();

    void setSeriesStyle(QLineSeries* series);
    void setAxisStyle(QChartView* view);

    void parseSplittingNumbers();
    void parseSplittingNumber(QString string, size_t i);

    void printHierarchicalData(size_t index);
};

#endif // MAINWINDOW_H
