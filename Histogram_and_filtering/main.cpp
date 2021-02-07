#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <vector>
#include "MisFunciones.h"

using namespace cv;
using namespace std;
using namespace misFunciones;
using namespace pdi;



int main(int argc, char** argv) {
	
//		//Carga de imagen con mal contraste -> oscura
////		Mat imagen3 = imread("C:\\Users\\ribeiro\\Desktop\\PDI- 2017\\Imagenes\\imagenE.tif", CV_LOAD_IMAGE_GRAYSCALE);
//		Mat imagen3 = imread("C:\\Users\\ribeiro\\Desktop\\PDI- 2017\\Imagenes\\imagenB.tif", CV_LOAD_IMAGE_GRAYSCALE);
////		Mat imagen3 = imread("C:\\Users\\ribeiro\\Desktop\\PDI- 2017\\Imagenes\\imagenC.tif", CV_LOAD_IMAGE_GRAYSCALE); //Aca la ecualizacion queda fea
////		Mat imagen3 = imread("C:\\Users\\ribeiro\\Desktop\\PDI- 2017\\Imagenes\\rostro2.png", CV_LOAD_IMAGE_GRAYSCALE); //Aca tambien
//		
//			
//		//Ejercicio 1_1: 
//		
//		// a) Cálculo y gráfico de histograma		
//		Mat histograma = histogram(imagen3,256,cv::Mat()); //from pdi functions		
//		//cv::MatND //N-Dimensional matrix		
//		Mat histograma_para_mostrar = graficar_histograma(histograma, 256);
//		
//		// b) Ecualización de histograma y graficación	
//		MatND histograma_ecualizado;
//		cv::Mat salida = ecualizar_histograma(imagen3,histograma_ecualizado);//				
//		cv::Mat histograma_ecualizado_para_mostrar = graficar_histograma(histograma_ecualizado, 256);//		
//		
//		
//		vector<cv::Mat> imagenes_e_histogramas;
//		
//		imagenes_e_histogramas.push_back(imagen3);		
//		imagenes_e_histogramas.push_back(salida);
//		imagenes_e_histogramas.push_back(histograma_para_mostrar);
//		imagenes_e_histogramas.push_back(histograma_ecualizado_para_mostrar);
//		
//		//Grafico los 4 resultados
//		Mat model = Mat::zeros( 300, 400, CV_8UC(1) ); //crear imagen para copiar tamaño (creo que es al pedo)
//		for (int i=0; i<imagenes_e_histogramas.size(); i++){
//			resize(imagenes_e_histogramas[i],imagenes_e_histogramas[i],model.size());
//		}
//		cv::Mat image_array = pdi::mosaic(imagenes_e_histogramas,2);
//		namedWindow("Image Array"); //ventana para mostrar la imagen
//		imshow("Image Array", image_array);
//		waitKey(0);
		
	//Ejercicio 1_2
	
//	Mat imagenA = imread("C:\\Users\\ribeiro\\Desktop\\PDI- 2017\\Imagenes\\imagenA.tif", CV_LOAD_IMAGE_GRAYSCALE);
//	Mat imagenB = imread("C:\\Users\\ribeiro\\Desktop\\PDI- 2017\\Imagenes\\imagenB.tif", CV_LOAD_IMAGE_GRAYSCALE);
//	Mat imagenC = imread("C:\\Users\\ribeiro\\Desktop\\PDI- 2017\\Imagenes\\imagenC.tif", CV_LOAD_IMAGE_GRAYSCALE);
//	Mat imagenD = imread("C:\\Users\\ribeiro\\Desktop\\PDI- 2017\\Imagenes\\imagenD.tif", CV_LOAD_IMAGE_GRAYSCALE);
//	Mat imagenE = imread("C:\\Users\\ribeiro\\Desktop\\PDI- 2017\\Imagenes\\imagenE.tif", CV_LOAD_IMAGE_GRAYSCALE);
//	
//	Mat histograma1 = histogram(imagenA,256,cv::Mat()); //from pdi functions		
//	//cv::MatND //N-Dimensional matrix		
//	Mat histograma_para_mostrar1 = graficar_histograma(histograma1, 256);
//	
//	Mat histograma2 = histogram(imagenB,256,cv::Mat()); //from pdi functions		
//	//cv::MatND //N-Dimensional matrix		
//	Mat histograma_para_mostrar2 = graficar_histograma(histograma2, 256);
//	
//	Mat histograma3 = histogram(imagenC,256,cv::Mat()); //from pdi functions		
//	//cv::MatND //N-Dimensional matrix		
//	Mat histograma_para_mostrar3 = graficar_histograma(histograma3, 256);
//	
//	Mat histograma4 = histogram(imagenD,256,cv::Mat()); //from pdi functions		
//	//cv::MatND //N-Dimensional matrix		
//	Mat histograma_para_mostrar4 = graficar_histograma(histograma4, 256);
//	
//	Mat histograma5 = histogram(imagenE,256,cv::Mat()); //from pdi functions		
//	//cv::MatND //N-Dimensional matrix		
//	Mat histograma_para_mostrar5 = graficar_histograma(histograma5, 256);
//	
//	
//	vector<cv::Mat> imagenes_e_histogramas;
//	
//	imagenes_e_histogramas.push_back(imagenA);		
//	imagenes_e_histogramas.push_back(imagenB);		
//	imagenes_e_histogramas.push_back(imagenC);		
//	imagenes_e_histogramas.push_back(imagenD);		
//	imagenes_e_histogramas.push_back(imagenE);		
//	imagenes_e_histogramas.push_back(histograma_para_mostrar1);
//	imagenes_e_histogramas.push_back(histograma_para_mostrar2);
//	imagenes_e_histogramas.push_back(histograma_para_mostrar3);
//	imagenes_e_histogramas.push_back(histograma_para_mostrar4);
//	imagenes_e_histogramas.push_back(histograma_para_mostrar5);
//	
//	Mat model = Mat::zeros( 200, 300, CV_8UC(1) ); //crear imagen para copiar tamaño (creo que es al pedo)
//	for (int i=0; i<imagenes_e_histogramas.size(); i++){
//		resize(imagenes_e_histogramas[i],imagenes_e_histogramas[i],model.size());
//	}
//	cv::Mat image_array = pdi::mosaic(imagenes_e_histogramas,2);
//	namedWindow("Image Array"); //ventana para mostrar la imagen
//	imshow("Image Array", image_array);
//	waitKey(0);
	
//		//Ejecicio 2
//		
//		cv::Mat img = imread("C:\\Users\\ribeiro\\Desktop\\PDI- 2017\\Imagenes\\camaleon.tif", CV_LOAD_IMAGE_GRAYSCALE); 
//		cv::Mat img = imread("C:\\Users\\ribeiro\\Desktop\\PDI- 2017\\Imagenes\\estanbul.tif", CV_LOAD_IMAGE_GRAYSCALE); 
//		namedWindow("Original", 1);
//		imshow("Original", img);
//		
//		int tamanio;
//		cout<<"Ingrese tamaño de la mascara: ";
//		cin>>tamanio;
//		float desvio;
//		cout<<"Ingrese desviación: ";
//		cin>>desvio;
//		
//		//FILTROS PROMEDIADO
//		cv::Mat filtros_promediadores;
//		cv::blur(img, filtros_promediadores, cv::Size(tamanio,tamanio)); 
//		
//		namedWindow("FiltroPromediado", 1);
//		imshow("FiltroPromediado", filtros_promediadores);
//		
//		//FILTROS GAUSSIANO
//		cv::Mat filtros_gaussiano;
//		cv::GaussianBlur(img, filtros_gaussiano, cv::Size(tamanio,tamanio),desvio); 		
//		
//		namedWindow("FiltroGaussiano", 1);
//		imshow("FiltroGaussiano", filtros_gaussiano);
//		waitKey(0);
		
		//ejercicio_2_3
		
//		ejercicio_2_3(img, 9);
		
		//ejercicio_3_1
		
//		ejercicio_3_1_y_3_2(3,img,1);
		
	//Ejercicio 4_1
	cv::Mat imagen4 = imread("C:\\Users\\ribeiro\\Desktop\\PDI- 2017\\Imagenes\\moon.jpg", CV_LOAD_IMAGE_GRAYSCALE); 
	cv::Mat resultado, mascara, blurreada;
	
	Mat model = Mat::zeros( 400, 500, CV_8UC(1) ); //crear imagen para copiar tamaño (creo que es al pedo)
	resize(imagen4,imagen4,model.size());
	namedWindow("Original"); //ventana para mostrar la imagen
	imshow("Original", imagen4);
	
	filrado_por_highboost(imagen4,resultado,mascara,blurreada,9, 1);			
	
	resize(resultado,resultado,model.size());
	resize(mascara,mascara,model.size());
	resize(blurreada,blurreada,model.size());	
	
	namedWindow("Blurred"); //ventana para mostrar la imagen
	namedWindow("Mascara"); //ventana para mostrar la imagen
	namedWindow("Resultado"); //ventana para mostrar la imagen	
	
	imshow("Blurred", blurreada);
	imshow("Mascara", mascara);
	imshow("Resultado",resultado);
	waitKey(0);	
		
	return 0;
} 
