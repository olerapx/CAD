#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <ctime>
#include <sstream>
#include <iomanip>

#include "chart/qcustomplot.h"
#include "chart/line.h"
#include "hgraph/hgraph.h"

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

    vector<HGraph*> hGraph;

    vector<vector<vector<HGraph*>>> hGraphHierarchy;
    vector<vector<int>> increaseOfCountExternalEdges;

    Line steps, edges;

    int experimentNumber; // for getting an average value
    int minNumberSubHG;

    // on each level of the hierarchical model
    int minSplittingNumber;
    int maxSplittingNumber;
    int levelNumber;

    Mode mode;

    QString data;

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

    void calculateData (int splittingNumber, QColor graphColor);
    void initHierarchyHG (int splittingNumber);
    void gatheringData (int splittingNumber);
    void showData (int splittingNumber, int complexity, QColor graphColor);
};

#endif // MAINWINDOW_H
