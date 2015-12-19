// Copyright  (C)  2010  Miguel González-Fierro

// Version: 0.3
// Author: Juan G Victores <jcgvicto at ing dot uc3m dot es>
// Maintainer: Juan G Victores <jcgvicto at ing dot uc3m dot es>
// URL: n/a

// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.

// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.

// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

/**
 * \mainpage image_stabilization
 *
 * This is the documentation of the image_stabilization module - Capture image frame stabilization

 Other existing driver modules are:

 *
 */


#include <cv.h>
#include <highgui.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

#include <iostream>
#include <vector>




IplImage* imagestabilization(IplImage* image);

IplImage* imagestabilization(IplImage* image, IplImage* oldImage);


