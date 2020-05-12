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

#include "AboutDialog.h"
#include "Global.h"
#include "Exceptions.h"
#include "ui_aboutdialog.h"

#include <QUrl>
#include <QDesktopServices>
#include <QMessageBox>

AboutDialog::AboutDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutDialog)
{
    ui->setupUi(this);
}

/**
 * Unremarkable destructor.
 */
AboutDialog::~AboutDialog()
{
    delete ui;
}

void AboutDialog::onBeaverPushButtonClicked()
{
    QUrl url(Global::APPLICATION_URL, QUrl::StrictMode);
    bool success = QDesktopServices::openUrl(url);

    if (!success)
    {
        QString msg("Unable to open URL: ");
        msg += url.toString() + ".";

        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setText(msg);
        msgBox.exec();
    }
}
