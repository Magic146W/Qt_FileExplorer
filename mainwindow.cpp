#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "fileexplorer.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    fileExplorer = new FileExplorer(this);


}

MainWindow::~MainWindow()
{
    delete ui;
}

