#ifndef TIMELINEHEADERVIEW_H
#define TIMELINEHEADERVIEW_H

#include <QHeaderView>

class TimelineHeaderView : public QHeaderView
{
    Q_OBJECT
public:
    TimelineHeaderView(Qt::Orientation orientation, QWidget *parent = 0);
    int sectionSizeHint(int logicalIndex) const;
    int scrollX = 0;
    int timelinePos = 0;
    QSize sizeHint() const;
protected:
    void paintSection(QPainter *painter, const QRect &rect, int logicalIndex) const;
signals:

public slots:

};

#endif // TIMELINEHEADERVIEW_H
