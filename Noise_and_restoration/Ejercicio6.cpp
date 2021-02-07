#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "pdi_functions/pdi_functions.h"

using namespace cv;

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

//FILTRO MEDIANA
cv::Mat filtro_mediana(cv::Mat img,int msize){
	uchar v[msize*msize];
	cv::Mat result=img.clone();
	int k,m=((msize*msize)-1)/2,mside=(msize-1)/2;
	for(int i=mside;i<img.rows-mside;i++)
		for(int j=mside;j<img.cols-mside;j++){
		k=0;
		for(int ii=i-mside;ii<=i+mside;ii++){
			for(int jj=j-mside;jj<=j+mside;jj++){
				v[k++]=img.at<uchar>(ii,jj);
			}
		}
		std::sort(v, v+(msize*msize));
		result.at<uchar>(i,j)=v[m];
	}
	return result;
}

//FILTRO PUNTO MEDIO
cv::Mat filtroPuntoMedio(cv::Mat img, int k_size){
	//k_size: Tamaño del kernel --> Debe ser IMPAR O EXPLOTA TODO !!
	//Punto Medio: f(x,y)=1/2*(min+max);	
	int Nrows=img.rows;
	int Ncols=img.cols;
	int R=(k_size-1)/2;
	int Mid=(k_size*k_size-1)/2;
	Mat tmp=img.clone();
	
	double min_,max_;
	Mat S;
	for(int i=R; i<Nrows-R; i++){
		for(int j=R; j<Ncols-R ; j++){			
			S=tmp(Range(i-R,i+R),Range(j-R,j+R));
			minMaxLoc(S,&min_,&max_);
			img.at<uchar>(i,j)= (uchar (int (0.5*(min_+max_))));			
		}		
	}
	return img;
}



int main(int argc, char** argv) {

	cv::Mat img1= imread("FAMILIA_a.jpg",1);
	cvtColor(img1,img1,CV_RGB2GRAY);
	namedWindow("Imagen Original 1",WINDOW_NORMAL);
	imshow("Imagen Original 1",img1);
	
	cv::Mat img2= imread("FAMILIA_b.jpg",1);
	cvtColor(img2,img2,CV_RGB2GRAY);
	namedWindow("Imagen Original 2",WINDOW_NORMAL);
	imshow("Imagen Original 2",img2);
	
	cv::Mat img3= imread("FAMILIA_c.jpg",1);
	cvtColor(img3,img3,CV_RGB2GRAY);
	namedWindow("Imagen Original 3",WINDOW_NORMAL);
	imshow("Imagen Original 3",img3);
	
	//VER QUE TIPO DE RUIDO ES: IMAGEN 1: GAUSEANO IMAGEN 2: UNIFORME IMAGEN 3: IMPULSIVO 
	cv::Mat img_hist1 = pdi::histogram(img1,180,cv::Mat()); 
	cv::Mat img_histograma1=graficar_histograma(img_hist1,180);
	namedWindow("Histograma Imagen 1",WINDOW_NORMAL);
	imshow("Histograma Imagen 1",img_histograma1);
	
	cv::Mat img_hist2 = pdi::histogram(img2,180,cv::Mat()); 
	cv::Mat img_histograma2=graficar_histograma(img_hist2,180);
	namedWindow("Histograma Imagen 2",WINDOW_NORMAL);
	imshow("Histograma Imagen 2",img_histograma2);
	
	cv::Mat img_hist3 = pdi::histogram(img3,180,cv::Mat()); 
	cv::Mat img_histograma3=graficar_histograma(img_hist3,180);
	namedWindow("Histograma Imagen 3",WINDOW_NORMAL);
	imshow("Histograma Imagen 3",img_histograma3);
	
	//RESTAURAR IMAGENES
	
	cv::Mat img_limpiar1=filtroPuntoMedio(img1,3);
	namedWindow("Imagen Limpia 1",WINDOW_NORMAL);
	imshow("Imagen Limpia 1",img_limpiar1);
	
	cv::Mat img_limpiar2=filtroPuntoMedio(img2,3);
	namedWindow("Imagen Limpia 2",WINDOW_NORMAL);
	imshow("Imagen Limpia 2",img_limpiar2);
	
	cv::Mat img_limpiar3=filtro_mediana(img3,3);
	namedWindow("Imagen Limpia 3",WINDOW_NORMAL);
	imshow("Imagen Limpia 3",img_limpiar3);

	waitKey(0);
	return 0;
} 


