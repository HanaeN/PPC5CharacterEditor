#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class TimelineModel;
class TimelineDelegate;
class TimelineHeaderView;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    TimelineModel *timeline;
    TimelineDelegate *delegate;
    TimelineHeaderView *headerView;
private slots:
    void on_action_Exit_triggered();
    void on_e_timelineScroll_sliderMoved(int position);
    void on_e_timelineScroll_valueChanged(int value);
    void onTimelinePositionChanged(int x);
private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
