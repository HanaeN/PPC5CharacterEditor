#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class TimelineModel;
class TimelineDelegate;
class TimelineHeaderView;
struct KeyframeObject;

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
    void onKeyframeSelected(KeyframeObject* obj, bool isint);
    void on_e_value_textEdited(const QString &arg1);
    void on_e_value_editingFinished();
    void on_c_easingin_toggled(bool checked);
    void on_c_easingout_toggled(bool checked);
    void on_e_easingout_currentIndexChanged(int index);
    void on_e_easingin_currentIndexChanged(int index);

private:
    KeyframeObject *editingKeyframe = NULL;
    bool isInt = true;
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
