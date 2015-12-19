/******************************************************************************************
*	ARCHIVO:	Comparador.h
*	AUTOR:		Ana Paula Mateo Garrido
*	FUNCION:	Declaración de la clase Comparador, que permite el calculo de la posicion
*				y orientación relativa entre imágenes consecutivas de video.
******************************************************************************************/
#include <cv.h>
#include <highgui.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <iostream>
#include <vector>

using namespace std;


// Definiciones de constantes
//#define USE_FLANN
#ifndef M_PI
	#define M_PI 3.14159265358979323846
#endif

// Estructura de datos de correccion.
struct Posicion
{
	double x, y;
	double phi;
};

// CLASE COMPARADOR:	Realiza la comparación entre imagenes consecutivas, calcula y
//						almacena el error en phi, x e y.
class Comparador
{
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// DECLARACIÓN DE VARIABLES
private:
	IplImage *actualImage, *oldImage;	// Imagenes actual y anterior con la que realiza la comparacion.
	IplImage *compensada;				// Imagen compensada.
	CvMemStorage* storage;				// Almacén de memoria para las secuencias de características.
	CvPoint centro;						// Centro del rectángulo establecido para la comparación.
	CvRect rect;						// Rectángulo de Interés para la comparación.
	int ancho, alto;					// Alto y ancho del rectángulo de comparación.
	Posicion pos;						// Posición relativa detectada entre la imagen anterior y la actual.
	char cad[50];						// Posición y orientación relativa en modo texto.
	CvMat *m_rot, *m_tras;				// Matrices para la rotacion y la traslacion.
	CvPoint2D32f centro32f, a[3], b[3];	// Puntos de referencia para la rotacion y la traslacion.
	Posicion K;							// Ganancias del regulador tipo P.
	int umbralPhi;						// Umbral de descarte de phi.
//--------------------------------------------------------------------------------------------------------

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// MÉTODOS PÚBLICOS DE LA CLASE
public:

	// Constructor y destructor de la clase.
	Comparador(IplImage* init);
	~Comparador();
	//**************************************/

	// Métodos de INTERFAZ de la clase.
	Posicion getPos(){return pos;}	// Devuelve la posición relativa actual.
	char* getInformacion();			// Devuelve la posición relativa actual en modo texto.
	IplImage* getOldImage(){return oldImage;}			// Devuelve la imagen usada para comparar.
	IplImage* getActualImage(){return actualImage;}		// Devuelve la imagen actual.
	IplImage* getImagenCompensada(){return compensada;}	// Devuelve la imagen estabilizada.
	Posicion getGanancias(){return K;}		// Devuelve las ganancias utilizadas en el regulador P.
	int getUmbralPhi(){return umbralPhi;}	// Devuelve el umbral empleado para el ángulo.

	void setOldImage(IplImage* old){oldImage = cvCloneImage(old);}				// Establece la imagen de referencia.
	void setActualImage(IplImage* actual){actualImage = cvCloneImage(actual);}	// Establece la imagen actual.
	void setCentro(CvPoint c){centro = c;}	// Establece el centro de la ROI de comparación.
	void setDimensiones(int w, int h){alto=h; ancho=w;}		// Establece las dimensiones de la ROI.
	void setGanancias(double kphi, double kx, double ky)	// Establece las ganancias del regulador P.
	{ K.phi = kphi; K.x = kx; K.y = ky; }
	void setUmbralPhi(int umbral){umbralPhi = umbral;}		// Establece el umbral de ángulo a emplear.

	void escribePos();	// Escribe en la línea de comandos la posicion relativa actual.
	//***************************************************************************************************/

	//	Metodos funcionales de la clase
	int calcular_errores();	// Calcula la posición y orientación relativas entre las imágenes.
	void compensarAngulo();		// Corrige el ángulo en la imagen actual.
	void compensarPosicion();	// Corrigue la posición en la imagen actual.
	void compensarAngulo(IplImage* input);		// Corrige el ángulo en la imagen que se le pasa.
	void compensarPosicion(IplImage* input);	// Corrigue el ángulo en la imagen que se le pasa.
	
	void estabilizarImagen(IplImage* input);	// Calcula y realiza la estabilización de las imágenes con
												// los valores del regulador establecidos en K.
	void devuelveRegion(IplImage* input, IplImage* output, CvRect r);	// Devuelve la region de la imagen de entrada.
//----------------------------------------------------------------------------------------------------------

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//MÉTODOS PRIVADOS DE LA CLASE
private:

	double calcular_angulo(CvPoint p);	// Devuelve el angulo formado por la recta que une p y el centro.
	double devuelveGrados(double phi){return phi*180/M_PI;}	// Devuelve el valor del ángulo en grados.

	// Compara descriptores de puntos para evaluar su correspondencia.
	double compareSURFDescriptors( const float* d1, const float* d2, double best, int length );

	// Establece la correspondencia entre puntos de ambas imágenes.
	int naiveNearestNeighbor(	const float* vec, int laplacian, const CvSeq* model_keypoints,
								const CvSeq* model_descriptors );
	

	// Buscan parejas de puntos correspondientes.
#ifdef USE_FLANN
	void flannFindPairs(	const CvSeq*, const CvSeq* objectDescriptors,
							const CvSeq*, const CvSeq* imageDescriptors,
							vector<int>& ptpairs );
#else
	void findPairs( const CvSeq* objectKeypoints, const CvSeq* objectDescriptors,
					const CvSeq* imageKeypoints, const CvSeq* imageDescriptors,
					vector<int>& ptpairs );
#endif
	// Identifica objetos planos entre las secuencias de puntos correspondientes.
	int locatePlanarObject( const CvSeq* objectKeypoints, const CvSeq* objectDescriptors,
							const CvSeq* imageKeypoints, const CvSeq* imageDescriptors,
                    		const CvPoint src_corners[4], CvPoint dst_corners[4] );
};


