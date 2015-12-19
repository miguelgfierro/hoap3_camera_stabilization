/* Hoap-3 Video Capture Client Program            */
/*                                                */
/*   sock.h  2004.12.13  takaishi                 */
/*                                                */
/* Copyright(C) FUJITU AUTOMATION LIMITED  2004   */

#ifndef SOCK_H
#define SOCK_H


/* initial setting of socket */
int InitSocket(char *hostname);

/* close of socket */
int CloseSocket();

/* End Signal */
void EndSignal(int sig);

/* initial setting of signal */
int InitSignal();


#endif // SOCK_H
