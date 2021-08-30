/*
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

#include "HelpDialog.h"

#include <QMainWindow>

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
    /**
     * Constructor.
     * @param parent Probably null.
     */
    explicit MainWindow(QWidget *parent = nullptr);

    /**
     * Destructor
     */
    ~MainWindow() override;

protected:
    /**
     * This overrides the @c QWidget function. It catches the close event
     * generated by clicking the <i>close</i> button on the MainWindow's frame.
     * @param event The generated event. We just @code accept() the event to let the
     * system know that it has been dealt with.
     */
    virtual void closeEvent(QCloseEvent *event) override;

private:
    /**
     * Displays the size of the character pool.
     */
    void setPoolSizeLineEditText();

    /**
     * Displays all of the current options. @see OptionsManager.
     */
    void displayCurrentOptions();

    /**
     * Manage the GUI to reflect the state of the options. @see OptionsManager.
     */
    void updateGui();

private slots:
    /**
     * Slot to show the About dialog.
     */
    void onAboutActionTriggered();

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
     * Slot to respond to the punctuation checkbox.
     * @param state Can be EXCLUDE, REQUEST or REQUIRE.
     */
    void onSymbolsCheckboxStateChanged(int state);

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
     * @param state The new state of the checkbox.
     */
    void onCopyToClipboardCheckboxStateChanged(int state);

    void onClipboardClearTimeSpinBoxValueChanged(int value);

    /**
     * Slot to respond to the extended ASCII checkbox.
     * @param state The new state of the checkbox.
     */
    void onExtendedAsciiCheckboxStateChanged(int state);

    /**
     * Slot to respond to changes in the password length spin box.
     * @param length The new value for the password length.
     */
    void onPasswordLengthSpinBoxValueChanged(int length);

    /**
     * Slot to respond to changes in the characters to exclude editor.
     */
    void onExcludeCharsLineEditEditingFinished();

    /**
     * Slot to respond to changes in the excluded characters editor.
     */
    void onExcludeCharsLineEditTextChanged();

    /**
     * Slot to respond to finishing editing in the editor in the combobox.
     */
    void onOptionsNameComboBoxEditingFinished();

    /**
     * Slot to respond to changes in the editor in the combobox.
     */
    void onOptionsNameComboBoxEditTextChanged();

    /**
     * Slot to respond to selection of a new name in the combobox.
     * @param index The index of the new selection.
     */
    void onOptionsNameComboBoxCurrentIndexChanged(int index);

    /**
     * Slot to respond to user clicking the 'Save' pushbutton.
     * @param bool Not used.
     */
    void onSaveOptionsPushButtonClicked(bool);

    /**
     * Slot to respond to user clicking the 'Delete' pushbutton.
     * @param bool Not used.
     */
    void onDeleteOptionsPushButtonClicked(bool);

    /**
     * Slot to show the manual.
     */
    void onShowManualTriggered();

    /**
     * Clear clipboard if timer triggers it.
     */
    void clearClipboard();

private:

    HelpDialog* helpDlg;

    /**
     * The GUI.
     */
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
