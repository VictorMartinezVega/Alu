#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::on_suma_clicked()
{


    float n1= this->ui->op1dec->text().toFloat();
    float n2 = this->ui->op2dec->text().toFloat();

}


void MainWindow::on_resta_clicked()
{

}


void MainWindow::on_producto_clicked()
{

}


void MainWindow::on_division_clicked()
{

}

