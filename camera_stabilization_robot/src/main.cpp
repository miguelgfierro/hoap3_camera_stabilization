#include <QtGui/QApplication>
#include <stdio.h>
#include "sock.h"
#include "maingui.h"

int main(int argc, char *argv[])
{
    printf("main");
    /* initial setting of Socket */
    InitSocket(argv[1]);
    /* initial setting of signal */
    InitSignal();

    QApplication a(argc, argv);
    MainGui w;
    w.start_timer();
    //w.show();
    printf("show");
    int ret = a.exec();

    /* close of Socket */
    EndSignal(0);
    //printf("Close");
    return ret;

}

