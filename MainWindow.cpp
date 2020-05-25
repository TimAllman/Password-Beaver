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
#include <QJsonDocument>
#include <QSettings>

#include "Global.h"
#include "MainWindow.h"
#include "ui_mainwindow.h"
#include "PasswordGenerator.h"
#include "OptionsManager.h"
#include "AboutDialog.h"
#include "Exceptions.h"
#include "OptionsManager.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // This should happen first so that the proper path to the
    // configuration file is set properly.
    QCoreApplication::setOrganizationName(QString(Global::ORGANIZATION_NAME));
    QCoreApplication::setOrganizationDomain(QString(Global::ORGANIZATION_DOMAIN));
    QCoreApplication::setApplicationName(QString(Global::APPLICATION_NAME));

    // Get the OptionsManager instance.
    OptionsManager& optsManager = OptionsManager::instance();

    // Make the initial settings to the UI controls before connecting the
    // signals and slots. Otherwise the values may change because of
    // slots being called before we want them
    ui->punctuationCheckBox->setCheckState(static_cast<Qt::CheckState>(optsManager.usePunctuation()));
    ui->symbolsCheckBox->setCheckState(static_cast<Qt::CheckState>(optsManager.useSymbols()));
    ui->digitsCheckBox->setCheckState(static_cast<Qt::CheckState>(optsManager.useDigits()));
    ui->upperCaseCheckBox->setCheckState(static_cast<Qt::CheckState>(optsManager.useUpperAlpha()));
    ui->lowerCaseCheckBox->setCheckState(static_cast<Qt::CheckState>(optsManager.useLowerAlpha()));
    ui->copyToClipboardCheckBox->setChecked(optsManager.copyToClipboard());
    ui->extendedAsciiCheckBox->setChecked(optsManager.useExtendedAscii());

    ui->passwordLengthSpinBox->setRange(Global::MIN_PW_LENGTH, Global::MAX_PW_LENGTH);
    int pwlen = static_cast<int>(optsManager.passwordLength());
    if (pwlen >= Global::MIN_PW_LENGTH && pwlen <= Global::MAX_PW_LENGTH)
        ui->passwordLengthSpinBox->setValue(pwlen);
    else
    {
        ui->passwordLengthSpinBox->setValue(Global::DEFAULT_PW_LENGTH);
        optsManager.setPasswordLength(Global::DEFAULT_PW_LENGTH);
    }

    ui->excludeCharsLineEdit->setText(optsManager.charsToExclude());

    // Now do the connections.
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

    connect(ui->excludeCharsLineEdit, SIGNAL(editingFinished()),
            this, SLOT(onExcludeCharsLineEditEditingFinished()));
    connect(ui->excludeCharsLineEdit, SIGNAL(textChanged(const QString&)),
            this, SLOT(onExcludeCharsLineEditTextChanged()));

    connect(ui->passwordLengthSpinBox, SIGNAL(valueChanged(int)),
            this, SLOT(onPasswordLengthSpinBoxValueChanged(int)));

    connect(ui->optionsNameComboBox, SIGNAL(editTextChanged(const QString&)),
            this, SLOT(onOptionsNameComboBoxEditTextChanged(const QString&)));

    connect(ui->saveOptionsPushButton, SIGNAL(clicked(bool)),
            this, SLOT(onSaveOptionsPushButtonClicked(int)));

    connect(ui->deleteOptionsPushButton, SIGNAL(clicked(bool)),
            this, SLOT(onDeleteOptionsPushButtonClicked(int)));

    // TODO continue with setting up push buttons
    ui->saveOptionsPushButton->setEnabled(false);
    ui->deleteOptionsPushButton->setEnabled(true);

    setPoolSizeLineEditText();

    QSettings settings;
    restoreGeometry(settings.value("WindowGeometry").toByteArray());
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
    OptionsManager& optsManager = OptionsManager::instance();

    CharacterPool pool(optsManager.useExtendedAscii(), optsManager.charsToExclude(),
                       optsManager.usePunctuation(), optsManager.useDigits(), optsManager.useUpperAlpha(),
                       optsManager.useLowerAlpha(), optsManager.useSymbols());

    // Here we inform the user of the pool size and show it
    // in @c Qt::red if it is too small.
    QString strSize = QString::number(pool.poolSize());
    QPalette palette;

    if (pool.poolSize() < Global::MIN_POOL_LENGTH)
    {
        palette.setColor(QPalette::Text, Qt::red);
    }
    else
    {
        palette.setColor(QPalette::Text, Qt::black);
    }

    ui->poolSizeLineEdit->setPalette(palette);
    ui->poolSizeLineEdit->setText(strSize);
}

void MainWindow::closeEvent(QCloseEvent* event)
{
    // We'll just call @c onExitActionTriggered()
    // and leave without complaint.
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
    QJsonObject jsonObj;

    OptionsManager& optsManager = OptionsManager::instance();
    optsManager.writeToJSON(jsonObj);
    QJsonDocument jsonDoc(jsonObj);
    QByteArray json = jsonDoc.toJson(QJsonDocument::Compact);

    QSettings settings;
    settings.setValue("Options", json);
    settings.setValue("WindowGeometry", saveGeometry());
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
        QMessageBox msgBox(QMessageBox::Critical, tr("Password Beaver Error"), ex.message(),
                           nullptr, this);

        msgBox.setDetailedText(tr("Some required characters are not available because "
                                  "they have been manually excluded. Remove at least one of these "
                                  "characters from the \"Exclude these characters\" editor or use "
                                  "the check box to turn off this feature"));
        msgBox.addButton(QMessageBox::Ok);
        msgBox.exec();

        return;
    }

    catch(SmallCharacterPoolException& ex)
    {
        QMessageBox msgBox(QMessageBox::Critical, tr("Password Beaver Error"), ex.message(),
                           nullptr, this);

        msgBox.setDetailedText(tr("The character pool from which the password is derived "
                                  "is too small. Add more character classes or remove some "
                                  "from the exclusion list."));
        msgBox.addButton(QMessageBox::Ok);
        msgBox.exec();

        return;
    }

    OptionsManager& optsMan = OptionsManager::instance();
    QClipboard* clip = QGuiApplication::clipboard();
    if (optsMan.copyToClipboard(optsMan.currentKey()))
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
    OptionsManager::instance().setUsePunctuation(state);
    setPoolSizeLineEditText();
}

void MainWindow::onSymbolsCheckboxStateChanged(int state)
{
    OptionsManager::instance().setUseSymbols(state);
    setPoolSizeLineEditText();
}

void MainWindow::onDigitsCheckboxStateChanged(int state)
{
    OptionsManager::instance().setUseDigits(state);
    setPoolSizeLineEditText();
}

void MainWindow::onUpperCaseCheckboxStateChanged(int state)
{
    OptionsManager::instance().setUseUpperAlpha(state);
    setPoolSizeLineEditText();
}

void MainWindow::onLowerCaseCheckboxStateChanged(int state)
{
    OptionsManager::instance().setUseLowerAlpha(state);
    setPoolSizeLineEditText();
}

void MainWindow::onCopyToClipboardCheckboxStateChanged(int state)
{
    OptionsManager::instance().setCopyToClipboard(state);
}

void MainWindow::onExtendedAsciiCheckboxStateChanged(int state)
{
    OptionsManager::instance().setUseExtendedAscii(state);
    setPoolSizeLineEditText();
}

void MainWindow::onPasswordLengthSpinBoxValueChanged(int length)
{
    OptionsManager::instance().setPasswordLength(length);
}

void MainWindow::onExcludeCharsLineEditTextChanged()
{
    OptionsManager::instance().setCharsToExclude(ui->excludeCharsLineEdit->text());
    setPoolSizeLineEditText();
}

void MainWindow::onOptionsNameComboBoxEditTextChanged(const QString &text)
{
    // This just responds to text edits. If the current text:
    //  - is empty, both buttons are disabled.
    //  - is not a key/name or is "Default", Delete button is disabled but Save is active.
    //  - is a key/name other than "Default", Delete and Save buttons are enabled.


    if (text.isEmpty())
    {
        ui->saveOptionsPushButton->setEnabled(false);
        ui->deleteOptionsPushButton->setEnabled(false);
    }
    else if (text == "Default")
    {
        ui->saveOptionsPushButton->setEnabled(false);
        ui->deleteOptionsPushButton->setEnabled(true);
    }
    else if (OptionsManager::instance().contains(text))
    {
        ui->saveOptionsPushButton->setEnabled(true);
        ui->deleteOptionsPushButton->setEnabled(true);
    }
    else
    {
        ui->saveOptionsPushButton->setEnabled(true);
        ui->deleteOptionsPushButton->setEnabled(false);
    }
}

void MainWindow::onSaveOptionsPushButtonClicked(int state)
{

}

void MainWindow::onDeleteOptionsPushButtonClicked(int)
{
    // Check to see that this is not the default options set.
//    OptionsManager& optMan = OptionsManager::instance();
//    if (optMan.)

}

void MainWindow::onExcludeCharsLineEditEditingFinished()
{
    OptionsManager::instance().setCharsToExclude(ui->excludeCharsLineEdit->text());
    setPoolSizeLineEditText();
}

