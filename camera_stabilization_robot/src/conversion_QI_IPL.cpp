#include "conversion_QI_IPL.h"


 IplImage* QImage2IplImage(QImage *Im)
    {
    //printf("qimage2ipp");
    IplImage *imgHeader = cvCreateImageHeader( cvSize(Im->width(), Im->width()), 8, 3);
    imgHeader->imageData = (char*) Im->bits();

    uchar* newdata = (uchar*) malloc(sizeof(uchar) * Im->byteCount());
    memcpy(newdata, Im->bits(), Im->byteCount());
    imgHeader->imageData = (char*) newdata;
    //cvClo
    return imgHeader;
    }

    QImage*  IplImage2QImage(IplImage *image)
    {
    int h = image->height;
    int w = image->width;
    int channels = image->nChannels;
    QImage *Im = new QImage(w, h, QImage::Format_RGB888);
    char *data = image->imageData;

    for (int y = 0; y < h; y++, data += image->widthStep)
    {
    for (int x = 0; x < w; x++)
    {
    char r, g, b, a = 0;
    if (channels == 1)
    {
    r = data[x * channels];
    g = data[x * channels];
    b = data[x * channels];
    }
    else if (channels == 3 || channels == 4)
    {
    r = data[x * channels + 2];
    g = data[x * channels + 1];
    b = data[x * channels];
    }

    if (channels == 4)
    {
    a = data[x * channels + 3];
    Im->setPixel(x, y, qRgba(r, g, b, a));
    }
    else
    {
    Im->setPixel(x, y, qRgb(r, g, b));
    }
    }
    }
    return Im;

    }
