/******************************************************************************************
*	ARCHIVO:	Comparador.cpp
*	AUTOR:		Ana Paula Mateo Garrido
*	FUNCION:	Implementacion de la clase Comparador.
******************************************************************************************/
#include "Comparador.h"

// Constructor de la clase.
Comparador::Comparador(IplImage* init)
{
	storage = cvCreateMemStorage(0);
	centro = cvPoint(init->width/2,init->height/2);
	ancho=init->width*0.8;
	alto=init->height*0.8;
	rect = cvRect(centro.x-ancho/2,centro.y-alto/2,ancho,alto);
	oldImage = cvCloneImage(init);
	actualImage = cvCloneImage(init);
	compensada = cvCloneImage(init);

	// Declaramos las matrices de rotación y traslacion e inicializamos las referencias para
	// dichas transformaciones afines.
	m_rot= cvCreateMat(2, 3, CV_64FC1);
	m_tras= cvCreateMat(2, 3, CV_64FC1);
	centro32f = cvPoint2D32f(actualImage->width/2.0, actualImage->height/2.0);	// Centro de la rotacion.
	a[0] = cvPoint2D32f(0,0);					// Puntos de referencia para la traslación.
	a[1] = cvPoint2D32f(actualImage->width,0);
	a[2] = cvPoint2D32f(0,actualImage->height);

	// Valores por defecto de los reguladores proporcionales.
	K.phi = 1.0;
	K.x = 0.5;
	K.y = 0.7;

	// Valor por defecto del umbral de ángulo para la estabilización.
	umbralPhi = 30;
}

// Destructor de la clase
Comparador::~Comparador()
{
	cvReleaseImage(&actualImage);
	cvReleaseImage(&oldImage);
	cvReleaseImage(&compensada);
	cvReleaseMat(&m_rot);
	cvReleaseMat(&m_tras);
	cvReleaseMemStorage(&storage);
}

// Compara descriptores de puntos para evaluar su correspondencia.
double Comparador::compareSURFDescriptors( const float* d1, const float* d2, double best, int length )
{
    double total_cost = 0;
    assert( length % 4 == 0 );
    for( int i = 0; i < length; i += 4 )
    {
        double t0 = d1[i] - d2[i];
        double t1 = d1[i+1] - d2[i+1];
        double t2 = d1[i+2] - d2[i+2];
        double t3 = d1[i+3] - d2[i+3];
        total_cost += t0*t0 + t1*t1 + t2*t2 + t3*t3;
        if( total_cost > best )
            break;
    }
    return total_cost;
}

// Establece la correspondencia entre puntos de ambas secuencias.
int Comparador::naiveNearestNeighbor(	const float* vec, int laplacian,
										const CvSeq* model_keypoints,
										const CvSeq* model_descriptors )
{
    int length = (int)(model_descriptors->elem_size/sizeof(float));
    int i, neighbor = -1;
    double d, dist1 = 1e6, dist2 = 1e6;
    CvSeqReader reader, kreader;
    cvStartReadSeq( model_keypoints, &kreader, 0 );
    cvStartReadSeq( model_descriptors, &reader, 0 );

    for( i = 0; i < model_descriptors->total; i++ )
    {
        const CvSURFPoint* kp = (const CvSURFPoint*)kreader.ptr;
        const float* mvec = (const float*)reader.ptr;
    	CV_NEXT_SEQ_ELEM( kreader.seq->elem_size, kreader );
        CV_NEXT_SEQ_ELEM( reader.seq->elem_size, reader );
        if( laplacian != kp->laplacian )
            continue;
        d = compareSURFDescriptors( vec, mvec, dist2, length );
        if( d < dist1 )
        {
            dist2 = dist1;
            dist1 = d;
            neighbor = i;
        }
        else if ( d < dist2 )
            dist2 = d;
    }
    if ( dist1 < 0.6*dist2 )
        return neighbor;
    return -1;
}

// Busca parejas de puntos en las secuencias y establece su correspondencia.
#ifdef USE_FLANN
void Comparador::flannFindPairs(	const CvSeq*, const CvSeq* objectDescriptors,
									const CvSeq*, const CvSeq* imageDescriptors,
									vector<int>& ptpairs )
{
	int length = (int)(objectDescriptors->elem_size/sizeof(float));

	cv::Mat m_object(objectDescriptors->total, length, CV_32F);
	cv::Mat m_image(imageDescriptors->total, length, CV_32F);


	// copy descriptors
	CvSeqReader obj_reader;
	float* obj_ptr = m_object.ptr<float>(0);
	cvStartReadSeq( objectDescriptors, &obj_reader );
	for(int i = 0; i < objectDescriptors->total; i++ )
	{
		const float* descriptor = (const float*)obj_reader.ptr;
		CV_NEXT_SEQ_ELEM( obj_reader.seq->elem_size, obj_reader );
		memcpy(obj_ptr, descriptor, length*sizeof(float));
		obj_ptr += length;
	}
	CvSeqReader img_reader;
	float* img_ptr = m_image.ptr<float>(0);
	cvStartReadSeq( imageDescriptors, &img_reader );
	for(int i = 0; i < imageDescriptors->total; i++ )
	{
		const float* descriptor = (const float*)img_reader.ptr;
		CV_NEXT_SEQ_ELEM( img_reader.seq->elem_size, img_reader );
		memcpy(img_ptr, descriptor, length*sizeof(float));
		img_ptr += length;
	}

	// find nearest neighbors using FLANN
	cv::Mat m_indices(objectDescriptors->total, 2, CV_32S);
	cv::Mat m_dists(objectDescriptors->total, 2, CV_32F);
	cv::flann::Index flann_index(m_image, cv::flann::KDTreeIndexParams(4));  // using 4 randomized kdtrees
	flann_index.knnSearch(m_object, m_indices, m_dists, 2, cv::flann::SearchParams(64) ); // maximum number of leafs checked

	int* indices_ptr = m_indices.ptr<int>(0);
	float* dists_ptr = m_dists.ptr<float>(0);
	for (int i=0;i<m_indices.rows;++i) {
		if (dists_ptr[2*i]<0.6*dists_ptr[2*i+1]) {
			ptpairs.push_back(i);
			ptpairs.push_back(indices_ptr[2*i]);
		}
	}
}
#else
void Comparador::findPairs(	const CvSeq* objectKeypoints, const CvSeq* objectDescriptors,
							const CvSeq* imageKeypoints, const CvSeq* imageDescriptors,
							vector<int>& ptpairs )
{
	int i;
	CvSeqReader reader, kreader;
	cvStartReadSeq( objectKeypoints, &kreader );
	cvStartReadSeq( objectDescriptors, &reader );
	ptpairs.clear();

	for( i = 0; i < objectDescriptors->total; i++ )
	{
		const CvSURFPoint* kp = (const CvSURFPoint*)kreader.ptr;
		const float* descriptor = (const float*)reader.ptr;
		CV_NEXT_SEQ_ELEM( kreader.seq->elem_size, kreader );
		CV_NEXT_SEQ_ELEM( reader.seq->elem_size, reader );
		int nearest_neighbor = naiveNearestNeighbor( descriptor, kp->laplacian, imageKeypoints, imageDescriptors );
		if( nearest_neighbor >= 0 )
		{
			ptpairs.push_back(i);
			ptpairs.push_back(nearest_neighbor);
		}
	}
}
#endif

// Busca objetos planos en las secuencias de puntos.
int Comparador:: locatePlanarObject( const CvSeq* objectKeypoints, const CvSeq* objectDescriptors,
                    		const CvSeq* imageKeypoints, const CvSeq* imageDescriptors,
                    		const CvPoint src_corners[4], CvPoint dst_corners[4] )
{
    double h[9];
    CvMat _h = cvMat(3, 3, CV_64F, h);
    vector<int> ptpairs;
    vector<CvPoint2D32f> pt1, pt2;
    CvMat _pt1, _pt2;
    int i, n;

#ifdef USE_FLANN
    flannFindPairs( objectKeypoints, objectDescriptors, imageKeypoints, imageDescriptors, ptpairs );
#else
    findPairs( objectKeypoints, objectDescriptors, imageKeypoints, imageDescriptors, ptpairs );
#endif

    n = ptpairs.size()/2;
    if( n < 4 )
        return 0;

    pt1.resize(n);
    pt2.resize(n);
    for( i = 0; i < n; i++ )
    {
        pt1[i] = ((CvSURFPoint*)cvGetSeqElem(objectKeypoints,ptpairs[i*2]))->pt;
        pt2[i] = ((CvSURFPoint*)cvGetSeqElem(imageKeypoints,ptpairs[i*2+1]))->pt;
    }

    _pt1 = cvMat(1, n, CV_32FC2, &pt1[0] );
    _pt2 = cvMat(1, n, CV_32FC2, &pt2[0] );
    if( !cvFindHomography( &_pt1, &_pt2, &_h, CV_RANSAC, 5 ))
        return 0;

    for( i = 0; i < 4; i++ )
    {
        double x = src_corners[i].x, y = src_corners[i].y;
        double Z = 1./(h[6]*x + h[7]*y + h[8]);
        double X = (h[0]*x + h[1]*y + h[2])*Z;
        double Y = (h[3]*x + h[4]*y + h[5])*Z;
        dst_corners[i] = cvPoint(cvRound(X), cvRound(Y));
    }

    return 1;
}

// Localiza la región definida por el rectángulo de interés en oldImage en la nueva imagen
// y realiza el calculo de la posición y orientación relativas 2D entre las mismas.
int Comparador::calcular_errores()
{
	IplImage* image = cvCreateImage(cvGetSize(actualImage),actualImage->depth,1);	// Creo una imagen en escala de grises.
	cvCvtColor(actualImage,image,CV_BGR2GRAY);	// Convierto a gris la imagen actual.

    cvSetImageROI(oldImage,  rect );			// Establece la región de interés de la imagen de referencia.
    IplImage* oldRegion = cvCreateImage(cvGetSize(oldImage),oldImage->depth,oldImage->nChannels);
	IplImage* object = cvCreateImage(cvGetSize(oldImage),oldImage->depth,1);
    cvCopy(oldImage,oldRegion,NULL);	// Se copia dicha región de interés en una nueva imagen.
	cvCvtColor(oldRegion,object,CV_BGR2GRAY);
	cvReleaseImage(&oldRegion);
    cvResetImageROI(oldImage);		// Se reestablece la región de interés de la imagen de referencia.

	// Inicialización de las secuencias de puntos.
    CvSeq *objectKeypoints = 0, *objectDescriptors = 0;
    CvSeq *imageKeypoints = 0, *imageDescriptors = 0;

    CvSURFParams params = cvSURFParams(500, 1);	// Se buscarán 500 puntos característicos en cada imagen.

	// Se extraen las secuencias de puntos de cada imagen.
    cvExtractSURF( object, 0, &objectKeypoints, &objectDescriptors, storage, params );
    cvExtractSURF( image, 0, &imageKeypoints, &imageDescriptors, storage, params );

	if( objectDescriptors->total > 2 && imageDescriptors->total > 2 )
	{
		// Se inicializan las esquinas del plano.
		CvPoint src_corners[4] = {{0,0}, {object->width,0}, {object->width, object->height}, {0, object->height}};
		CvPoint dst_corners[4] = {src_corners[0],src_corners[1],src_corners[2],src_corners[3]};

		if(	locatePlanarObject( objectKeypoints, objectDescriptors, imageKeypoints,
			imageDescriptors, src_corners, dst_corners ))	
		{	// Si se localiza un objeto plano se calcula el error de posicion y orientación a
			// a partir de sus vértices.

			double vector[4]={0,0,0,0};						
			double sumaX=0;
			double sumaY=0;

			for( int i = 0; i < 4; i++ )
			{
				CvPoint r1 = dst_corners[i%4];
				CvPoint r2 = dst_corners[(i+1)%4];

				sumaX+=r1.x;
				sumaY+=r1.y;
				vector[i]=calcular_angulo(r1); 
			}
			pos.phi= devuelveGrados((vector[0]+vector[1]+vector[2]+vector[3])/4.0);
			pos.x = (sumaX/4.0) - centro.x;
			pos.y = (sumaY/4.0) - centro.y;
		}
		else
		{
			pos.phi = 0;
			pos.x = 0;
			pos.y = 0;
		}
	}
	else
	{
		pos.phi = 0;
		pos.x = 0;
		pos.y = 0;
	}

	// Se libera la memoria ocupada en este método.
	cvReleaseImage(&object);
	cvReleaseImage(&image);

	// Se limpia el almacén de memoria.
	cvClearMemStorage(storage);
    return 0;
}


double Comparador::calcular_angulo(CvPoint p)
{
	double phi= atan2((float)(p.y-centro.y),(float)(p.x-centro.x));
	return phi;
}

void Comparador::escribePos()
{
	printf("\n +++ angulo->%f, pos-> %f, %f\n", pos.phi,pos.x,pos.y);
}

char* Comparador::getInformacion()
{
	sprintf(cad,"PHI: %f | X: %f | Y: %f",pos.phi,pos.x,pos.y);
	return cad;
}

void Comparador::compensarAngulo(IplImage* input)
{
	// Crea una matriz de giro para el ángulo phi calculado.
	cv2DRotationMatrix(centro32f, pos.phi*K.phi, 1.0, m_rot);	// Calcula la matriz de giro.
	cvWarpAffine(input, compensada, m_rot);		// Gira la imagen con dicha matriz.
}

void Comparador::compensarPosicion(IplImage* input)
{
	for( int i = 0; i < 3; i++ )
	{
		b[i].x = a[i].x - pos.x*K.x;
		b[i].y = a[i].y - pos.y*K.y;
	}
	cvGetAffineTransform( a, b, m_tras );		// Calcula la matriz de traslacion.
	cvWarpAffine(input, compensada, m_tras );	// Transforma la imagen condicha traslacion.
}

void Comparador::compensarAngulo()
{
	compensarAngulo(actualImage);
}

void Comparador::compensarPosicion()
{
	compensarPosicion(actualImage);
}

void Comparador::estabilizarImagen(IplImage *input)
{
	setActualImage(input);		// Establece la imagen actual.
 	calcular_errores();			// Calcula la posición relativa respecto de la imagen
										// almacenada anteriormente.		

	if( pos.phi < umbralPhi )
	{
		compensarAngulo();
		// Retroalimentación: pone como imagen de referencia para futuras comparaciones
		// la imagen con el giro corregido.
		setOldImage(compensada);	// Establece la imagen de referencia.

		compensarPosicion(oldImage);
		setOldImage(compensada);
	}
	else
		setOldImage(input);
}

// Metodo que devuelve la region limitada por r de la imagen de entrada en la salida.
void Comparador::devuelveRegion(IplImage *input, IplImage *output, CvRect r)
{
	
	CvPoint2D32f a[4],b[4];

	// Puntos de origen del warp.
	a[0].x = r.x;				a[0].y = r.y;
	a[1].x = r.x + r.width;		a[1].y = r.y;
	a[2].x = r.x + r.width;		a[2].y = r.y + r.height;
	a[3].x = r.x;				a[3].y = r.y + r.height;

	// Puntos de destino del warp.
	b[0].x = 0;					b[0].y = 0;
	b[1].x = output->width;		b[1].y = 0;
	b[2].x = output->width;		b[2].y = output->height;
	b[3].x = 0;					b[3].y = output->height;

	CvMat* t = cvCreateMat(3, 3, CV_64FC1);	// Matriz de transformacion.

	cvGetPerspectiveTransform(a,b,t);		// Calcula la matriz de transformacion.
	cvWarpPerspective(input,output,t);		// Realiza la transformacion de perspectiva.

	cvReleaseMat( &t );	// Liberamos la memoria de la matriz creada.
}
