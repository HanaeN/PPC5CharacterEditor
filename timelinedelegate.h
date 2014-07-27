#ifndef TIMELINEDELEGATE_H
#define TIMELINEDELEGATE_H

#include <QStyledItemDelegate>

class TimelineDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit TimelineDelegate(QObject *parent = 0);
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;
    bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index);
    int scrollX = 0;
    int timelinePos = 100;
    int selectedRow = -1;
    int selectedX = 0;
    int selectedY = 0;
    QModelIndex selectedParent;
private:
    bool mouseDown = false;
signals:
    void timelinePositionChange(int x);
public slots:

};

#endif // TIMELINEDELEGATE_H
