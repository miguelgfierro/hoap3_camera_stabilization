/* Hoap-3 Video Capture Client Program            */
/*                                                */
/*   command.h  2004.12.20  takaishi                 */
/*                                                */
/* Copyright(C) FUJITU AUTOMATION LIMITED  2004   */

#ifndef COMMAND_H
#define COMMAND_H

#include <QtGlobal>

#define BUF_SIZE (512)

/* Send endsignal to server */
int EndSoc();

/* Recv CaptureFrame from int CamNo */
int RecvFrame(char* vbuf, int id_cam);

#define V4L_WIDTH  320
#define V4L_HEIGHT 240

#endif // COMMAND_H
