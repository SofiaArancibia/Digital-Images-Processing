#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <iostream>

using namespace cv;
using namespace std;

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

//RUIDO GAUSSIANO
cv::Mat contaminarRuidoGaussiano(cv::Mat img, int media, int desvio){
	Mat img_ruidogauss = Mat::zeros(img.rows,img.cols, CV_8UC1);
	randn(img_ruidogauss,media,desvio);
	return sumaRuido(img,img_ruidogauss);	
}

//FILTRO MEDIA GEOMETRICA
cv::Mat filtroMediaGeometrica(cv::Mat img, int msize){
	double acum;
	cv::Mat result=img.clone();
	int mside=(msize-1)/2;
	for(int i=mside;i<img.rows-mside;i++)
		for(int j=mside;j<img.cols-mside;j++){
		acum=1;
		for(int ii=i-mside;ii<=i+mside;ii++){
			for(int jj=j-mside;jj<=j+mside;jj++){
				acum*=img.at<uchar>(ii,jj);
			}
		}
		acum=pow(acum,1./(msize*msize));
		result.at<uchar>(i,j)=acum;
	}
	return result;
}

//FILTRO CONTRA ARMONICA
cv::Mat filtroMediaContraArmonica(cv::Mat img, float Q, int msize){
	//k_size: Tamaño del kernel --> Debe ser IMPAR O EXPLOTA TODO !!
	//Q>0 -> Elimina Pimienta
	//Q<0 -> Elimina Sal
	//Q=0 -> Media Aritmetica
	//Q=-1 -> Media Armonica (Bueno para Sal, Malo para pimienta, Bien para ruido Gaussiano)
	float acum1,acum2;
	cv::Mat result=img.clone();
	int mside=(msize-1)/2;
	for(int i=mside;i<img.rows-mside;i++)
		for(int j=mside;j<img.cols-mside;j++){
		acum1=0;acum2=0;
		for(int ii=i-mside;ii<=i+mside;ii++){
			for(int jj=j-mside;jj<=j+mside;jj++){
				acum1+=pow(img.at<uchar>(ii,jj),Q+1);
				acum2+=pow(img.at<uchar>(ii,jj),Q);
			}
		}
		result.at<uchar>(i,j)=acum1/acum2;
	}
	return result;
}

int main(int argc, char** argv) {

	int media = 0;
	int desvio = 0.1;
	int tam=3;
	int Q=3;
	
	cv::Mat img= imread("sangre.jpg",1);
	namedWindow("Imagen",WINDOW_NORMAL);
	imshow("Imagen",img);
	
	//FILTRO IMPULSIVO + FILTRO EDIA GEOMETRICA
	//CONTAMINO RUIDO GAUSSIANO
	cvtColor(img,img,CV_RGB2GRAY);

	cv::Mat img_ruidog=contaminarRuidoGaussiano(img,media,desvio);
	namedWindow("Imagen Ruido Gaussiano",WINDOW_NORMAL);
	imshow("Imagen Ruido Gaussiano",img_ruidog);
	
	//FILTRO MEDIA GEOMETRICA
	cv::Mat img_limpia_media_geometrica=filtroMediaGeometrica(img_ruidog,tam);
	namedWindow("Imagen Limpia Media Geometrica",WINDOW_NORMAL);
	imshow("Imagen Limpia Media Geometrica",img_limpia_media_geometrica);
	
	//FILTRO IMPULSIVO + FILTRO MEDIACONTRA ARMONICA
	//FILTRO IMPULSIVO
	img.convertTo(img,CV_32F,1./255);
	cv::Mat img_ruidoi= contaminarRuidoImpulsivo(img,0.1,1);
	img_ruidoi.convertTo(img_ruidoi,CV_8UC1,1.*255);
	namedWindow("Imagen Ruido Impulsivo",WINDOW_NORMAL);
	imshow("Imagen Ruido Impulsivo",img_ruidoi);
	
	//FILTRO MEDIA CONTRA ARMONICA
	cv::Mat img_limpiar_media_contra_armonica=filtroMediaContraArmonica(img_ruidoi,Q,tam);
	namedWindow("Imagen Limpia Media Contra Armonica",WINDOW_NORMAL);
	imshow("Imagen Limpia Media Contra Armonica",img_limpiar_media_contra_armonica);
	
	//LO QUE PIDE EL EJERCICIO: SIEMPRE FILTRAR PRIMERO EL RUIDO IMPULSIVO Y LUEGO EL GAUSSIANO
	cv::Mat img_ruido=contaminarRuidoGaussiano(img,media,desvio);
	img_ruido.convertTo(img_ruido,CV_32F,1./255);
	img_ruido= contaminarRuidoImpulsivo(img_ruido,0.1,1);
	img_ruido.convertTo(img_ruido,CV_8UC1,1.*255);
	
	namedWindow("Imagen Ruido TODO",WINDOW_NORMAL);
	imshow("Imagen Ruido TODO",img_ruido);
	cv::Mat img_limpia=filtroMediaContraArmonica(img_ruido,Q,tam);
	img_limpia=filtroMediaGeometrica(img_limpia,tam);

	namedWindow("Imagen limpia TODO",WINDOW_NORMAL);
	imshow("Imagen limpia TODO",img_limpia);
	
	waitKey(0);
	return 0;
} 


