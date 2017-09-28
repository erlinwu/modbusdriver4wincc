#include "mainwindow.h"
#include <QApplication>
//添加自定义类引用
#include "xmlhelp.h"



int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    //
    //xmlhelp xmlhelp;

    return a.exec();
}
