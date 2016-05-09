#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <ctime>

#include "hgraph.h"

namespace Ui {
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

    HGraph ** hGraph;
    HGraph **** hierarhyHG;
    int ** increaseOfCountExternalEdges;
    int countHG;
    int minNumberSubHG;

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

    void resetGraphs();

    double getMax(QVector<double> vector);

    void initHierarhyHG ();                       // Инициализация массивов для
                                                   // иерархического разбиения

     void gatheringData ();                        // Проведение иерархического
                                                   // разбиения и сбор данных
                                                   // об изменении числа внешних связей

     void showData (int Complexity);               // Вывод данных на график
                                                   // "затраты в шагах" в зависимости
                                                   // от сложности алгоритма трассировки
};

#endif // MAINWINDOW_H
