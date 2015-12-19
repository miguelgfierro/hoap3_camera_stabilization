#include "image_stabilization.h"


#include "Comparador.h"


IplImage* imagestabilization(IplImage* image)

{

        IplImage *muestra;	// Declaration of images to be used

        // Window creation
        //cvNamedWindow("ORIGINAL");
        //cvNamedWindow("ESTABILIZADA");
        //cvNamedWindow("REGION");


        muestra = cvCreateImage(cvGetSize(image),8,3);
        CvRect rec = cvRect(image->width*0.1,image->height*0.1,image->width*0.8,image->height*0.8);


        Comparador compara = Comparador(image);	// comparator creation and intialization
        //image = cvQueryFrame(capture);		// Capturing an image
        compara.estabilizarImagen(image);	// Stabilizing
        compara.devuelveRegion(compara.getImagenCompensada(),muestra,rec);

        // Shows the previous and final images
        //cvShowImage("ORIGINAL",image);
        //cvShowImage("ESTABILIZADA",compara.getImagenCompensada());
        //cvShowImage("REGION",muestra);



        return muestra;
}


IplImage* imagestabilization(IplImage* image, IplImage* oldImage)
{

        IplImage *muestra;	// Declaration of images to be used

        // Window creation
        cvNamedWindow("ORIGINAL");
        cvNamedWindow("ESTABILIZADA");
        cvNamedWindow("REGION");


        muestra = cvCreateImage(cvGetSize(image),8,3);
        CvRect rec = cvRect(image->width*0.1,image->height*0.1,image->width*0.8,image->height*0.8);


        Comparador compara = Comparador(image);	// comparator creation and intialization
        compara.setOldImage(oldImage);

        //image = cvQueryFrame(capture);		// Capturing an image
        compara.estabilizarImagen(image);	// Stabilizing
        compara.devuelveRegion(compara.getImagenCompensada(),muestra,rec);

        // Shows the previous and final images
        cvShowImage("ORIGINAL",image);
        cvShowImage("ESTABILIZADA",compara.getImagenCompensada());
        cvShowImage("REGION",muestra);



        return muestra;
}
