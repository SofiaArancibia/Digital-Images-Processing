#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "MisFunciones.h"
#include "pdi_functions.h"

using namespace cv;
using namespace std;
using namespace misFunciones;

int main(int argc, char** argv) {
	
//	namedWindow("Original",1);
//	cv::Mat imagen = imread("C:\\Users\\ribeiro\\Desktop\\PDI- 2017\\Imagenes\\mosquito.jpg", CV_LOAD_IMAGE_GRAYSCALE);
//	cv::Mat imagen = imread("C:\\Users\\ribeiro\\Desktop\\PDI- 2017\\Imagenes\\estanbul.tif", CV_LOAD_IMAGE_GRAYSCALE);
//	cv::Mat imagen = imread("C:\\Users\\ribeiro\\Desktop\\PDI- 2017\\Imagenes\\ej7a.tif", CV_LOAD_IMAGE_GRAYSCALE);
//	cv::Mat imagen = imread("C:\\Users\\ribeiro\\Desktop\\PDI- 2017\\Imagenes\\img.tif", CV_LOAD_IMAGE_GRAYSCALE);
//	cv::Mat imagen = imread("C:\\Users\\ribeiro\\Desktop\\PDI- 2017\\Imagenes\\huang1.jpg", CV_LOAD_IMAGE_GRAYSCALE);
//	cv::Mat imagen = imread("C:\\Users\\ribeiro\\Desktop\\PDI- 2017\\Imagenes\\patron2.tif", CV_LOAD_IMAGE_GRAYSCALE);
//	imshow("Original", imagen);
//	waitKey(0);
//	
//	cv::Mat salida;	
//	
//	cv::Mat ruidosa = ensuciar(imagen,0,0.05);
//	namedWindow("Ruidosa",1);
//	imshow("Ruidosa",ruidosa);
//	
//	detector_de_bordes(imagen, salida,1); //El laplaciano y el LoG dan horrible
//	namedWindow("Deteccion de bordes",1);	
//	imshow("Deteccion de bordes",salida);
//	waitKey(0);
	
	
	//ejercicio21_guia7
	ejercicio21_guia7();
	
	return 0;
} 
