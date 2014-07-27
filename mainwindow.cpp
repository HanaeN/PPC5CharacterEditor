#include "mainwindow.h"
#include "timelinemodel.h"
#include "timelinedelegate.h"
#include "timelineheaderview.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    timeline = new TimelineModel(this);
    delegate = new TimelineDelegate(ui->e_Editor);
    headerView = new TimelineHeaderView(Qt::Horizontal, ui->e_Editor);
    headerView->setStretchLastSection(true);
    ui->e_Editor->setHeader(headerView);
    ui->e_Editor->setItemDelegateForColumn(1, delegate);
    ui->e_Editor->setModel(timeline);
    ui->e_Editor->expandAll();
    ui->e_Editor->setMouseTracking(true);
    headerView->resizeSection(0, 200);
    connect(delegate, SIGNAL(timelinePositionChange(int)), this, SLOT(onTimelinePositionChanged(int)));
}


MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_action_Exit_triggered()
{
    this->close();
}

void MainWindow::on_e_timelineScroll_sliderMoved(int position)
{
//    delegate->scrollX = position;
//    headerView->scrollX = position;
//    ui->e_Editor->reset();
}

void MainWindow::on_e_timelineScroll_valueChanged(int value)
{
    delegate->scrollX = value;
    headerView->scrollX = value;
    headerView->viewport()->update();
    ui->e_Editor->viewport()->update();
}

void MainWindow::onTimelinePositionChanged(int x) {
    delegate->timelinePos = x;
    headerView->timelinePos = x;
    headerView->viewport()->update();
    ui->e_Editor->viewport()->update();
}
