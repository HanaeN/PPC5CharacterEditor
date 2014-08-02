#ifndef TIMELINEDELEGATE_H
#define TIMELINEDELEGATE_H

#include <QStyledItemDelegate>
#include "timelinemodel.h"


class TimelineDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit TimelineDelegate(QObject *parent = 0);
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;
    bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index);
    int scrollX = 0;
    int timelinePos = 0;
    int selectedRow = -1;
    int selectedX = 0;
    QModelIndex selectedParent;
private:
    QImage cellImg = QImage(":/gfx/images/cells.png");
signals:
    void contextMenu(CharacterObject *obj, QString propertyName, Keyframe::PropertyType propertyType, int index, QPoint mPos);
    void timelinePositionChange(int x);
    void keyframeSelected(KeyframeObject *obj, bool isAtX, bool isInt);
public slots:

};

#endif // TIMELINEDELEGATE_H
