#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "../pdi_functions-master/pdi_functions.h"
#include "MisFunciones.h"
#include <vector>


using namespace cv;
using namespace std;
using namespace pdi;

int main(int argc, char** argv){
	//create a gui window:
//	namedWindow("Output",1); //ventana para mostrar la imagen
	//initialize a 120X350 matrix of black pixels:
	//Mat output = Mat::zeros( 120, 350, CV_8UC(3) ); //crear imagen 
	
	/// Ejercicio 1
	
//	Mat output = imread("C:\\Users\\ribeiro\\Desktop\\PDI- 2017\\Imagenes\\billete.jpg");
//	Mat output = imread("C:\\Users\\ribeiro\\Desktop\\PDI- 2017\\Imagenes\\chairs.jpg");
//	imshow("Output", output);
	
//	int filas = output.rows;
//	int columnas = output.cols;
	
//	cout<<"Cantidad de columnas: "<<columnas<<endl;
//	cout<<"Cantidad de filas: "<<filas<<endl;
//	cout<<"Cantidad de canales: "<<output.channels()<<endl;
//	cout<<"Profundidad: "<<output.depth()<<endl;
//	cout<<"Size: "<<output.size()<<endl;

	//write text on the matrix:
//	putText(output,
//			"Hello World :)",
//			cvPoint(15,70),
//			FONT_HERSHEY_PLAIN,
//			3,
//			cvScalar(0,255,0),
//			4);
	
	//La imagen se almacena como una matriz de filas, columnas y canales
	//Los pixeles se almacenan en orden BGR
	
	//Recortar imagen
	
	//Setup a rectangle to define your ROI
//	Rect myROI(100,100,200,200);
//		
//	///CROP the Image
//	Mat croppedImage = output(myROI);
	
	//Show the croppedImage
//	namedWindow("Cropped Image",1); //ventana para mostrar la imagen
//	imshow("Cropped Image", croppedImage);

	///varias imagenes en una sola ventana : en readme.txt
	
	///Ejercicio 2
	
	//Si abro la imagen en escala de grises puedo decir que lo veo son valores de intensidad
	
	Mat botellas = imread("C:\\Users\\ribeiro\\Desktop\\PDI- 2017\\Imagenes\\Botellas_test\\3.jpg",CV_LOAD_IMAGE_GRAYSCALE);
//	Mat botellas = imread("C:\\Users\\ribeiro\\Desktop\\PDI- 2017\\Imagenes\\botellas.tif",CV_LOAD_IMAGE_GRAYSCALE);
//	Mat botellas = imread("C:\\Users\\ribeiro\\Desktop\\PDI- 2017\\Imagenes\\botellas.tif");
		
	std::cout<<"Filas: "<<botellas.rows<<std::endl;
	std::cout<<"Columnas: "<<botellas.cols<<std::endl;
	
	namedWindow("Botellas",1); //ventana para mostrar la imagen
	imshow("Botellas", botellas);
	
	///IMPORTANTE: ver como se pasan los parametros extra en el callback
	setMouseCallback("Botellas", misFunciones::capturar_ROI,(void*)&botellas);
	
//	misFunciones::nivel_de_llenado_botella(botellas);
	
	//paso f100, puede ser cualquier otra...
	vector<int>intensidades;
	misFunciones::PerfilDeIntensidad(botellas,"f100",false,intensidades);
	
	
	

	
	
	
	

	//wait for the user to press any key:
	waitKey(0);
	return 0;
}
