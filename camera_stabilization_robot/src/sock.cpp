/* Hoap-3 Video Capture Client Program            */
/*                                                */
/*   sock.c  2004.12.13  takaishi                 */
/*                                                */
/* Copyright(C) FUJITU AUTOMATION LIMITED  2004   */

//#include <QtGlobal>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <signal.h>
#include <unistd.h>

#include "vget.h"
#include "command.h"

#define DB(x)


int Soc =0;


int InitSocket(char *hostname)
{
        struct servent *se;
        struct hostent *servhost;
        struct sockaddr_in server;

        DB(fprintf(stderr,"InitSocket(%s)\n",hostname));


        /* get of service infomation */
        if((se=getservbyname("hoapserver","tcp"))==NULL){
                perror("getservbyname");
                return(-1);
        }

        /* get of host infomation */
        if((servhost=gethostbyname(hostname))==NULL){
                perror("gethostbyname");
                return(-1);
        }

        /* make Socket */
        if((Soc=socket(AF_INET,SOCK_STREAM,0)) < 0){
                perror("socket");
                return(-1);
        }
        DB(fprintf(stderr,"socket:%d\n",Soc));

        /* connect of Socket */
        memset((char *)&server,0,sizeof(server));
        server.sin_family=AF_INET;
        server.sin_port=se->s_port;
        memcpy((char*)&server.sin_addr,servhost->h_addr,servhost->h_length);
        if(connect(Soc,(struct sockaddr*)&server,sizeof(server))==-1){
                perror("connect");
                return(-1);
        }

        //char* yuvbuf;

        //RecvFrame(yuvbuf,0);


        return(0);
}

int CloseSocket()
{

        DB(fprintf(stderr,"CloseSocket()\n"));
        if(Soc>0){
                /* close Socket */
                close(Soc);
        }
        Soc=0;

        return(0);
}

void EndSignal(int sig)
{

        EndSoc();
        CloseSocket();
        vget_end();
        exit(0);
}

int InitSignal()
{
        DB(fprintf(stderr,"InitSignal()\n"));
        /* initial signal */
        signal(SIGINT,EndSignal);
        signal(SIGQUIT,EndSignal);
        signal(SIGTERM,EndSignal);

        return(0);
}
