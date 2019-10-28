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

#include <QClipboard>
#include <QIcon>
#include <QMessageBox>
#include <QCloseEvent>

#include "Global.h"
#include "MainWindow.h"
#include "ui_mainwindow.h"
#include "PasswordGenerator.h"
#include "Settings.h"
#include "AboutDialog.h"
#include "Exceptions.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // This should happen first so that the proper path to the configuration
    // is set properly.
    QCoreApplication::setOrganizationName(Global::ORGANIZATION_NAME);
    QCoreApplication::setOrganizationDomain(Global::ORGANIZATION_DOMAIN);
    QCoreApplication::setApplicationName(Global::APPLICATION_NAME);
    Settings& settings = Settings::instance();

    connect(ui->actionAbout_Qt, &QAction::triggered, qApp, &QApplication::aboutQt);
    connect(ui->aboutAction, &QAction::triggered, this, &MainWindow::onAboutActionTriggered);
    connect(ui->exitAction, &QAction::triggered, this, &MainWindow::onExitActionTriggered);
    connect(ui->quitPushButton, &QPushButton::clicked, this, &MainWindow::onExitActionTriggered);
    connect(ui->generatePushButton, &QPushButton::clicked, this, &MainWindow::onGeneratePushButtonClicked);

    connect(ui->punctuationCheckBox, SIGNAL(stateChanged(int)),
            this, SLOT(onPunctuationCheckboxStateChanged(int)));
    connect(ui->symbolsCheckBox, SIGNAL(stateChanged(int)),
            this, SLOT(onSymbolsCheckboxStateChanged(int)));
    connect(ui->digitsCheckBox, SIGNAL(stateChanged(int)),
            this, SLOT(onDigitsCheckboxStateChanged(int)));
    connect(ui->upperCaseCheckBox, SIGNAL(stateChanged(int)),
            this, SLOT(onUpperCaseCheckboxStateChanged(int)));
    connect(ui->lowerCaseCheckBox, SIGNAL(stateChanged(int)),
            this, SLOT(onLowerCaseCheckboxStateChanged(int)));

    connect(ui->copyToClipboardCheckBox, SIGNAL(stateChanged(int)),
            this, SLOT(onCopyToClipboardCheckboxStateChanged(int)));
    connect(ui->extendedAsciiCheckBox, SIGNAL(stateChanged(int)),
            this, SLOT(onExtendedAsciiCheckboxStateChanged(int)));
    connect(ui->excludeCheckBox, SIGNAL(stateChanged(int)),
            this, SLOT(onExcludeCharactersCheckBoxStateChanged(int)));

    connect(ui->excludeCharsLineEdit, SIGNAL(editingFinished()),
            this, SLOT(onExcludeCharsLineEditEditingFinished()));
    connect(ui->excludeCharsLineEdit, SIGNAL(textChanged(const QString&)),
            this, SLOT(onExcludeCharsLineEditTextChanged()));

    ui->punctuationCheckBox->setCheckState(static_cast<Qt::CheckState>(settings.usePunctuation()));
    ui->symbolsCheckBox->setCheckState(static_cast<Qt::CheckState>(settings.useSymbols()));
    ui->digitsCheckBox->setCheckState(static_cast<Qt::CheckState>(settings.useDigits()));
    ui->upperCaseCheckBox->setCheckState(static_cast<Qt::CheckState>(settings.useUpperAlpha()));
    ui->lowerCaseCheckBox->setCheckState(static_cast<Qt::CheckState>(settings.useLowerAlpha()));
    ui->copyToClipboardCheckBox->setChecked(settings.copyToClipboard());
    ui->extendedAsciiCheckBox->setChecked(settings.useExtendedAscii());

    ui->passwordLengthSpinBox->setRange(Global::MIN_PW_LENGTH, Global::MAX_PW_LENGTH);
    int pwlen = static_cast<int>(settings.passwordLength());
    if (pwlen >= Global::MIN_PW_LENGTH && pwlen <= Global::MAX_PW_LENGTH)
        ui->passwordLengthSpinBox->setValue(pwlen);
    else
    {
        ui->passwordLengthSpinBox->setValue(Global::DEFAULT_PW_LENGTH);
        settings.setPasswordLength(Global::DEFAULT_PW_LENGTH);
    }

    connect(ui->passwordLengthSpinBox, SIGNAL(valueChanged(int)),
            this, SLOT(onPasswordLengthSpinBoxValueChanged(int)));

    ui->excludeCheckBox->setChecked(settings.excludeCharacters());
    ui->excludeCharsLineEdit->setText(settings.charactersToExclude());
    ui->excludeCharsLineEdit->setEnabled(settings.excludeCharacters());

    setPoolSizeLineEditText();

    restoreGeometry(settings.windowGeometry());
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::testGenerator()
{

}

void MainWindow::setPoolSizeLineEditText()
{
    Settings& settings = Settings::instance();

    CharacterPool pool(settings.useExtendedAscii(), settings.excludeCharacters(),
                       settings.charactersToExclude(), settings.usePunctuation(),
                       settings.useDigits(), settings.useUpperAlpha(),
                       settings.useLowerAlpha(), settings.useSymbols());

    QString strSize = QString::number(pool.poolSize());
    QPalette palette;

    if (pool.poolSize() < Global::MIN_POOL_LENGTH)
    {
        palette.setColor(QPalette::Text,Qt::red);
    }
    else
    {
        palette.setColor(QPalette::Text,Qt::black);
    }

    ui->poolSizeLineEdit->setPalette(palette);
    ui->poolSizeLineEdit->setText(strSize);

}

void MainWindow::closeEvent(QCloseEvent* event)
{
    // We'll just call @c onExitActionTriggered().
    event->accept();
    onExitActionTriggered();
}

void MainWindow::onAboutActionTriggered()
{
    AboutDialog dlg;
    dlg.exec();
}

void MainWindow::onExitActionTriggered()
{
    Settings::instance().setWindowGeometry(saveGeometry());
    close();
}

void MainWindow::onGeneratePushButtonClicked()
{
    PasswordGenerator gen;
    QString password;

    try
    {
        password = gen.password();
    }

    catch(ExclusionException ex)
    {
        QMessageBox msgBox(QMessageBox::Critical, tr("Password Beaver Error"), ex.what(),
                           nullptr, this);

        msgBox.setDetailedText(tr("Some required characters are not available because "
                                  "they have been manually excluded. Remove at least one of these "
                                  "characters from the \"Exclude these characters\" editor or use "
                                  "the check box to turn off this feature"));
        msgBox.addButton(QMessageBox::Ok);
        msgBox.exec();

        return;
    }

    catch(SmallCharacterPoolException ex)
    {
        QMessageBox msgBox(QMessageBox::Critical, tr("Password Beaver Error"), ex.what(),
                           nullptr, this);

        msgBox.setDetailedText(tr("The character pool from which the password is derived "
                                  "is too small. Add more character classes or remove some "
                                  "from the exclusion list."));
        msgBox.addButton(QMessageBox::Ok);
        msgBox.exec();

        return;
    }

    QClipboard* clip = QGuiApplication::clipboard();
    if (Settings::instance().copyToClipboard())
        clip->setText(password);

    ui->passwordLineEdit->setText(password);
    QString entrStr = QString::number(gen.entropy(), 'f', 1) + " bits";
    ui->entropyLineEdit->setText(entrStr);
}

void MainWindow::onHelpActionTriggered()
{

}

void MainWindow::onPunctuationCheckboxStateChanged(int state)
{
    Settings::instance().setUsePunctuation(state);
    setPoolSizeLineEditText();
}

void MainWindow::onSymbolsCheckboxStateChanged(int state)
{
    Settings::instance().setUseSymbols(state);
    setPoolSizeLineEditText();
}

void MainWindow::onDigitsCheckboxStateChanged(int state)
{
    Settings::instance().setUseDigits(state);
    setPoolSizeLineEditText();
}

void MainWindow::onUpperCaseCheckboxStateChanged(int state)
{
    Settings::instance().setUseUpperAlpha(state);
    setPoolSizeLineEditText();
}

void MainWindow::onLowerCaseCheckboxStateChanged(int state)
{
    Settings::instance().setUseLowerAlpha(state);
    setPoolSizeLineEditText();
}

void MainWindow::onCopyToClipboardCheckboxStateChanged(int state)
{
    Settings::instance().setCopyToClipboard(state);
}

void MainWindow::onExtendedAsciiCheckboxStateChanged(int state)
{
    Settings::instance().setUseExtendedAscii(state);
    setPoolSizeLineEditText();
}

void MainWindow::onPasswordLengthSpinBoxValueChanged(int length)
{
    Settings::instance().setPasswordLength(length);
}

void MainWindow::onExcludeCharactersCheckBoxStateChanged(int state)
{
    Settings::instance().setExcludeCharacters(state);
    ui->excludeCharsLineEdit->setEnabled(state);
    setPoolSizeLineEditText();
}

void MainWindow::onExcludeCharsLineEditTextChanged()
{
    Settings::instance().setCharactersToExclude(ui->excludeCharsLineEdit->text());
    setPoolSizeLineEditText();
}

void MainWindow::onExcludeCharsLineEditEditingFinished()
{
    Settings::instance().setCharactersToExclude(ui->excludeCharsLineEdit->text());
    setPoolSizeLineEditText();
}

