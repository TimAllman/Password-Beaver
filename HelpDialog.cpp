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
#include "HelpDialog.h"
#include "ui_helpdialog.h"

#include <QFile>
#include <QTextStream>

HelpDialog::HelpDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::HelpDialog)
{
    ui->setupUi(this);

    setSizeGripEnabled(true);

    QFile manFile(":/manual/Manual.html");
    if (!manFile.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QTextStream inStream(&manFile);
    QString text = inStream.read(100000l);

    ui->textBrowser->setHtml(text);

}

HelpDialog::~HelpDialog()
{
    delete ui;
}

void HelpDialog::setHtmlText(const QString& /*helpText*/)
{

}
