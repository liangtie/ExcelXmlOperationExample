#include "Widget.h"

#include <QApplication>

#include <QtAxExcelEngine.h>
#include <QtCore/QCoreApplication>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;
    w.show();

    return a.exec();
}
