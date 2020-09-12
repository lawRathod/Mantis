#include "mainwindow.h"

#include <QApplication>
#include <QPalette>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    a.setStyle("Fusion");

    MainWindow w;
    w.show();
    return a.exec();
}
