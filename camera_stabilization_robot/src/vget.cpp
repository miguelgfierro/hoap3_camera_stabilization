/* Hoap-3 Video Capture Client Program            */
/*                                                */
/*   vget.c  2004.12.17  takaishi                 */
/*                                                */
/* Copyright(C) FUJITU AUTOMATION LIMITED  2004   */
/* Hoap-3 Video Capture Client Program            */
/*                                                */
/*   vget.cpp  2010.09.14  Hernandez Garcia                 */
/* Copyright(C) UNIVERSIDAD CARLOS III DE MADRID  2010   */


#include <QtGlobal>
#include <QImage>
#include <QPixmap>
#include <cstdlib>
#include <fcntl.h>
#include <stdio.h>
#include <time.h>
#include <sys/timeb.h>

#include "vget.h"
#include "command.h"

#include <cv.h>
#include <highgui.h>

#include "conversion_QI_IPL.h"
#include "image_stabilization.h"

#define DB(x)

char* yuvbuf;
char* rgbbuf;
int framecnt=0;

void vget_init(){
        yuvbuf = (char*)malloc(sizeof(char) * V4L_WIDTH * V4L_HEIGHT *3/2); //yuvbuf = new uchar[sizeof(uchar) *V4L_WIDTH * V4L_HEIGHT *3/2]; //
        rgbbuf = (char*)malloc(sizeof(char) * V4L_WIDTH * V4L_HEIGHT *3); //rgbbuf = new uchar[sizeof(uchar) *V4L_WIDTH * V4L_HEIGHT *3]; //
        //printf(":: %d ::\n", sizeof(uchar) *V4L_WIDTH * V4L_HEIGHT *3/2);
}

void vget_end(){
         free(rgbbuf); //delete yuvbuf;
         free(yuvbuf); //delete rgbbuf;
        //DB(fprintf(stderr,"Malloc Free\n"));
}

///* 							*/
///*  next frame capture		*/
///*							*/
//GdkImlibImage* get_next_frame(GdkImlibImage* current,gint fid){
//	GdkImlibImage* ret;
//
//	DB(time_check(100));
//
//	/* get frame */
//
//	if (fid == 0){
//		RecvFrame(yuvbuf,0);
//	}else{
//		RecvFrame(yuvbuf,1);
//	}
//
//	/* RGB convert from YUV240P */
//	yuv240p_rgb(yuvbuf,rgbbuf);
//
//
//
//	/* Return New Frame */
//	if (current) gdk_imlib_kill_image(current);
//	ret =  gdk_imlib_create_image_from_data(rgbbuf, NULL,V4L_WIDTH, V4L_HEIGHT);
//
//	return ret;
//}
//QImage get_next_frame(int fid){
//    //QImage *ret;
//
//
//    DB(time_check(100));
//
//        /* get frame */
//
//
//        if (fid == 0){
//            //printf("get 0");
//                RecvFrame(yuvbuf,0);
//        }else{
//            //printf("get 1");
//                RecvFrame(yuvbuf,1);
//        }
//
//        /* RGB convert from YUV240P */
//        yuv240p_rgb((uchar*)yuvbuf,(uchar*)rgbbuf);
//        //printf("YUV2RGB");
//
//        QImage im((uchar*)rgbbuf, V4L_WIDTH, V4L_HEIGHT, QImage::Format_RGB888);
//
//        //im.save("test_qtimage.png");
//        cvNamedWindow("getFrame");
//        IplImage* myIplImage = cvCreateImageHeader(cvSize(V4L_WIDTH, V4L_HEIGHT), 8, 3);
//        cvSetData(myIplImage, (uchar*)rgbbuf, V4L_WIDTH*3);
//        cvShowImage("getFrame",myIplImage);
//
//        return im;
//}

IplImage* get_next_frame(int fid){

    DB(time_check(100));

        /* get frame */
        if (fid == 0){
                RecvFrame(yuvbuf,0);
        }else{
                RecvFrame(yuvbuf,1);
        }

        /* RGB convert from YUV240P */
        yuv240p_rgb((uchar*)yuvbuf,(uchar*)rgbbuf);
        //printf("YUV2RGB");

        //cvNamedWindow("getFrame");
        IplImage* mImage = cvCreateImageHeader(cvSize(V4L_WIDTH, V4L_HEIGHT), 8, 3);
        cvSetData(mImage, (uchar*)rgbbuf, V4L_WIDTH*3);
        //cvShowImage("getFrame",mImage);

        return mImage;
}



QImage next_frame(int fid){
    QImage frame;
    QImage* nextFrame;
    IplImage* iplFrame;


    iplFrame = get_next_frame(fid); //frame = get_next_frame(fid);  //CAPTURE FRAME FROM HOAP

    //iplFrame = QImage2IplImage(&frame);
    printf("\nQIMAGE next frame to IPLIMAGE\n");
    //iplFrame = imagestabilization(iplFrame);
    printf("\nESTABILIZACION DE LA IPLIMAGE\n");
    nextFrame = IplImage2QImage(iplFrame);
    printf("\nIPLIMAGE next frame to QIMAGE\n");

    return *nextFrame;
}

QImage next_frame(int fid, QImage oldFrame){
    QImage* nextFrame;
    IplImage* iplFrame;
    IplImage* iplOldFrame;

    iplFrame = get_next_frame(fid); //frame = get_next_frame(fid);  //CAPTURE FRAME FROM HOAP
//printf("\n11111111111111\n");
    iplOldFrame = QImage2IplImage(&oldFrame);
//printf("\n222222222222222\n");
    iplFrame = imagestabilization(iplFrame, iplOldFrame);  //Stabilize Frame
//printf("\n333333333333333\n");
    nextFrame = IplImage2QImage(iplFrame); //convert to QImage

//printf("\n444444444444444\n");
    return *nextFrame;
}


/*
*   YUV240P => RGB
*/
void yuv240p_rgb(uchar* yuvbuf,uchar* rgbbuf){
        int i;
        int j;

        int Y;
        int U;
        int V;
        int R;
        int G;
        int B;
        uint c_num;
        uint u_ofs;
        uint v_ofs;
        uint y_num;
        uint u_num;
        uint v_num;
        uint uv_cntup_w;
        uint uv_cntup_h;

        u_ofs=V4L_WIDTH*V4L_HEIGHT;
        v_ofs=(V4L_WIDTH*V4L_HEIGHT)+((V4L_WIDTH*V4L_HEIGHT)>>2);
        y_num=0;
        u_num=u_ofs;
        v_num=v_ofs;
        c_num=0;
        uv_cntup_w=0;
        uv_cntup_h=0;
        for (j=0; j< V4L_HEIGHT; j++){
                if(uv_cntup_h==0){
                        uv_cntup_h=1;
                }else{
                        u_num = u_num - (V4L_WIDTH>>1);
                        v_num = v_num - (V4L_WIDTH>>1);
                        uv_cntup_h=0;
                }
                for (i=0; i < V4L_WIDTH;i++){
                        Y = (int)yuvbuf[y_num];
                        U = (int)yuvbuf[u_num];
                        V = (int)yuvbuf[v_num];

                        /* YUV to RGB Convert */
                        R=1.402*(V-128)+Y;
                        G=-0.344*(U-128)-0.714*(V-128)+Y;
                        B=1.722*(U-128)+Y;

                        //B = 1.164*(Y - 16) + 2.018*(U - 128);
                        //G = 1.164*(Y - 16) - 0.813*(V - 128) - 0.391*(U - 128);
                        //R = 1.164*(Y - 16) + 1.596*(V - 128);

                        /* Limit Check*/
                        if(R < 0) R=0;
                        if(R > 255) R=255;
                        if(G < 0) G=0;
                        if(G > 255) G=255;
                        if(B < 0) B=0;
                        if(B > 255) B=255;

                        rgbbuf[c_num]=(uchar)B;
                        rgbbuf[c_num+1]=(uchar)G;
                        rgbbuf[c_num+2]=(uchar)R;

                        c_num=c_num+3;
                        y_num++;
                        if(uv_cntup_w==0){
                                uv_cntup_w=1;
                        }else{
                                u_num=u_num+1;
                                v_num=v_num+1;
                                uv_cntup_w=0;
                        }
                }
        }
}

void time_check(int framenum){

        /* frame rate counter */
        static struct timeb start;
        static struct timeb finish;
        static int framecnt;
        double duration;
        double frame_rate;
        double bps;
        if(framecnt==0){
                ftime(&start);
                framecnt++;
        }else if(framecnt==framenum){
                ftime(&finish);
                duration = (difftime(finish.time,start.time)*1000)+((long)finish.millitm - (long)start.millitm);
                frame_rate=((framecnt/2)/duration)*1000;
                bps=(V4L_WIDTH*V4L_HEIGHT*12*framecnt/duration)/1000;
                fprintf(stderr,"FRAME_RATE=%3.2f[frm/sec]\n",frame_rate);
                fprintf(stderr,"BPS=%3.2f[Mbps]\n",bps);
                framecnt=0;
        }else{
                framecnt++;
        }
        return;
}
