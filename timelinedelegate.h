#ifndef TIMELINEDELEGATE_H
#define TIMELINEDELEGATE_H

#include <QStyledItemDelegate>

struct KeyframeObject;

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
    QModelIndex selectedParent;
signals:
    void timelinePositionChange(int x);
    void keyframeSelected(KeyframeObject *obj);
public slots:

};

#endif // TIMELINEDELEGATE_H
