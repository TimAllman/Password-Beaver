#include "MainWindow.h"
#include <QApplication>
#include <QStyleFactory>

int main(int argc, char *argv[])
{
    //    QStringList keys = QStyleFactory::keys();

    QStyle* style = QStyleFactory::create("fusion");
    if (style)
        QApplication::setStyle(style);

    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
