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

