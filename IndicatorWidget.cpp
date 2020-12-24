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

    // rect() returns QRect(0, 0, width(), height())
    QRect r = rect();

    // need to pull the bottom right corner in one point
    r.adjust(0, 0, -1, -1);

    QBrush brush;
    brush.setStyle(Qt::SolidPattern);

    if (mIsActive)
        brush.setColor(Qt::red);
    else
        brush.setColor(Qt::green);

    // Set the custom brush. The default pen is fine.
    painter.setBrush(brush);

    painter.drawEllipse(r);
}
