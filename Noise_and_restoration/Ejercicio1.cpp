#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "pdi_functions/pdi_functions.h"
#include <iostream>

using namespace cv;
using namespace std;

cv::Mat graficar_histograma(cv::Mat histograma, int niveles){ 
	double maxVal;		
	minMaxLoc(histograma,NULL, &maxVal, NULL,NULL);
	
	int hist_w = 500; int hist_h = 400;
	int bin_w = cvRound( (double) hist_w/niveles );
	
	Mat imagen_del_histograma( hist_h, hist_w, CV_8UC1, Scalar( 255,255,255) ); //output
	
	//Normalize the result to [ 0, imagen_del_histograma.rows ]
	normalize(histograma, histograma, 0, imagen_del_histograma.rows, NORM_MINMAX, -1, Mat() );
	
	for( int i = 1; i < niveles; i++ )
	{
		line( imagen_del_histograma, Point( bin_w*(i-1), hist_h - cvRound(histograma.at<float>(i-1)) ) ,
			 Point( bin_w*(i), hist_h - cvRound(histograma.at<float>(i)) ),
			 Scalar( 0, 0, 0), 1, 8, 0  );
	}
	
	return imagen_del_histograma;		
}


//RUIDO IMPULSIVO
cv::Mat contaminarRuidoImpulsivo(Mat img, float prob_ocurr, int tipo){
	// tipo=0->sal y pimienta	tipo=1->sal		tipo=2->pimienta
	
	int prob = prob_ocurr*100;
	for (int i=0 ; i<img.cols ; i++){
		for(int j=0 ; j<img.rows ; j++){
			if (rand()%100<prob){
				switch (tipo){
				case 0: {
					if (rand()%2==0)
						img.at<int>(j,i) = 255; //sal
					else 
						img.at<int>(j,i) = 0; //pimienta
					break;}
				case 1: img.at<int>(j,i) = 255; break; //sal
				case 2: img.at<int>(j,i) = 0; break; //pimienta
				}
			}
		}
	}
	
	return img;
}

//SUMA RUIDO
Mat sumaRuido(Mat a, Mat b){
	Mat rta = Mat::zeros(a.rows,b.cols,CV_8UC1);
	for (int i=0; i<a.cols; i++){
		for (int j=0; j<a.rows; j++){
			int valor = (a.at<uchar>(j,i) + (b.at<uchar>(j,i)-127) );
			if (valor<0) rta.at<uchar>(j,i)=0;
			else if (valor>255) rta.at<uchar>(j,i)=255;
			else rta.at<uchar>(j,i)=valor;
		}
	}
	return rta;
}

//RUIDO UNIFORME
cv::Mat contaminarRuidoUniforme(cv::Mat img, int media, int desvio,int desvio_u){
	//img.convertTo(img,CV_32F,1./255);
	cv::Mat img_ruidouniforme = Mat::zeros(img.rows,img.cols, CV_8UC1);
	for (int i=0 ; i<img.rows ; i++){
		for (int j=0 ; j<img.cols ; j++){
			img_ruidouniforme.at<uchar>(i,j) = (media-desvio_u/2) + rand()%desvio_u;
		}
	}
	//img_ruidouniforme.convertTo(img_ruidouniforme,CV_8UC1,1.*255);
	return sumaRuido(img,img_ruidouniforme);
}

//RUIDO GAUSSIANO
cv::Mat contaminarRuidoGaussiano(cv::Mat img, int media, int desvio){
	Mat img_ruidogauss = Mat::zeros(img.rows,img.cols, CV_8UC1);
	randn(img_ruidogauss,media,desvio);
	return sumaRuido(img,img_ruidogauss);	
}

int main(int argc, char** argv) {

	cv::Mat img = imread("circulo.png",1);
	cvtColor(img,img,CV_RGB2GRAY);
	
	namedWindow("Imagen",WINDOW_NORMAL);
	imshow("Imagen",img);
	
	cv::Mat img_hist = pdi::histogram(img,180,cv::Mat()); 
	cv::Mat img_histograma=graficar_histograma(img_hist,180);
	namedWindow("Histograma Imagen",WINDOW_NORMAL);
	imshow("Histograma Imagen",img_histograma);
	
	//RUIDO IMPULSIVO
//	img.convertTo(img,CV_32F,1./255);
//	cv::Mat img_ruidoimpulsivo = contaminarRuidoImpulsivo(img,0.5,0);
//	img_ruidoimpulsivo.convertTo(img_ruidoimpulsivo,CV_8UC1,1.*255);
//	
//	namedWindow("Imagen Ruido Impulsivo",WINDOW_NORMAL);
//	imshow("Imagen Ruido Impulsivo",img_ruidoimpulsivo);
//	
//	cv::Mat img_hist2 = pdi::histogram(img_ruidoimpulsivo,180,cv::Mat()); 
//	cv::Mat histograma_impulsivo=graficar_histograma(img_hist2,180);
//	namedWindow("Histograma Ruido Impulsivo",WINDOW_NORMAL);
//	imshow("Histograma Ruido Impulsivo",histograma_impulsivo);
	
	//RUIDO UNIFORME
//	int media = 127, desvio = 5, desvio_u = 20;
//	cv::Mat img_ruidouniforme=contaminarRuidoUniforme(img,media,desvio,desvio_u);
//	
//	namedWindow("Imagen Ruido Uniforme",WINDOW_NORMAL);
//	imshow("Imagen Ruido Uniforme",img_ruidouniforme);
//	
//	cv::Mat img_hist3 = pdi::histogram(img_ruidouniforme,256,cv::Mat()); 
//	cv::Mat histograma_uniforme=graficar_histograma(img_hist3,256);
//	namedWindow("Histograma Ruido Uniforme",WINDOW_NORMAL);
//	imshow("Histograma Ruido Uniforme",histograma_uniforme);
	
	//RUIDO GAUSSIANO
//	int media = 127, desvio = 5;
//	cv::Mat img_ruidogauss=contaminarRuidoGaussiano(img,media,desvio);
//	
//	namedWindow("Imagen Ruido Gaussiano",WINDOW_NORMAL);
//	imshow("Imagen Ruido Gaussiano",img_ruidogauss);
//	
//	cv::Mat img_hist3 = pdi::histogram(img_ruidogauss,256,cv::Mat()); 
//	cv::Mat histograma_gauss=graficar_histograma(img_hist3,256);
//	namedWindow("Histograma Ruido Uniforme",WINDOW_NORMAL);
//	imshow("Histograma Ruido Uniforme",histograma_gauss);
	
	waitKey(0);
	return 0;
} 


