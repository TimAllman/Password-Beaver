/*
 * Copyright © 2023 Tim Allman
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
#include "HelpDialog.h"
#include "Exceptions.h"
#include "OptionsManager.h"

#include <QClipboard>
#include <QIcon>
#include <QMessageBox>
#include <QCloseEvent>
#include <QJsonDocument>
#include <QSettings>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), helpDlg(nullptr), ui(new Ui::MainWindow)
{
    QIcon windowIcon(":/icons/icns/beaver.icns");
    setWindowIcon(windowIcon);

    ui->setupUi(this);

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
    // slots being called before we want them to change.
    ui->punctuationCheckBox->setCheckState(static_cast<Qt::CheckState>(optsMan.usePunctuation()));
    ui->symbolsCheckBox->setCheckState(static_cast<Qt::CheckState>(optsMan.useSymbols()));
    ui->digitsCheckBox->setCheckState(static_cast<Qt::CheckState>(optsMan.useDigits()));
    ui->upperCaseCheckBox->setCheckState(static_cast<Qt::CheckState>(optsMan.useUpperAlpha()));
    ui->lowerCaseCheckBox->setCheckState(static_cast<Qt::CheckState>(optsMan.useLowerAlpha()));
    ui->copyToClipboardCheckBox->setChecked(optsMan.autoCopyToClipboard());
    ui->extendedAsciiCheckBox->setChecked(optsMan.useExtendedAscii());

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
    ui->actionAbout_Qt->setMenuRole(QAction::AboutQtRole);
    connect(ui->actionAbout_Qt, &QAction::triggered, qApp, &QApplication::aboutQt);

    //ui->actionAbout->setIcon(QIcon::fromTheme("help-about"));
    ui->actionAbout->setMenuRole(QAction::AboutRole); // for macos only
    connect(ui->actionAbout, &QAction::triggered, this, &MainWindow::onAboutActionTriggered);

    ui->actionQuit->setShortcut(QKeySequence::Quit);
    ui->actionQuit->setIcon(QIcon::fromTheme("application-exit"));
    connect(ui->actionQuit, &QAction::triggered, this, &QWidget::close);

    ui->actionManual->setIcon(QIcon::fromTheme("help-contents"));
    connect(ui->actionManual, &QAction::triggered, this, &MainWindow::onShowManualTriggered);

    connect(ui->quitPushButton, &QPushButton::clicked, this, &QWidget::close);
    connect(ui->generatePushButton, &QPushButton::clicked, this, &MainWindow::onGeneratePushButtonClicked);
    connect(ui->copyToClipboardPushButton, &QPushButton::clicked, this, &MainWindow::onCopyPushButtonClicked);

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
    connect(ui->excludeCharsLineEdit, SIGNAL(textChanged(QString)),
            this, SLOT(onExcludeCharsLineEditTextChanged()));

    connect(ui->passwordLengthSpinBox, SIGNAL(valueChanged(int)),
            this, SLOT(onPasswordLengthSpinBoxValueChanged(int)));

    connect(ui->optionsNameComboBox, SIGNAL(editTextChanged(QString)),
            this, SLOT(onOptionsNameComboBoxEditTextChanged()));
    connect(ui->optionsNameComboBox, SIGNAL(currentIndexChanged(int)),
            this, SLOT(onOptionsNameComboBoxCurrentIndexChanged(int)));

    connect(ui->saveOptionsPushButton, SIGNAL(clicked(bool)),
            this, SLOT(onSaveOptionsPushButtonClicked(bool)));

    connect(ui->deleteOptionsPushButton, SIGNAL(clicked(bool)),
            this, SLOT(onDeleteOptionsPushButtonClicked(bool)));

    connect(ui->copyToClipboardPushButton, SIGNAL(clicked(bool)),
            this, SLOT(onCopyPushButtonClicked()));

    setPoolSizeLineEditText();
    makePassword();
    updateGui();

    QSettings settings;
    QPoint pos = settings.value("WindowPosition").toPoint();
    if (pos != QPoint(0, 0))
        move(settings.value("WindowPosition").toPoint());
}

MainWindow::~MainWindow()
{
    delete helpDlg;
    helpDlg = nullptr;

    delete ui;
}

void MainWindow::setPoolSizeLineEditText()
{
    const OptionsManager& optsMan = OptionsManager::instance() const;

    CharacterPool pool(optsMan.useExtendedAscii(), optsMan.charsToExclude(), optsMan.usePunctuation(),
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

    const OptionsManager& const optsMan = OptionsManager::instance() const;
    optsMan.writeToJSON(jsonObj);
    QJsonDocument jsonDoc(jsonObj);
    QByteArray json = jsonDoc.toJson(QJsonDocument::Indented);

    QSettings settings;
    settings.setValue(QLatin1String("Options"), json);
    settings.setValue(QLatin1String("WindowGeometry"), saveGeometry());
    QPoint position = pos();
    settings.setValue(QLatin1String("WindowPosition"), position);
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
    ui->copyToClipboardCheckBox->setChecked(optsMan.autoCopyToClipboard());
    ui->extendedAsciiCheckBox->setChecked(optsMan.useExtendedAscii());

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
    // Set the text for the password and the entropy.
    if (mIsNewPassword)
    {
        ui->passwordLineEdit->setText(mPassword);
        ui->entropyLabel->setText(mEntropyStr);
        mIsNewPassword = false;
    }

    // This responds to text edits. If the current text:
    //  - is empty, both buttons are disabled.
    //  - is not a key/name or is "Default", Delete button is disabled but Save is active.
    //  - is a key/name other than "Default", Delete and Save buttons are enabled.
    // The state of the indicator widget always reflects the modified status.
    QString curText = ui->optionsNameComboBox->currentText();
    OptionsManager& optsMan = OptionsManager::instance();

    if (curText == "Default")
    {
        ui->saveOptionsPushButton->setEnabled(false);
        ui->deleteOptionsPushButton->setEnabled(false);
        ui->indicatorWidget->setActive(optsMan.isModified(curText));
    }
    else
    {
        if (optsMan.contains(curText))
        {
            if (optsMan.isModified(curText))
            {
                ui->saveOptionsPushButton->setEnabled(true);
                ui->indicatorWidget->setActive(true);
            }
            else
            {
                ui->saveOptionsPushButton->setEnabled(false);
                ui->indicatorWidget->setActive(false);
            }
            ui->deleteOptionsPushButton->setEnabled(true);
        }
        else
        {
            // something new has been typed in.
            ui->saveOptionsPushButton->setEnabled(true);
            ui->deleteOptionsPushButton->setEnabled(false);
            ui->indicatorWidget->setActive(true);
        }
    }
}

void MainWindow::copyToClipboard(const QString& password)
{
    QClipboard* clip = QGuiApplication::clipboard();
    clip->setText(password);
}

void MainWindow::makePassword()
{
    PasswordGenerator gen;

    try
    {
        mPassword = gen.password();
        mEntropyStr = QString::number(gen.entropy(), 'f', 1) + " bits";
    }

    catch(ExclusionException& ex)
    {
        QMessageBox msgBox(this);
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.setText(tr("Password Beaver Error"));
        msgBox.setInformativeText(ex.message());
        msgBox.setDetailedText(tr("Some required characters are not available because "
                                  "they have been manually excluded. Remove at least one of these "
                                  "characters from the \"Exclude\" editor or use "
                                  "the check box to turn off this feature."));
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
        msgBox.setDetailedText(tr("The character pool from which the password is "
                                  "derived is too small. Add more character classes "
                                  "or remove some from the exclusion list."));
        msgBox.addButton(QMessageBox::Ok);
        msgBox.exec();

        return;
    }

    mIsNewPassword = true;
    if (OptionsManager::instance().autoCopyToClipboard())
        copyToClipboard(mPassword);
}

void MainWindow::onAboutActionTriggered()
{
    AboutDialog dlg;
    dlg.exec();
}

void MainWindow::onGeneratePushButtonClicked()
{
    makePassword();
    updateGui();


    ui->passwordLineEdit->setText(mPassword);
    ui->entropyLabel->setText(mEntropyStr);
}

void MainWindow::onCopyPushButtonClicked()
{
    copyToClipboard(mPassword);
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

void MainWindow::onExtendedAsciiCheckboxStateChanged(int state)
{
    OptionsManager::instance().setUseExtendedAscii(state);
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

void MainWindow::onOptionsNameComboBoxEditTextChanged()
{
    // This just responds to text edits. The real work is done by updateGui().
    updateGui();
}

void MainWindow::onOptionsNameComboBoxCurrentIndexChanged(int index)
{
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
    if (curName == OptionsManager::STR_DEFAULT_NAME)
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

void MainWindow::onShowManualTriggered()
{
    if (!helpDlg)
        helpDlg = new HelpDialog();

    helpDlg->show();
    helpDlg->raise();
}

void MainWindow::clearClipboard()
{
    QClipboard* clip = QGuiApplication::clipboard();
    clip->clear();
}

void MainWindow::onExcludeCharsLineEditEditingFinished()
{
    OptionsManager::instance().setCharsToExclude(ui->excludeCharsLineEdit->text());
    setPoolSizeLineEditText();
}

