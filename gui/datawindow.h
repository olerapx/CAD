#ifndef DATAWINDOW_H
#define DATAWINDOW_H

#include <QDialog>

namespace Ui {
class DataWindow;
}

class DataWindow : public QDialog
{
    Q_OBJECT

public:
    explicit DataWindow(QWidget *parent = 0);
    ~DataWindow();

    void setData (QString data);

private slots:
    void on_exitButton_clicked();

private:
    Ui::DataWindow *ui;
};

#endif // DATAWINDOW_H
