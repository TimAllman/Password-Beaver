/*
 * mainwindow.h
 *
 * Copyright © 2019 Tim Allman
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

/*
 * Application icon made by Flaticon https://www.flaticon.com/authors/freepik
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "Settings.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

    void testGenerator();

private slots:
    void onAboutActionTriggered();
    void onExitActionTriggered();
    void onGeneratePushButtonClicked();
    void onPunctuationCheckboxStateChanged(int state);
    void onDigitsCheckboxStateChanged(int state);
    void onUpperCaseCheckboxStateChanged(int state);
    void onLowerCaseCheckboxStateChanged(int state);
    void onCopyToClipboardCheckboxStateChanged(int state);
    void onExtendedAsciiCheckboxStateChanged(int state);
    void onPasswordLengthSpinBoxValueChanged(int length);
    void onExcludeCharactersCheckBoxStateChanged(int state);
    void onExcludeCharsLineEditEditingFinished();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
