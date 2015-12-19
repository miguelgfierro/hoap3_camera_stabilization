//
// image_stabilization.cpp
//
// Author: Ana Paula Mateo <anapaula.mateog@gmail.com >, (C) 2010
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
#include "Comparer.h"


IplImage* imagestabilization(IplImage* image, IplImage* oldImage)
{
  IplImage *muestra;	// Declaration of images to be used

  muestra = cvCreateImage(cvGetSize(image),8,3);
  CvRect rec = cvRect(image->width*0.1,image->height*0.1,image->width*0.8,image->height*0.8);

  Comparador compara = Comparador(image);	// comparator creation and intialization
  compara.setOldImage(oldImage);

  compara.estabilizarImagen(image);	// Stabilizing
  compara.devuelveRegion(compara.getImagenCompensada(),muestra,rec);
  muestra = cvCloneImage(compara.getImagenCompensada());

  return muestra;
}

