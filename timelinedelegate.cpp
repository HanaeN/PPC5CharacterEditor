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
        for (int n = 0; n <= cells; n++) {  // draw background lines
            if (n + scrollX > 600) break;
            painter->drawLine(QPoint(sX, sY + 1), QPoint(sX, sY + h - 2));
            sX += 10;
        }
        sX = option.rect.left() + marginL;
        QLinearGradient fgGradient(0, sY + 2, 0, sY + 15);
        fgGradient.setColorAt(0, QColor(50, 50, 50, 128));
        fgGradient.setColorAt(0.5, QColor(90, 90, 90));
        fgGradient.setColorAt(1, QColor(0, 0, 0, 128));
        painter->setPen(QColor(0, 0, 0, 180));
        painter->setClipRect(sX, sY, option.rect.right() - marginL, h);
        for (int n = 0; n < o->keyframes.count(); n++) {
            int cellLength = 0;
            KeyframeObject &obj = o->keyframes[n];
            if (obj.frameIndex - scrollX > cells) break;
            if (obj.frameIndex - scrollX >= 0 || (n < o->keyframes.count() - 1 && o->keyframes[n + 1].frameIndex - scrollX >= 0)) { // draw it
                int kX = sX + ((obj.frameIndex - scrollX) * 10);
                if (n < o->keyframes.count() - 1) {
                    cellLength = (o->keyframes[n + 1].frameIndex - obj.frameIndex) - 1;
                }
                QLinearGradient bgGradient2(kX, 0, kX + 7 + (cellLength * 10), 0);
                bgGradient2.setColorAt(0, QColor(128, 128, 255, 100));
                bgGradient2.setColorAt(1, QColor(160, 160, 255, 100));
                painter->fillRect(QRect(kX + 1, sY + 2, 9 + (cellLength * 10), 13), bgGradient2);
                painter->fillRect(QRect(kX + 2, sY + 2, 7, 13), fgGradient);
                if (cellLength > 0) {
                    int eX = kX + 7 + (cellLength * 10);
                    painter->drawLine(QPoint(kX + 9, sY + 8), QPoint(eX, sY + 8));
                    painter->drawLine(QPoint(eX - 3, sY + 5), QPoint(eX, sY + 8));
                    painter->drawLine(QPoint(eX - 3, sY + 12), QPoint(eX, sY + 9));
                }
            }
        }
        painter->setClipping(false);
        if (selectedRow == index.row() && selectedParent == index.parent()) {
            int selX = sX + ((selectedX - scrollX) * 10);
            if (selX >= sX) {
                int selY = sY + 1;
                QLinearGradient fgGradient(0, selY, 0, selY + 15);
                fgGradient.setColorAt(0, QColor(40, 40, 255, 128));
                fgGradient.setColorAt(1, QColor(80, 80, 255, 128));
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
            if (mX > 599) mX = 599;
            if (mX < 0) mX = 0;
            selectedRow = index.row();
            selectedParent = index.parent();
            selectedX = mX;

            KeyframeList *o = (KeyframeList*)index.internalPointer();
            bool emitted = false;
            for (int n = 0; n < o->keyframes.count(); n++) {
                KeyframeObject &obj = o->keyframes[n];
                if (obj.frameIndex == mX) {
                    emitted = true;
                    emit keyframeSelected(&o->keyframes[n]);
                    break;
                }
            }
            if (!emitted) {
                for (int n = 0; n < o->keyframes.count(); n++) {
                    KeyframeObject &obj = o->keyframes[n];
                    if (obj.frameIndex < mX && (n < o->keyframes.count() - 1 && o->keyframes[n + 1].frameIndex > mX)) {
                        emitted = true;
                        emit keyframeSelected(&o->keyframes[n]);
                        break;
                    }
                }
            }
            if (!emitted) {
                emit keyframeSelected(NULL);
            }
            emit this->timelinePositionChange(mX);
        }
    }
    return false;
}
