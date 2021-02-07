#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "MisFunciones.h"
#include "pdi_functions.h"

using namespace cv;
using namespace misFunciones;

int main(int argc, char** argv) {
	
	int filas = 512;
	int columnas = 512;	
	
	Mat image1 = imread("C:\\Users\\ribeiro\\Desktop\\PDI- 2017\\Imagenes\\huang1.jpg",CV_8UC1);
	Mat image2 = imread("C:\\Users\\ribeiro\\Desktop\\PDI- 2017\\Imagenes\\cameraman.tif",CV_8UC1);
	Mat image3 = imread("C:\\Users\\ribeiro\\Desktop\\PDI- 2017\\Imagenes\\camino.tif",CV_8UC1);
	Mat image4 = imread("C:\\Users\\ribeiro\\Desktop\\PDI- 2017\\Imagenes\\cuadros.tif",CV_8UC1);
	Mat image5 = imread("C:\\Users\\ribeiro\\Desktop\\PDI- 2017\\Imagenes\\patron2.tif",CV_8UC1);
	
	ejercicio_1_4_guia5(image1);
	ejercicio_1_4_guia5(image2);
	ejercicio_1_4_guia5(image3);
	ejercicio_1_4_guia5(image4);
	ejercicio_1_4_guia5(image5);
	
	return 0;
} 
