//
// camera_stabilization:main.cpp
//
// Author: Daniel Hernández García <dhgarcia@ing.uc3m.es>, (C) 2010
// Author: Miguel González-Fierro <mgpalaci@ing.uc3m.es>, (C) 2010
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
//

#include "image_stabilization.h"
#include <cv.h>
#include <highgui.h>
#include <iostream>

using namespace cv;
using namespace std;

int main(int argc, char *argv[])
{
  /*
    IplImage* cFrame;
    IplImage* pFrame;
    IplImage* sFrame;
    //const char* video_input= argv[1];
    const char* video_input= "../share/walk.avi";
    CvCapture* capture = cvCreateFileCapture( video_input );
    cvNamedWindow( "Image Stabilization: Original", CV_WINDOW_AUTOSIZE );
    cvNamedWindow( "Image Stabilization: Stable", CV_WINDOW_AUTOSIZE );
    cFrame = cvQueryFrame( capture );
    pFrame = cvCloneImage(cFrame);
    const char* video_output= argv[2];
    CvVideoWriter* writer = cvCreateVideoWriter( video_output, CV_FOURCC('D', 'I', 'V', 'X') , 5, cvGetSize(cFrame));
    while(1) {
        cFrame = cvQueryFrame( capture );
        if( !cFrame ) break;
        sFrame = imagestabilization(cFrame, pFrame);
        cvShowImage( "Image Stabilization: Original", cFrame );
        cvShowImage( "Image Stabilization: Stable", sFrame );
        char c = cvWaitKey(15);
        if( c == 27 ) break;
        pFrame = cvCloneImage(cFrame);
        cvWriteFrame( writer, sFrame );
    }
    cvReleaseCapture(&capture);
    cvReleaseVideoWriter(&writer);
    cvDestroyWindow("Image Stabilization: Original");
    cvDestroyWindow("Image Stabilization: Stable");
    cvReleaseImage(&cFrame);
    cvReleaseImage(&pFrame);
    cvReleaseImage(&sFrame);
    */

  string video_file = "../share/walk.avi";
  VideoCapture video(video_file); // open the default camera
   if(!video.isOpened())  // check if we succeeded
       return -1;

   namedWindow("original",1);
   for(;;)
   {
       Mat frame;
       video >> frame; // get a new frame from video
       imshow("original", frame);
       if(waitKey(30) >= 0) break;
   }


    return 0;

}
