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
    //repaint(rect().x()+1, rect().y()+1, -rect().width(), -rect().height());
    repaint();
}

void IndicatorWidget::paintEvent(QPaintEvent*)
{
    QPainter painter(this);

    QRect r = rect();
    r.adjust(0, 0, -1, -1);
    painter.drawRect(r);

    if (mIsActive)
    {
        r.adjust(1, 1, -1, -1);
        painter.setPen(Qt::red);
        painter.setBrush(Qt::red);
        painter.drawRect(r);
    }
}
