#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <ctime>
#include <sstream>
#include <iomanip>

#include "chart/qcustomplot.h"
#include "chart/line.h"
#include "hgraph/hgraph.h"
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

    int experimentNumber;

    /* single-level */
    vector<HGraph*> hGraph;
    int minNumberSubHG;

    /* hierarchical */
    vector<vector<vector<HGraph*>>> hGraphHierarchy;
    vector<vector<int>> increaseOfCountExternalEdges;

    vector<vector<int>> splittingNumbers;
    int levelNumber;

private slots:
    void on_singleLevelAction_changed();

    void on_hierarchicalAction_changed();

    void on_createHGButton_clicked();

    void on_randomButton_clicked();

    void on_seriesButton_clicked();

    void on_startButton_clicked();

    void on_showButton_clicked();

private:
    Ui::MainWindow *ui;

    void drawLine (QCustomPlot* chart, Line line, QString name, QPen pen, bool drawLabels = true);
    void drawText (QCustomPlot* chart, QString text, QPen pen, double x, double y);
    void reset();

    void clearHGraphs();

    void parseSplittingNumbers();
    void parseSplittingNumber(QString string, int i);

    void calculateData (QColor graphColor, size_t index);

    int getNumberOfComputersOnLevel(size_t index, size_t level);
    void initHierarchyHG (size_t index);
    void gatheringData (size_t index);
    void showData (QColor graphColor, size_t index);
};

#endif // MAINWINDOW_H
