#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "hgraph.h"

namespace Ui {
class MainWindow;
}

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

private:
    Ui::MainWindow *ui;

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
