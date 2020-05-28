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
#ifndef INDICATORWIDGET_H
#define INDICATORWIDGET_H

#include <QFrame>

class IndicatorWidget : public QFrame
{
public:
    IndicatorWidget(QWidget* parent = nullptr);

    void setActive(bool isActive);

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    bool mIsActive;
};

#endif // INDICATORWIDGET_H
