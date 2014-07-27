#include "timelineheaderview.h"
#include <QPainter>
#include <QDebug>

TimelineHeaderView::TimelineHeaderView(Qt::Orientation orientation, QWidget *parent) :
    QHeaderView(orientation, parent)
{
}

int TimelineHeaderView::sectionSizeHint(int logicalIndex) const {
    return 300;
}

void TimelineHeaderView::paintSection(QPainter *painter, const QRect &rect, int logicalIndex) const {
    painter->setClipRect(rect);
    int marginL = 50;
    int sX = rect.left() + marginL;
    int sY = rect.top();
    int h = rect.height();
    QLinearGradient bgGradient(0, sY, 0, sY + h);
    bgGradient.setColorAt(0, Qt::white);
    bgGradient.setColorAt(1, QColor(230, 230, 230));
    painter->setPen(QColor(128, 128, 128));
    painter->fillRect(rect, bgGradient);
    if (logicalIndex == 1) {
        painter->setPen(QColor(128, 128, 128, 200));
        painter->drawLine(QPoint(rect.left() + 1, sY + 2), QPoint(rect.left() + 1, sY + h - 4));
        int cells = ceil(rect.width() / 10);
        painter->setPen(QColor(0, 0, 0));
        for (int n = 0; n <= cells; n++) {
            if (n + scrollX > 600) break;
            int tick = 0;
            if ((n + scrollX) % 10 == 0) tick = - 6;
            painter->drawLine(QPoint(sX, sY + 26 + tick), QPoint(sX, sY + h));
            if (tick == -6) {
                painter->drawText(QRect(sX - 15, sY + 4, 30, h), QString::number(n + scrollX), QTextOption(Qt::AlignHCenter));
            }
            sX += 10;
        }
    } else {
        painter->setPen(QColor(128, 128, 128, 200));
        painter->drawLine(QPoint(rect.right() - 1, sY + 2), QPoint(rect.right() - 1, sY + h - 4));
    }
}

QSize TimelineHeaderView::sizeHint() const {
    return QSize(0, 30);
}
