#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "pdi_functions/pdi_functions.h"
#include <iostream>

using namespace cv;
using namespace std;

//FILTRO NOTCH
cv::Mat FiltroNotch(cv::Mat img, cv::Mat modulo){
	cv::Mat modulo_filtro = cv::Mat::ones(img.size(),CV_32FC1);
	
	//PUNTO IMAGEN img_degradada.tif
//	CvPoint ctro1 = cvPoint(198,218);								//Punto donde esta el ruido
//	CvPoint ctro2 = cvPoint(58,38);									//Punto donde esta el ruido
//	circle(modulo_filtro,ctro1,10,0,-1);
//	circle(modulo_filtro,ctro2,10,0,-1);
	
	//PUNTO IMAGEN HeadCT_degradada.tif
//	CvPoint ctro1 = cvPoint(img.rows/2,img.cols/2);					//Punto donde esta el ruido
//	circle(modulo_filtro,ctro1,10,0,-1);

	//PUNTO IMAGEN Noisy_moon.jpg
	CvPoint ctro1 = cvPoint(img.rows/2,img.cols/2);					//Punto donde esta el ruido
	circle(modulo_filtro,ctro1,5,0,-1);
	
	namedWindow("Filtro Notch",WINDOW_NORMAL);
	imshow("Filtro Notch",modulo_filtro);
	pdi::centre(modulo_filtro);
	cv::Mat img_filtrada = pdi::filter(img,modulo_filtro);

	namedWindow("Imagen Filtrada Notch",WINDOW_NORMAL);
	imshow("Imagen Filtrada Notch",img_filtrada);
	modulo = pdi::spectrum(img_filtrada);
	namedWindow("Modulo Imagen Filtrada",WINDOW_NORMAL);
	imshow("Modulo Imagen Filtrada",modulo);
	
	return img_filtrada;
}

//ECM
float errorCuadraticoMedio(Mat a, Mat b){
	if (a.size()!=b.size()) {cout<<"Imagenes de distinto tamanio!!"; return 0;}
	float error=0, tam = a.rows*a.cols;
	for (int i=0 ; i<a.rows ; i++){
		for (int j=0 ; j<a.cols ; j++){
			error += pow(a.at<float>(i,j) - b.at<float>(i,j),2)/tam;
		}
	}
	return error;
}

int main(int argc, char** argv) {

//	cv::Mat img_nd= imread("img.tif",CV_LOAD_IMAGE_GRAYSCALE);
//	namedWindow("Imagen no degradada",WINDOW_NORMAL);
//	imshow("Imagen no degradada",img_nd);
//	
//	cv::Mat img= imread("img_degradada.tif",CV_LOAD_IMAGE_GRAYSCALE);
//	namedWindow("Imagen degradada",WINDOW_NORMAL);
//	imshow("Imagen degradada",img);
	
//	cv::Mat img= imread("HeadCT_degradada.tif",CV_LOAD_IMAGE_GRAYSCALE);
//	namedWindow("Imagen degradada",WINDOW_NORMAL);
//	imshow("Imagen degradada",img);
	
	cv::Mat img= imread("noisy_moon.jpg",CV_LOAD_IMAGE_GRAYSCALE);
	namedWindow("Imagen degradada",WINDOW_NORMAL);
	imshow("Imagen degradada",img);
	
	//INFORMACION Y ESPECTRO DE LA IMAGEN
	img.convertTo(img,CV_32FC1,1./255);
	pdi::info(img);
	cv::Mat modulo = pdi::spectrum(img);
	namedWindow("Espectro Original",WINDOW_NORMAL);
	imshow("Espectro Original",modulo);
	
	//FILTRO NOTCH
	cv::Mat img_filtrada=FiltroNotch(img,modulo);

	//ECM
	cout<<endl<<"ECM: "<<errorCuadraticoMedio(img_filtrada,img);
	
	waitKey(0);
	return 0;
}


