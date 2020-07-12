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
/**
 * This is a small widget which is used to indicate that the current options are
 * not identical with any in the @c OptionsManager.
 */
class IndicatorWidget : public QFrame
{
public:
    /**
     * Constructor.
     * @param parent The parent window.
     */
    IndicatorWidget(QWidget* parent = nullptr);

    /**
     * Used top turn this on and off.
     * @param isActive Show 'on' state when true, 'off' if false.
     */
    void setActive(bool isActive);

protected:
    /**
     * Required paint event so that the system can draw the widget.
     * @param event The event.
     */
    void paintEvent(QPaintEvent* event) override;

private:
    bool mIsActive; ///< Variable to hold state.
};

#endif // INDICATORWIDGET_H
