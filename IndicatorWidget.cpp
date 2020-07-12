/*
 * Copyright © 2020 Tim Allman
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "IndicatorWidget.h"

#include <QPainter>
//#include <QPaintEvent>

IndicatorWidget::IndicatorWidget(QWidget* parent)
    : QFrame(parent), mIsActive(false)
{
}

void IndicatorWidget::setActive(bool isActive)
{
    mIsActive = isActive;
    repaint();
}

void IndicatorWidget::paintEvent(QPaintEvent*)
{
    QPainter painter(this);

    // rect() returns QPoint(0, 0, width(), height())
    QRect r = rect();

    // need to pull the bottom right corner in one point
    r.adjust(0, r.height()/6, -1, -r.height()/6-1);

    // We want to draw a parallelogram on point so we set up a polygon.
    QPolygonF poly;
    QPointF topPoint(r.x() + r.width() / 2, r.y());
    QPointF rightPoint(QPointF(r.x() + r.width(), r.y() + r.height() / 2));
    QPointF bottomPoint(r.x() + r.width() / 2, r.height());
    QPointF leftPoint(QPointF(r.x(), r.y() + r.height() / 2));
    poly << topPoint << rightPoint << bottomPoint << leftPoint;

//    painter.setPen(Qt::darkGray);
//    painter.setBrush(Qt::darkGray);
//    painter.drawConvexPolygon(poly);
    painter.drawEllipse(r);
//    painter.drawRect(r);
    if (mIsActive)
    {
//        poly.clear();
        r.adjust(1, 1, -1, -1);

//        QPointF topPoint(r.x() + r.width() / 2, r.y());
//        QPointF rightPoint(QPointF(r.x() + r.width(), r.y() + r.height() / 2));
//        QPointF bottomPoint(r.x() + r.width() / 2, r.y() + r.width() / 2);
//        QPointF leftPoint(QPointF(r.x() + r.width(), r.y() + r.height() / 2));

//        poly << topPoint << rightPoint << bottomPoint << leftPoint;
        painter.setPen(Qt::red);
        painter.setBrush(Qt::red);
//        painter.drawConvexPolygon(poly);
        painter.drawEllipse(r);
//        painter.drawRect(r);
    }
}
