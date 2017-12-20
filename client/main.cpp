#include "mainwindow.h"
#include <QApplication>
//#include "client.h"
#include <mutex>
int log_level=3;// 1.no log  2.print log 3 print and write log 4.write without print log
#include "mainwidget.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    prt(info,"main start");

  //  MainWindow w;//make sure that no delay in constraction
    //  w.setWindowFlags(Qt::WindowFullScreen);
    //  w.setWindowState(Qt::WindowFullScreen);



//    w.setWindowState(Qt::WindowMaximized);
 //   w.show();



    MainWidget w;
    w.show();
    return a.exec();
}
