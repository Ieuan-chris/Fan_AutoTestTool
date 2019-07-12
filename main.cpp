#include "mainwindow.h"
#include <QApplication>

#include "testitem.h"
#include "teststartup.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
//    MainWindow w;
//    w.show();
    TestItem *test = new TestStartup("COM6");
    test->readyGo();

    return a.exec();
}
