#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <ctime>

#include "chart/qcustomplot.h"
#include "chart/line.h"
#include "hgraph/hgraph.h"

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

    vector<HGraph*> hGraph;

    vector<vector<vector<HGraph*>>> hGraphHierarchy;
    vector<vector<int>> increaseOfCountExternalEdges;

    int experimentNumber; // for getting an average value
    int minNumberSubHG;
    int splittingNumber = 2; // on each level of the hierarchical model

    Mode mode;

private slots:
    void on_singleLevelAction_changed();

    void on_hierarchicalAction_changed();

    void on_createHGButton_clicked();

    void on_randomButton_clicked();

    void on_seriesButton_clicked();

    void on_startButton_clicked();

private:

    Ui::MainWindow *ui;

    void drawLine (QCustomPlot* chart, Line line, QString name, QPen pen);
    void drawText (QCustomPlot* chart, QString text, QPen pen, double x, double y);
    void resetCharts();

    void initHierarchyHG ();
    void gatheringData ();
    void showData (int complexity);
};

#endif // MAINWINDOW_H
