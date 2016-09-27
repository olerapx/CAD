#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <ctime>
#include <sstream>
#include <iomanip>

#include "chart/qcustomplot.h"
#include "chart/line.h"
#include "hgraph/hgraphworker.h"
#include "chart/linecolorpicker.h"

#include "datawindow.h"

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
    Line steps, edges;
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

    void drawLine (QCustomPlot* chart, Line line, QString name, QPen pen, bool drawLabels = true);
    void drawText (QCustomPlot* chart, QString text, QPen pen, double x, double y);
    void resetCharts();

    void clearGraphs();

    void printRandomData();
    void printSeriesData();

    void parseSplittingNumbers();
    void parseSplittingNumber(QString string, size_t i);

    void calculateData (QColor graphColor, size_t index);

    size_t getNumberOfComputersOnLevel(size_t index, size_t level);
    void initGraphHierarchy (size_t index);
    void copyGraphToHierarchy ();
    void gatheringData (size_t index);
    void showData (QColor graphColor, size_t index);
    void printHierarchicalData(QColor graphColor, size_t index);
};

#endif // MAINWINDOW_H
