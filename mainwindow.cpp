#include "mainwindow.h"
#include "timelinemodel.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    timeline = new TimelineModel(this);
//    ui->e_Editor->currentIndex();
    ui->e_Editor->setModel(timeline);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_action_Exit_triggered()
{
    this->close();
}
