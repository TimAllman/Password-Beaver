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

#include "Global.h"
#include "MainWindow.h"
#include "ui_mainwindow.h"
#include "PasswordGenerator.h"
#include "OptionsManager.h"
#include "AboutDialog.h"
#include "Exceptions.h"
#include "OptionsManager.h"

#include <QClipboard>
#include <QIcon>
#include <QMessageBox>
#include <QCloseEvent>
#include <QJsonDocument>
#include <QSettings>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setFixedSize(width(),height());

    // This should happen first so that the proper path to the
    // configuration file is set properly.
    QCoreApplication::setOrganizationName(QString(Global::ORGANIZATION_NAME));
    QCoreApplication::setOrganizationDomain(QString(Global::ORGANIZATION_DOMAIN));
    QCoreApplication::setApplicationName(QString(Global::APPLICATION_NAME));

    // Get the OptionsManager instance. This has default options that are set on construction
    // and may have other option sets created from reading the settings.
    OptionsManager& optsMan = OptionsManager::instance();

    // load up the combobox with the names and set the active name in the editor.
    ui->optionsNameComboBox->addItems(optsMan.names());
    ui->optionsNameComboBox->setEditText(optsMan.activeKey());
    ui->optionsNameComboBox->setDuplicatesEnabled(false);
    ui->optionsNameComboBox->setInsertPolicy(QComboBox::InsertAlphabetically);

    // Make the initial settings to the UI controls before connecting the
    // signals and slots. Otherwise the values may change because of
    // slots being called before we want them
    ui->punctuationCheckBox->setCheckState(static_cast<Qt::CheckState>(optsMan.usePunctuation()));
    ui->symbolsCheckBox->setCheckState(static_cast<Qt::CheckState>(optsMan.useSymbols()));
    ui->digitsCheckBox->setCheckState(static_cast<Qt::CheckState>(optsMan.useDigits()));
    ui->upperCaseCheckBox->setCheckState(static_cast<Qt::CheckState>(optsMan.useUpperAlpha()));
    ui->lowerCaseCheckBox->setCheckState(static_cast<Qt::CheckState>(optsMan.useLowerAlpha()));
    ui->copyToClipboardCheckBox->setChecked(optsMan.copyToClipboard());
    ui->unicodeCheckBox->setChecked(optsMan.useUnicode());

    ui->passwordLengthSpinBox->setRange(Global::MIN_PW_LENGTH, Global::MAX_PW_LENGTH);
    int pwlen = static_cast<int>(optsMan.passwordLength());
    if (pwlen >= Global::MIN_PW_LENGTH && pwlen <= Global::MAX_PW_LENGTH)
        ui->passwordLengthSpinBox->setValue(pwlen);
    else
    {
        ui->passwordLengthSpinBox->setValue(Global::DEFAULT_PW_LENGTH);
        optsMan.setPasswordLength(Global::DEFAULT_PW_LENGTH);
    }

    ui->excludeCharsLineEdit->setText(optsMan.charsToExclude());

    // Now do the connections.
    ui->aboutQtAction->setMenuRole(QAction::AboutQtRole);
    connect(ui->aboutQtAction, &QAction::triggered, qApp, &QApplication::aboutQt);

    ui->aboutAction->setIcon(QIcon::fromTheme("help-about"));
    ui->aboutAction->setMenuRole(QAction::AboutRole); // for macos only
    connect(ui->aboutAction, &QAction::triggered, this, &MainWindow::onAboutActionTriggered);

    ui->exitAction->setShortcut(QKeySequence::Quit);
    ui->exitAction->setIcon(QIcon::fromTheme("application-exit"));
    connect(ui->exitAction, &QAction::triggered, this, &QWidget::close);

    ui->helpAction->setIcon(QIcon::fromTheme("help-contents"));

    connect(ui->quitPushButton, &QPushButton::clicked, this, &QWidget::close);
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
    connect(ui->unicodeCheckBox, SIGNAL(stateChanged(int)),
            this, SLOT(onUnicodeCheckboxStateChanged(int)));

    connect(ui->excludeCharsLineEdit, SIGNAL(editingFinished()),
            this, SLOT(onExcludeCharsLineEditEditingFinished()));
    connect(ui->excludeCharsLineEdit, SIGNAL(textChanged(const QString&)),
            this, SLOT(onExcludeCharsLineEditTextChanged()));

    connect(ui->passwordLengthSpinBox, SIGNAL(valueChanged(int)),
            this, SLOT(onPasswordLengthSpinBoxValueChanged(int)));

    connect(ui->optionsNameComboBox, SIGNAL(editTextChanged(const QString&)),
            this, SLOT(onOptionsNameComboBoxEditTextChanged(const QString&)));
    connect(ui->optionsNameComboBox, SIGNAL(currentIndexChanged(int)),
            this, SLOT(onOptionsNameComboBoxCurrentIndexChanged(int)));

    connect(ui->saveOptionsPushButton, SIGNAL(clicked(bool)),
            this, SLOT(onSaveOptionsPushButtonClicked(bool)));

    connect(ui->deleteOptionsPushButton, SIGNAL(clicked(bool)),
            this, SLOT(onDeleteOptionsPushButtonClicked(bool)));


    setPoolSizeLineEditText();
    updateGui();

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
    OptionsManager& optsMan = OptionsManager::instance();

    CharacterPool pool(optsMan.useUnicode(), optsMan.charsToExclude(), optsMan.usePunctuation(),
                       optsMan.useDigits(), optsMan.useUpperAlpha(), optsMan.useLowerAlpha(),
                       optsMan.useSymbols());

    // Here we inform the user of the pool size and show it
    // in @c Qt::red if it is too small.
    QString strSize = QString::number(pool.poolSize());
    QPalette palette;

    if (pool.poolSize() < Global::MIN_POOL_LENGTH)
        palette.setColor(QPalette::Text, Qt::red);
    else
        palette.setColor(QPalette::Text, Qt::black);

    ui->poolSizeLabel->setPalette(palette);
    ui->poolSizeLabel->setText(strSize);
}

void MainWindow::closeEvent(QCloseEvent* event)
{
    QJsonObject jsonObj;

    OptionsManager& optsMan = OptionsManager::instance();
    optsMan.writeToJSON(jsonObj);
    QJsonDocument jsonDoc(jsonObj);
    QByteArray json = jsonDoc.toJson(QJsonDocument::Indented);

    QSettings settings;
    settings.setValue("Options", json);
    settings.setValue("WindowGeometry", saveGeometry());

    event->accept();
}

void MainWindow::displayCurrentOptions()
{
    OptionsManager& optsMan = OptionsManager::instance();

    // Make the initial settings to the UI controls before connecting the
    // signals and slots. Otherwise the values may change because of
    // slots being called before we want them
    ui->punctuationCheckBox->setCheckState(static_cast<Qt::CheckState>(optsMan.usePunctuation()));
    ui->symbolsCheckBox->setCheckState(static_cast<Qt::CheckState>(optsMan.useSymbols()));
    ui->digitsCheckBox->setCheckState(static_cast<Qt::CheckState>(optsMan.useDigits()));
    ui->upperCaseCheckBox->setCheckState(static_cast<Qt::CheckState>(optsMan.useUpperAlpha()));
    ui->lowerCaseCheckBox->setCheckState(static_cast<Qt::CheckState>(optsMan.useLowerAlpha()));
    ui->copyToClipboardCheckBox->setChecked(optsMan.copyToClipboard());
    ui->unicodeCheckBox->setChecked(optsMan.useUnicode());

    ui->passwordLengthSpinBox->setRange(Global::MIN_PW_LENGTH, Global::MAX_PW_LENGTH);
    int pwlen = static_cast<int>(optsMan.passwordLength());
    if (pwlen >= Global::MIN_PW_LENGTH && pwlen <= Global::MAX_PW_LENGTH)
        ui->passwordLengthSpinBox->setValue(pwlen);
    else
    {
        ui->passwordLengthSpinBox->setValue(Global::DEFAULT_PW_LENGTH);
        optsMan.setPasswordLength(Global::DEFAULT_PW_LENGTH);
    }

    ui->excludeCharsLineEdit->setText(optsMan.charsToExclude());
}

void MainWindow::updateGui()
{
    // This just responds to text edits. If the current text:
    //  - is empty, both buttons are disabled.
    //  - is not a key/name or is "Default", Delete button is disabled but Save is active.
    //  - is a key/name other than "Default", Delete and Save buttons are enabled.
    QString curText = ui->optionsNameComboBox->currentText();
    OptionsManager& optsMan = OptionsManager::instance();

    if (curText.isEmpty() || curText == "Default")
    {
        ui->saveOptionsPushButton->setEnabled(false);
        ui->deleteOptionsPushButton->setEnabled(false);
        ui->indicatorWidget->setActive(false);
    }
    else if (optsMan.contains(curText))
    {
        if (optsMan.isModified(curText))
        {
            ui->saveOptionsPushButton->setEnabled(true);
            ui->deleteOptionsPushButton->setEnabled(true);
            ui->indicatorWidget->setActive(true);
        }
        else
        {
            ui->saveOptionsPushButton->setEnabled(false);
            ui->deleteOptionsPushButton->setEnabled(true);
            ui->indicatorWidget->setActive(false);
        }
    }
    else
    {
        // something new has been typed in.
        ui->saveOptionsPushButton->setEnabled(true);
        ui->deleteOptionsPushButton->setEnabled(false);
        ui->indicatorWidget->setActive(true);
    }
}

void MainWindow::onAboutActionTriggered()
{
    AboutDialog dlg;
    dlg.exec();
}

void MainWindow::onGeneratePushButtonClicked()
{
    PasswordGenerator gen;
    QString password;

    try
    {
        password = gen.password();
    }

    catch(ExclusionException& ex)
    {
        QMessageBox msgBox(this);
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.setText(tr("Password Beaver Error"));
        msgBox.setInformativeText(ex.message());
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
        QMessageBox msgBox(this);
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.setText(tr("Password Beaver Error"));
        msgBox.setInformativeText(ex.message());
        msgBox.setDetailedText(tr("The character pool from which the password is derived "
                                  "is too small. Add more character classes or remove some "
                                  "from the exclusion list."));
        msgBox.addButton(QMessageBox::Ok);
        msgBox.exec();

        return;
    }

    OptionsManager& optsMan = OptionsManager::instance();
    QClipboard* clip = QGuiApplication::clipboard();
    if (optsMan.copyToClipboard())
        clip->setText(password);

    ui->passwordLineEdit->setText(password);
    QString entrStr = QString::number(gen.entropy(), 'f', 1) + " bits";
    ui->entropyLabel->setText(entrStr);
}

void MainWindow::onHelpActionTriggered()
{

}

void MainWindow::onPunctuationCheckboxStateChanged(int state)
{
    OptionsManager::instance().setUsePunctuation(state);
    setPoolSizeLineEditText();
    updateGui();
}

void MainWindow::onSymbolsCheckboxStateChanged(int state)
{
    OptionsManager::instance().setUseSymbols(state);
    setPoolSizeLineEditText();
    updateGui();
}

void MainWindow::onDigitsCheckboxStateChanged(int state)
{
    OptionsManager::instance().setUseDigits(state);
    setPoolSizeLineEditText();
    updateGui();
}

void MainWindow::onUpperCaseCheckboxStateChanged(int state)
{
    OptionsManager::instance().setUseUpperAlpha(state);
    setPoolSizeLineEditText();
    updateGui();
}

void MainWindow::onLowerCaseCheckboxStateChanged(int state)
{
    OptionsManager::instance().setUseLowerAlpha(state);
    setPoolSizeLineEditText();
    updateGui();
}

void MainWindow::onCopyToClipboardCheckboxStateChanged(int state)
{
    OptionsManager::instance().setCopyToClipboard(state);
    updateGui();
}

void MainWindow::onUnicodeCheckboxStateChanged(int state)
{
    OptionsManager::instance().setUseUnicode(state);
    setPoolSizeLineEditText();
    updateGui();
}

void MainWindow::onPasswordLengthSpinBoxValueChanged(int length)
{
    OptionsManager::instance().setPasswordLength(length);
    updateGui();
}

void MainWindow::onExcludeCharsLineEditTextChanged()
{
    OptionsManager::instance().setCharsToExclude(ui->excludeCharsLineEdit->text());
    setPoolSizeLineEditText();
    updateGui();
}

void MainWindow::onOptionsNameComboBoxEditingFinished()
{
    qDebug() << "optionsNameComboBoxEditingFinished: " <<
                ui->optionsNameComboBox->lineEdit()->text();
}

void MainWindow::onOptionsNameComboBoxEditTextChanged(const QString &text)
{
    // This just responds to text edits. If the current text:
    //  - is empty, both buttons are disabled.
    //  - is not a key/name or is "Default", Delete button is disabled but Save is active.
    //  - is a key/name other than "Default", Delete and Save buttons are enabled.

    qDebug() << "onOptionsNameComboBoxEditTextChanged: " << text;
    updateGui();
}

void MainWindow::onOptionsNameComboBoxCurrentIndexChanged(int index)
{
    qDebug() << "onOptionsNameComboBoxCurrentIndexChanged: " << index;
    QString name = ui->optionsNameComboBox->itemText(index);
    OptionsManager::instance().setActive(name);
    displayCurrentOptions();
    updateGui();
}

void MainWindow::onSaveOptionsPushButtonClicked(bool)
{
    // get current opt set, set name to edit text, add to map.
    QString newName = ui->optionsNameComboBox->currentText();
    OptionsManager& optMan = OptionsManager::instance();
    optMan.saveOptions(newName);

    // We just update the combobox list in bulk
    ui->optionsNameComboBox->clear();
    ui->optionsNameComboBox->addItems(optMan.names());
    int idx = ui->optionsNameComboBox->findText(newName);
    ui->optionsNameComboBox->setCurrentIndex(idx);
}

void MainWindow::onDeleteOptionsPushButtonClicked(bool)
{
    // Check to see that this is not the default options set.
    QString curName = ui->optionsNameComboBox->currentText();
    if (curName == OptionsManager::STR_DEFAULT)
        return;

    OptionsManager& optMan = OptionsManager::instance();
    if (optMan.contains(ui->optionsNameComboBox->currentText()))
    {
        // remove it from the combobox
        int curIdx = ui->optionsNameComboBox->currentIndex();
        QString curText = ui->optionsNameComboBox->currentText();
        ui->optionsNameComboBox->removeItem(curIdx);

        optMan.deleteOptions(curText);
        displayCurrentOptions();
        updateGui();
    }
}

void MainWindow::onExcludeCharsLineEditEditingFinished()
{
    OptionsManager::instance().setCharsToExclude(ui->excludeCharsLineEdit->text());
    setPoolSizeLineEditText();
}

