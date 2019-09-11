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

/**
 * The application's main window.
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

    void testGenerator();

private slots:
    /**
     * Slot to show the About dialog.
     */
    void onAboutActionTriggered();

    /**
     * Slot to exit the program.
     */
    void onExitActionTriggered();

    /**
     * Slot to show help.
     */
    void onHelpActionTriggered();

    /**
     * Slot to generate a password.
     */
    void onGeneratePushButtonClicked();

    /**
     * Slot to respond to the punctuation checkbox.
     * @param state Can be EXCLUDE, REQUEST or REQUIRE.
     */
    void onPunctuationCheckboxStateChanged(int state);

    /**
     * Slot to respond to the digits checkbox.
     * @param state Can be EXCLUDE, REQUEST or REQUIRE.
     */
    void onDigitsCheckboxStateChanged(int state);

    /**
     * Slot to respond to the upper case checkbox.
     * @param state Can be EXCLUDE, REQUEST or REQUIRE.
     */
    void onUpperCaseCheckboxStateChanged(int state);

    /**
     * Slot to respond to the lower case checkbox.
     * @param state Can be EXCLUDE, REQUEST or REQUIRE.
     */
    void onLowerCaseCheckboxStateChanged(int state);

    /**
     * Slot to respond to the copy to clipboard checkbox.
     * @param checked The new state of the checkbox
     */
    void onCopyToClipboardCheckboxClicked(bool checked);

    /**
     * Slot to respond to the extended ASCII checkbox.
     * @param checked The new state of the checkbox
     */
    void onExtendedAsciiCheckboxStateClicked(bool checked);

    /**
     * Slot to respond to changes in the password length spin box.
     * @param checked The new state of the checkbox
     */
    void onPasswordLengthSpinBoxValueChanged(int length);

    /**
     * Slot to respond to the exclude characters checkbox.
     * @param checked The new state of the checkbox
     */
    void onExcludeCharactersCheckBoxClicked(bool checked);

    /**
     * Slot to respond to changes in the characters to exclude editor.
     * @param checked The new state of the checkbox
     */
    void onExcludeCharsLineEditEditingFinished();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
