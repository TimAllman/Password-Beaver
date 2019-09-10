/*
 * mainwindow.cpp
 *
 * Copyright © %YEAR% Tim Allman
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

#include <QClipboard>
#include <QIcon>
#include <QMessageBox>

#include "MainWindow.h"
#include "ui_mainwindow.h"
#include "PasswordGenerator.h"
#include "Settings.h"
#include "AboutDialog.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->actionAbout_Qt, &QAction::triggered, qApp, &QApplication::aboutQt);
    connect(ui->aboutAction, &QAction::triggered, this, &MainWindow::onAboutActionTriggered);
    connect(ui->exitAction, &QAction::triggered, this, &MainWindow::onExitActionTriggered);
    connect(ui->quitPushButton, &QPushButton::clicked, this, &MainWindow::onExitActionTriggered);
    connect(ui->generatePushButton, &QPushButton::clicked, this, &MainWindow::onGeneratePushButtonClicked);

    connect(ui->punctuationCheckBox, SIGNAL(stateChanged(int)),
            this, SLOT(onPunctuationCheckboxStateChanged(int)));
    connect(ui->digitsCheckBox, SIGNAL(stateChanged(int)),
            this, SLOT(onDigitsCheckboxStateChanged(int)));
    connect(ui->upperCaseCheckBox, SIGNAL(stateChanged(int)),
            this, SLOT(onUpperCaseCheckboxStateChanged(int)));
    connect(ui->lowerCaseCheckBox, SIGNAL(stateChanged(int)),
            this, SLOT(onLowerCaseCheckboxStateChanged(int)));
    connect(ui->copyToClipboardCheckBox, SIGNAL(stateChanged(int)),
            this, SLOT(onCopyToClipboardCheckboxStateChanged(int)));
    connect(ui->passwordLengthSpinBox, SIGNAL(valueChanged(int)),
            this, SLOT(onPasswordLengthSpinBoxValueChanged(int)));
    connect(ui->extendedAsciiCheckBox, SIGNAL(stateChanged(int)),
            this, SLOT(onExtendedAsciiCheckboxStateChanged(int)));
    connect(ui->excludeCheckBox, SIGNAL(stateChanged(int)),
            this, SLOT(onExcludeCharactersCheckBoxStateChanged(int)));
    connect(ui->excludeCharsLineEdit, SIGNAL(editingFinished()),
            this, SLOT(onExcludeCharsLineEditEditingFinished()));

    //    connect(ui->excludeComboBox, SIGNAL(editTextChanged(const QString&)),
    //            this, SLOT(onCharactersToExcludeComboBoxEditTextChanged(const QString&)));
    //    connect(ui->excludeComboBox, SIGNAL(currentIndexChanged(int)),
    //            this, SLOT(onCharactersToExcludeComboBoxCurrentIndexChanged(int)));

    QCoreApplication::setOrganizationName("Brasscats");
    QCoreApplication::setOrganizationDomain("brasscats.ca");
    QCoreApplication::setApplicationName("Password Beaver");

    Settings& settings = Settings::instance();
    ui->punctuationCheckBox->setChecked(static_cast<Qt::CheckState>(settings.usePunctuation()));
    ui->digitsCheckBox->setCheckState(static_cast<Qt::CheckState>(settings.useDigits()));
    ui->upperCaseCheckBox->setChecked(static_cast<Qt::CheckState>(settings.useUpperAlpha()));
    ui->lowerCaseCheckBox->setChecked(static_cast<Qt::CheckState>(settings.useLowerAlpha()));
    ui->copyToClipboardCheckBox->setChecked(settings.copyToClipboard());
    ui->extendedAsciiCheckBox->setChecked(settings.useExtendedAscii());
    ui->passwordLengthSpinBox->setRange(8, 20);
    int pwlen = static_cast<int>(settings.passwordLength());
    if (pwlen > 7 && pwlen < 21)
        ui->passwordLengthSpinBox->setValue(pwlen);
    else
    {
        ui->passwordLengthSpinBox->setValue(16);
        settings.setPasswordLength(16);
    }
    ui->excludeCheckBox->setChecked(settings.excludeCharacters());
    ui->excludeCharsLineEdit->setText(settings.charactersToExclude());
    ui->excludeCharsLineEdit->setEnabled(settings.excludeCharacters());

    restoreGeometry(settings.windowGeometry());
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::testGenerator()
{

}

void MainWindow::onAboutActionTriggered()
{
    AboutDialog dlg;
    dlg.exec();
}

void MainWindow::onGeneratePushButtonClicked()
{
    Settings& settings = Settings::instance();
    PasswordGenerator gen;
    QString password;

    try
    {
        password = gen.password();
    }

    catch(std::runtime_error ex)
    {
        QMessageBox msgBox(QMessageBox::Warning, tr("Password Beaver Warning"), ex.what(),
                           nullptr, this);

        msgBox.setDetailedText(tr("Some required characters are not available because "
                                  "they have been manually excluded. Remove at least one of these "
                                  "characters from the exclusion characters editor or use the "
                                  "check box to turn off this feature"));
        msgBox.addButton(QMessageBox::Ok);
        msgBox.exec();

        return;
    }

    QClipboard* clip = QGuiApplication::clipboard();
    if (settings.copyToClipboard())
        clip->setText(password);

    ui->passwordLineEdit->setText(password);
    QString entrStr = QString::number(gen.entropy(), 'f', 1) + " bits";
    ui->entropyLabel->setText(entrStr);
}

void MainWindow::onExitActionTriggered()
{
    Settings& settings = Settings::instance();
    settings.setWindowGeometry(saveGeometry());

    close();
}

void MainWindow::onHelpActionTriggered()
{

}

void MainWindow::onPunctuationCheckboxStateChanged(int state)
{
    Settings::instance().setUsePunctuation(state);
}

void MainWindow::onDigitsCheckboxStateChanged(int state)
{
    Settings::instance().setUseDigits(state);
}

void MainWindow::onUpperCaseCheckboxStateChanged(int state)
{
    Settings::instance().setUseUpperAlpha(state);
}

void MainWindow::onLowerCaseCheckboxStateChanged(int state)
{
    Settings::instance().setUseLowerAlpha(state);
}

void MainWindow::onCopyToClipboardCheckboxStateChanged(int state)
{
    Settings::instance().setCopyToClipboard(state);
}

void MainWindow::onExtendedAsciiCheckboxStateChanged(int state)
{
    Settings::instance().setUseExtendedAscii(state);
}

void MainWindow::onPasswordLengthSpinBoxValueChanged(int length)
{
    Settings::instance().setPasswordLength(length);
}

void MainWindow::onExcludeCharactersCheckBoxStateChanged(int state)
{
    Settings::instance().setExcludeCharacters(state);
    ui->excludeCharsLineEdit->setEnabled(static_cast<bool>(state));
}

void MainWindow::onExcludeCharsLineEditEditingFinished()
{
    Settings::instance().setCharactersToExclude(ui->excludeCharsLineEdit->text());
}

