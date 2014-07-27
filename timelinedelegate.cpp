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
    int h = (o->parameters.count()) * 16;
    if (h < 16) h = 16;
    return QSize(0, h);
}

void TimelineDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const {
    QStyledItemDelegate::paint(painter, option, index);
    int marginL = 50;
    int sX = option.rect.left() + marginL;
    int sY = option.rect.top();
    int h = option.rect.height();
    QLinearGradient bgGradient(0, sY, 0, sY + h);
    bgGradient.setColorAt(0, Qt::white);
    bgGradient.setColorAt(1, QColor(248, 248, 248));
    painter->setPen(QColor(128, 128, 128));
    painter->fillRect(option.rect.adjusted(marginL, 0, 0, -1), bgGradient);
    painter->drawRect(option.rect.adjusted(marginL, 0, 0, -1));
    int cells = ceil(option.rect.width() / 10);
    painter->setPen(QColor(128, 128, 128, 50));
    CharacterObject *o = (CharacterObject*)index.internalPointer();
    for (int y = 1; y <= o->parameters.count(); y++) {
        painter->drawLine(QPoint(sX, sY + (y * 16)), QPoint(option.rect.right(), sY + (y * 16)));
    }
    painter->setPen(QColor(128, 128, 128, 255));
    for (int y = 0; y < o->parameters.count(); y++) {
        painter->drawText(QRect(sX - marginL, sY + (y * 16), marginL - 4, 16), o->parameters[y]->propertyName, QTextOption(Qt::AlignRight));
    }
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
            int selY = sY + (selectedY * 16) + 1;
            QLinearGradient fgGradient(0, selY, 0, selY + 15);
            fgGradient.setColorAt(0, QColor(200, 200, 255));
            fgGradient.setColorAt(1, QColor(170, 170, 200));
            painter->fillRect(QRect(selX + 1, selY, 9, 15), fgGradient);
        }
    }
    painter->setPen(QColor(128, 0, 0));
    int timeX = sX + ((timelinePos - scrollX) * 10);
    if (timeX >= sX) painter->drawLine(QPoint(timeX, sY), QPoint(timeX, sY + h));
}

bool TimelineDelegate::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index) {
    if (event->type() != QEvent::MouseButtonPress && event->type() != QEvent::MouseButtonRelease && event->type() != QEvent::MouseMove) return true;
    QMouseEvent *e = (QMouseEvent*)event;
    mouseDown = false;
    if (e->buttons() == Qt::LeftButton) mouseDown = true;
    if (mouseDown) { // track
        int mX = floor((e->x() - option.rect.left() - 50) / 10);
        mX = mX + scrollX;
        int mY = floor((e->y() - option.rect.top()) / 16);
        if (mX > 599) mX = 599;
        if (mX < 0) mX = 0;
        selectedRow = index.row();
        selectedParent = index.parent();
        selectedX = mX;
        selectedY = mY;
        emit this->timelinePositionChange(mX);
    }
    return false;
}
