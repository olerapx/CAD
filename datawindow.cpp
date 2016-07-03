#include "datawindow.h"
#include "ui_datawindow.h"

DataWindow::DataWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DataWindow)
{
    ui->setupUi(this);
}

DataWindow::~DataWindow()
{
    delete ui;
}

void DataWindow::setData(QString data)
{
    ui->textBrowser->setText(data);
}

void DataWindow::on_exitButton_clicked()
{
    this->close();
}
