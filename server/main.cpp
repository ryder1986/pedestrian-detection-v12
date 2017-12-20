#include <QCoreApplication>
#include "tools.h"
#include "camera.h"
#include "server.h"


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);


    prt(info,"main start ");

  // fflush(NULL);


#if 1

    ServerInfoReporter *reportor=new ServerInfoReporter();


    CameraManager &mgr=CameraManager::GetInstance();
    reportor->start();
    Server s;
    s.print_server_info();
    //  this_thread::sleep_for(chrono::seconds(30));
    //   delete mgr;
#else
    VideoSrc src("test.264");
    VideoHandler han;
    Mat *mt;
    while(1)
    {
        mt=src.get_frame();
        han.set_frame(mt);
        han.work();
        this_thread::sleep_for(chrono::milliseconds(10));
    }
#endif
    return a.exec();
}
