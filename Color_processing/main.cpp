#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <iostream>
#include "pdi_functions.h"
#include "MisFunciones.h"

using namespace cv;
using namespace std;
using namespace misFunciones;

int main(int argc, char** argv) {
	
	//Ejercicio1
	
	Mat patron = imread("C:\\Users\\ribeiro\\Desktop\\PDI- 2017\\Imagenes\\patron.tif",CV_LOAD_IMAGE_COLOR);
	Mat model = Mat::zeros( 200, 400, CV_8UC(1) ); //crear imagen para copiar tamaño (creo que es al pedo)
	resize(patron,patron,model.size());
//	namedWindow("Original",1);
//	imshow("Original", patron);
//	waitKey(0);	
	
	//1.1a)
	
	std::vector< cv::Mat > bgr; 
	cv::split(patron, bgr); //separa la imagen en tres canales para manipularlos individualmente
	
//	namedWindow("Azul",1);
//	imshow("Azul", bgr[0]);
//	waitKey(0);
//	namedWindow("Verde",1);
//	imshow("Verde", bgr[1]);
//	waitKey(0);
//	namedWindow("Rojo",1);
//	imshow("Rojo", bgr[2]);
//	waitKey(0);
	
	//1.1b)
	cv::Mat patronHSV = patron;
	cv::cvtColor(patron, patronHSV, CV_BGR2HSV); //convierte de BGR a HSV. Si out es 8 bits, H varía en [0-180], S y V en [0-255].
	
//	namedWindow("HSV",1);
//	imshow("HSV", patronHSV);
//	waitKey(0);
	
	std::vector< cv::Mat > hsv; 
	cv::split(patronHSV, hsv); //separa la imagen en tres canales para manipularlos individualmente
	
//	namedWindow("Hue",1);
//	imshow("Hue", hsv[0]);
//	waitKey(0);
//	namedWindow("Saturation",1);
//	imshow("Saturation", hsv[1]);
//	waitKey(0);
//	namedWindow("Value",1);
//	imshow("Value", hsv[2]);
//	waitKey(0);
	
//	pdi::stats(hsv[0]); //uso esto para ver el mayor valor que uso espues para restar en (*)
	
	for (int i=0; i<hsv[0].rows; i++){ 
		for (int j=0; j<hsv[0].cols; j++){
/*hue*/			hsv[0].at<uint8_t>(i,j) = 120-hsv[0].at<uint8_t>(i,j); //(*) el 120 en este caso... //(1)
/*saturation*/	hsv[1].at<uint8_t>(i,j) = 255;
/*value*/		hsv[2].at<uint8_t>(i,j) = 255;
		}
	}		
	
	cv::Mat patronModificado;
	cv::merge(hsv, patronModificado); //junta los canales manipulados en image.

	cv::cvtColor(patronModificado, patronModificado, CV_HSV2BGR); //convierte de HSV a BGR.
	//Usando CV_HSV2RGB no es necesario hacer la convercion que hice arriba (1) ¬¬
//	namedWindow("patronModificado",1);
//	imshow("patronModificado", patronModificado);
//	waitKey(0);
	
	//1.2
	
	Mat rosas = imread("C:\\Users\\ribeiro\\Desktop\\PDI- 2017\\Imagenes\\rosas.jpg",CV_LOAD_IMAGE_COLOR);
//	namedWindow("Rosas",1);
//	imshow("Rosas", rosas);
//	waitKey(0);
	
	
	cv::Mat rosasHSV = rosas;

	std::vector< cv::Mat > hsv2; 
	
	cv::cvtColor(rosas, rosasHSV, CV_BGR2HSV);
	
	cv::split(rosasHSV, hsv2); //separa la imagen en tres canales para manipularlos individualmente
	
//	pdi::stats(hsv2[0]);
//	pdi::stats(hsv2[1]);
//	pdi::stats(hsv2[2]);
	
	///COLOR OPUESTO EN HSV
	for (int i=0; i<hsv2[0].rows; i++){
		for (int j=0; j<hsv2[0].cols; j++){
			hsv2[2].at<uchar>(i,j) = 255 - hsv2[2].at<uchar>(i,j);
			int tmp_hue = ((hsv2[0].at<uchar>(i,j)) + 90); 
			if (tmp_hue>180){
				tmp_hue -=180;
			}
			hsv2[0].at<uchar>(i,j) = tmp_hue;			
		}
	}
	
//	pdi::stats(hsv2[0]);
	
	cv::Mat rosasModificado;
	cv::merge(hsv2, rosasModificado); //junta los canales manipulados en image.
	
	cv::cvtColor(rosasModificado, rosasModificado, CV_HSV2BGR); //convierte de HSV a BGR.
	
//	namedWindow("Rosas Modificado",1);
//	imshow("Rosas Modificado", rosasModificado);
//	waitKey(0);
	
	//Ejercicio 2
	
	Mat rio = imread("C:\\Users\\ribeiro\\Desktop\\PDI- 2017\\Imagenes\\rio.jpg", CV_LOAD_IMAGE_GRAYSCALE);
//	namedWindow("Rio",1);
//	imshow("Rio", rio);
//	waitKey(0);
	
	// a) Cálculo y gráfico de histograma		
	Mat histograma = pdi::histogram(rio,256,cv::Mat()); //from pdi functions		
	Mat histograma_para_mostrar = graficar_histograma(histograma, 256);
	
//	namedWindow("Histograma",1);
//	imshow("Histograma", histograma_para_mostrar);
//	waitKey(0);
	
	//Creo una máscara
	//A ojo determino que el agua esta entre los niveles 0 y 16 de intensidad
	Mat mask = Mat::zeros(rio.rows,rio.cols,CV_8UC(1));
	cv::inRange(rio,cv::Scalar(0),cv::Scalar(20),mask);
	
	cv::Mat rio_retocada(rio.rows,rio.cols,CV_8UC(3));
	
	vector<cv::Mat> bgr3;
	cv::split(rio_retocada, bgr3);
	bgr3[0] = rio;
	bgr3[1] = rio;
	bgr3[2] = rio;
	cv::merge(bgr3, rio_retocada);	
	rio_retocada.setTo(cv::Scalar(0,255,255),mask);
	
//	cv::inRange(image,cv::Scalar(lower),cv::Scalar(upper),mask); ? genera una máscara con valor 1 en
//		cada pixel cuyo valor esté en el rango [lower,upper].
//		? image.setTo(cv::Scalar(B,G,R),mask); ? Asigna el color BGR a los píxeles de la máscara.

	
//	namedWindow("Imagen Retocada",1);
//	imshow("Imagen Retocada", rio_retocada);
//	waitKey(0);
	
	//Ejercicio 3
	//3.1
	Mat sillas = imread("C:\\Users\\ribeiro\\Desktop\\PDI- 2017\\Imagenes\\flowers_oscura.tif",CV_LOAD_IMAGE_COLOR);
	Mat sillas_copia = sillas.clone();
	
	
	Mat model_sillas = Mat::zeros( sillas.rows/2, sillas.cols/2, CV_8UC(1) ); //crear imagen para copiar tamaño (creo que es al pedo)
	resize(sillas,sillas,model_sillas.size());
	resize(sillas_copia,sillas_copia,model_sillas.size());
	
//	namedWindow("Sillas",1);
//	imshow("Sillas", sillas);
//	waitKey(0);	
	
	std::vector< cv::Mat > bgr4,hsv4; 
	cv::split(sillas, bgr4);
	
	cv::cvtColor(sillas,sillas_copia, CV_BGR2HSV);
	cv::split(sillas_copia, hsv4);
	
	MatND histogramaB_ecualizado,histogramaG_ecualizado,histogramaR_ecualizado, histogramaV_ecualizado ;
	
	bgr4[0] = ecualizar_histograma(bgr4[0],histogramaB_ecualizado);//	
	bgr4[1] = ecualizar_histograma(bgr4[1],histogramaG_ecualizado);//				
	bgr4[2] = ecualizar_histograma(bgr4[2],histogramaR_ecualizado);//
	hsv4[2] = ecualizar_histograma(hsv4[2],histogramaV_ecualizado);//
	cv::merge(bgr4, sillas);
	cv::merge(hsv4, sillas_copia);
	
//	namedWindow("Sillas con histograma RGB ecualizado",1);
//	imshow("Sillas con histograma RGB ecualizado", sillas);
//	waitKey(0);	
	
	cv::Mat sillas_copia_copia;
	cv::cvtColor(sillas_copia, sillas_copia_copia, CV_HSV2BGR);
	
//	namedWindow("Sillas con histograma HSV ecualizado",1);
//	imshow("Sillas con histograma HSV ecualizado", sillas_copia_copia);
//	waitKey(0);
	
	
	//3.2
//	Mat camino = imread("C:\\Users\\ribeiro\\Desktop\\PDI- 2017\\Imagenes\\camino.tif",CV_LOAD_IMAGE_COLOR);
//	namedWindow("Camino",1);
//	imshow("Camino", camino);
//	waitKey(0);
//	
//	cv::Mat imagen_filtrada_RGB = filtro_pasa_alto_RGB(3, camino, 1);
//	
//	namedWindow("Camino Filtrada RGB",1);
//	imshow("Camino Filtrada RGB", imagen_filtrada_RGB);
//	waitKey(0);
//	
//	cv::Mat caminos_copia;
//	cv::cvtColor(camino, caminos_copia, CV_BGR2HSV);
//	cv::Mat imagen_filtrada_HSV = filtro_pasa_alto_HSV(3, caminos_copia, 1);
//	
//	cv::cvtColor(imagen_filtrada_HSV,caminos_copia, CV_HSV2BGR);
//	
//	namedWindow("Camino Filtrada HSV",1);
//	imshow("Camino Filtrada HSV", caminos_copia);
//	waitKey(0);
	
	//4
	//4.1
//	Mat imagen = imread("C:\\Users\\ribeiro\\Desktop\\PDI- 2017\\Imagenes\\germenes.jpg",CV_LOAD_IMAGE_COLOR);
//	Mat imagen = imread("C:\\Users\\ribeiro\\Desktop\\PDI- 2017\\Imagenes\\futbol.jpg",CV_LOAD_IMAGE_COLOR);
//	Mat imagen = imread("C:\\Users\\ribeiro\\Desktop\\PDI- 2017\\Imagenes\\paraguas.jpg",CV_LOAD_IMAGE_COLOR);
//	Mat imagen = imread("C:\\Users\\ribeiro\\Desktop\\PDI- 2017\\Imagenes\\wall1.png",CV_LOAD_IMAGE_COLOR);
	Mat imagen = imread("C:\\Users\\ribeiro\\Desktop\\PDI- 2017\\Imagenes\\316412.jpg",CV_LOAD_IMAGE_COLOR); //Con esta imagen se ve re bien la diferencia entre las segmentaciones en cada espacio
	
	cv::Mat model2 = cv::Mat::zeros( 500, 700, CV_8UC(1) ); //crear imagen para copiar tamaño (creo que es al pedo)
	resize(imagen,imagen,model2.size());
	
//	namedWindow("Original",1);
//	imshow("Original", imagen);	
//	waitKey(0);
	cv::Mat segRGBmask;
	misFunciones::segmentacion_RGB("Original", imagen,segRGBmask);
	
	//4.2
	cv::Mat segHSVmask;
//	misFunciones::segmentacion_HSV("Original", imagen, segHSVmask);
	
	//5
////	Mat imagen5 = imread("C:\\Users\\ribeiro\\Desktop\\PDI- 2017\\Imagenes\\germenes.jpg",CV_LOAD_IMAGE_COLOR);
//	Mat imagen5 = imread("C:\\Users\\ribeiro\\Desktop\\PDI- 2017\\Imagenes\\Deforestacion.png",CV_LOAD_IMAGE_COLOR);
//	
//	cv::Mat model3 = cv::Mat::zeros( 500, 700, CV_8UC(1) ); //crear imagen para copiar tamaño (creo que es al pedo)
//	resize(imagen5,imagen5,model3.size());
//	
//	namedWindow("Original",1);
//	imshow("Original", imagen5);	
//	waitKey(0);
//	
//	cv::Mat mascara, desmonte;
//	namedWindow("Segmentada",1);
//	cv::Mat imagen_segmentada = misFunciones::segmentacion_HSV("Segmentada", imagen5, mascara);
////	cv::Mat imagen_segmentada = misFunciones::segmentacion_RGB("Segmentada", imagen5, mascara);
//	
//	namedWindow("Mascara",1);
//	imshow("Mascara", mascara);	
//	waitKey(0);
//	
////	for (int i=0;i<mascara.rows;i++){
////		for (int j=0;j<mascara.cols;j++){
////			if (mascara.at<uchar>(i,j)==0){
////				mascara.at<uchar>(i,j)=255;
////			}else{
////				mascara.at<uchar>(i,j)=0;
////			}
////		}
////	}
//	
//	desmonte = imagen5.clone();
//	desmonte.setTo(cv::Scalar(0,0,80),mascara);
//	
//	namedWindow("Desmonte",1);
//	imshow("Desmonte",desmonte);
//	waitKey(0);
//	
//	
//	//Deteccion de zona delimitada
//	
	
	return 0;
} 
