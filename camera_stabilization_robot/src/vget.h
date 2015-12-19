#ifndef VGET_H
#define VGET_H

/* Hoap-3 Video Capture Client Program            */
/*                                                */
/*   vget.h  2004.12.14  takaishi                 */
/*                                                */
/* Copyright(C) FUJITU AUTOMATION LIMITED  2004   */

#include <QtGlobal>
#include <QImage>
#include <cv.h>
#include <highgui.h>

#define V4L_WIDTH  320
#define V4L_HEIGHT 240


void vget_init();
void vget_end();

///* Next Fram Capture */
//GdkImlibImage* get_next_frame(GdkImlibImage*,gint);
//QImage get_next_frame(int fid);
QImage next_frame(int fid);
IplImage* get_next_frame(int fid);
QImage next_frame(int fid, QImage oldFrame);

/* YUV240P->RGB Convert */
void yuv240p_rgb(uchar* ,uchar*);

/* time check for debug */
void time_check(int);


#endif // VGET_H
