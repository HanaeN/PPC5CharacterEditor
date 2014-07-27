#include "timelinedelegate.h"
#include "timelinemodel.h"
#include <QPainter>
#include <QDebug>
#include <QEvent>
#include <QMouseEvent>

TimelineDelegate::TimelineDelegate(QObject *parent) :
    QStyledItemDelegate(parent)
{
}

QSize TimelineDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const {
//    if (!index.parent().isValid()) return QSize(0, 20);
    CharacterObject *o = (CharacterObject*)index.internalPointer();
    return QSize(0, 16);
}

void TimelineDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const {
    QStyledItemDelegate::paint(painter, option, index);
    int marginL = 15;
    int sX = option.rect.left() + marginL;
    int sY = option.rect.top();
    int h = option.rect.height();
    QLinearGradient bgGradient(0, sY, 0, sY + h);
    bgGradient.setColorAt(0, Qt::white);
    bgGradient.setColorAt(1, QColor(248, 248, 248));
    painter->setPen(QColor(128, 128, 128));
    TimelineObject *t = (TimelineObject*)index.internalPointer();
    if (t->type == TimelineObject::KeyObject) {
        painter->fillRect(option.rect.adjusted(marginL, 0, 0, -1), bgGradient);
        painter->drawRect(option.rect.adjusted(marginL, 0, 0, -1));
        int cells = ceil(option.rect.width() / 10);
        KeyframeList *o = (KeyframeList*)index.internalPointer();

        painter->setPen(QColor(128, 128, 128, 50));
        for (int n = 0; n <= cells; n++) {
            if (n + scrollX > 600) break;
            painter->drawLine(QPoint(sX, sY + 1), QPoint(sX, sY + h - 2));
            sX += 10;
        }
        sX = option.rect.left() + marginL;
        if (selectedRow == index.row() && selectedParent == index.parent()) {
            int selX = sX + ((selectedX - scrollX) * 10);
            if (selX >= sX) {
                int selY = sY + 1;
                QLinearGradient fgGradient(0, selY, 0, selY + 15);
                fgGradient.setColorAt(0, QColor(200, 200, 255));
                fgGradient.setColorAt(1, QColor(170, 170, 200));
                painter->fillRect(QRect(selX + 1, selY, 9, 15), fgGradient);
            }
        }
    }
    painter->setPen(QColor(128, 0, 0));
    int timeX = sX + ((timelinePos - scrollX) * 10);
    if (timeX >= sX) painter->drawLine(QPoint(timeX, sY), QPoint(timeX, sY + h));
}

bool TimelineDelegate::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index) {
    TimelineObject *t = (TimelineObject*)index.internalPointer();
    if (t->type == TimelineObject::KeyObject) {
        if (event->type() != QEvent::MouseButtonPress && event->type() != QEvent::MouseButtonRelease && event->type() != QEvent::MouseMove) return true;
        QMouseEvent *e = (QMouseEvent*)event;
        if (e->buttons() == Qt::LeftButton) { // track
            int mX = floor((e->x() - option.rect.left() - 15) / 10);
            mX = mX + scrollX;
            int mY = floor((e->y() - option.rect.top()) / 16);
            if (mX > 599) mX = 599;
            if (mX < 0) mX = 0;
            selectedRow = index.row();
            selectedParent = index.parent();
            selectedX = mX;
            emit this->timelinePositionChange(mX);
        }
    }
    return false;
}
