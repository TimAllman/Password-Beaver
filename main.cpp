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

#include "MainWindow.h"

#include <QApplication>
#include <QTranslator>

//#include <QStyleFactory>
#include <QDebug>

int main(int argc, char *argv[])
{
/* Generally, keeping commented code is a bad idea but we
 * keep this here to help with porting.
*/

//    QStringList keys = QStyleFactory::keys();
//    qDebug() << keys;
//    QStyle* style = QStyleFactory::create("fusion");
//    if (style)
//        QApplication::setStyle(style);

    QApplication application(argc, argv);

    QLocale::setDefault(QLocale::French);

    QTranslator translator;
    QLocale locale;
    if (locale.language() == QLocale::French)
    {
        if (translator.load("pwbeaver_fr.qm", ":/locale") == false)
            qDebug() << "Could not load translation file.";
        application.installTranslator(&translator);
    }

    MainWindow w;
    w.show();

    return application.exec();
}
