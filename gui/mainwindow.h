#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QChartView>
#include <QLineSeries>
#include <QMessageBox>
#include <QChart>
#include <QValueAxis>
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

    /* common */
    Mode mode;

    double deploymentComplexity, tracingComplexity;

    QString data;

    size_t experimentNumber;

    /* single-level */
    vector<HGraph*> hGraph;
    size_t minSubGraphsNumber;

    /* hierarchical */
    vector<vector<vector<HGraph*>>> hGraphHierarchy;
    vector<vector<size_t>> externalEdgesNumberIncreasing;

    vector<vector<size_t>> splittingNumbers;
    int levelNumber;

private slots:
    void on_singleLevelAction_changed();

    void on_hierarchicalAction_changed();

    void on_createGraphButton_clicked();

    void on_randomButton_clicked();

    void on_seriesButton_clicked();

    void on_startButton_clicked();

    void on_showButton_clicked();

private:
    Ui::MainWindow *ui;

    void initMenu();
    void initCharts();

    void resetCharts();

    void clearGraphs();

    void printRandomData(QLineSeries *steps, QLineSeries *edges);
    void printSeriesData(QLineSeries *steps, QLineSeries *edges);

    void setSeriesStyle(QLineSeries* series);
    void setAxisStyle(QChartView* view);

    void parseSplittingNumbers();
    void parseSplittingNumber(QString string, size_t i);

    void calculateData(size_t index);

    size_t getNumberOfComputersOnLevel(size_t index, size_t level);
    void initGraphHierarchy(size_t index);
    void copyGraphToHierarchy();
    void gatheringData(size_t index);
    void showData(size_t index, QLineSeries *steps, QLineSeries *edges);
    void printHierarchicalData(size_t index, QLineSeries *steps, QLineSeries *edges);
};

#endif // MAINWINDOW_H
