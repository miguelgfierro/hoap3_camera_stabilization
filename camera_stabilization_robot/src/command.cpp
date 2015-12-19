/* Hoap-3 Video Capture Client Program            */
/*                                                */
/*   command.c  2004.12.28  takaishi              */
/*                                                */
/* Copyright(C) FUJITU AUTOMATION LIMITED  2004   */


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>

#include "vget.h"
#include "command.h"

#define DB(x)


extern int Soc;

int EndSoc(){
	char buf[10];
	int len;
	strcpy(buf,"end\n\0");
	DB(fprintf(stderr,"CMD:%s\n",buf));
	len=send(Soc,buf,strlen(buf),0);
	if(len<0){
		/* error */
		perror("send");
	}
        printf("ENDSOCENDSOC");
	return(0);
}

int RecvFrame(char* vbuf,int id_cam)
{
	char buf[BUF_SIZE];
	int len;

	DB(fprintf(stderr,"MainLoop()\n"));

	/* send getcap command */
	if(id_cam == 0){
		strcpy(buf,"getcap0\n\0");
	}else{
		strcpy(buf,"getcap1\n\0");
	}

	DB(fprintf(stderr,"CMD:%s\n",buf));
	len=send(Soc,buf,strlen(buf),0);
	if(len<0){
		/* error */
		perror("send");
	}


        /* capture recv start */
        int i=0;
        int j=0;
        int loop;
        div_t div_result;
        double frame_size=V4L_WIDTH*V4L_HEIGHT*3/2;

        /* set loop count */
        div_result=div(frame_size,BUF_SIZE);
        if(div_result.rem==0){
                loop=div_result.quot;
        }else{
                loop=div_result.quot+1;
        }

        DB(fprintf(stderr,"loop=%d\n",loop));
        while(i<loop){

                /* recv capture data */
                len=recv(Soc,buf,sizeof(buf),MSG_WAITALL);
                if(len<0){
                        /* error */
                        perror("recv");
                }
                DB(fprintf(stderr,"RECVOK len=%d,cnt=%d\n",len,i));


                //copy from buf to vbuf
                for(j=0;j<len;j++){
                        *vbuf=buf[j];
                 //       printf("%f(%d)::::%f\n", (float)buf[j], j, (float)*vbuf);
                        vbuf++;

                }
                //printf("\n\n::%d::%d::\n", j, i);
                i++;
        }
		
	return(0);
}

