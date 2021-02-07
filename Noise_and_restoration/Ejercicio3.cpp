#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"

using namespace cv;

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

//FILTRO ALFA RECORTADO
cv::Mat FiltroAlfaRecortado(cv::Mat img,int msize, int d){
	uchar v[msize*msize];
	cv::Mat result=img.clone();
	float t=1./((msize*msize)-d);
	int a,k,t2=(msize*msize)-1,mside=(msize-1)/2;
	for(int i=mside;i<img.rows-mside;i++)
		for(int j=mside;j<img.cols-mside;j++){
		k=0;
		for(int ii=i-mside;ii<=i+mside;ii++){
			for(int jj=j-mside;jj<=j+mside;jj++){
				v[k++]=img.at<uchar>(ii,jj);
			}
		}
		std::sort(v, v+(msize*msize));
		a=0;
		for(int i=d/2;i<=t2-(d/2);i++)
			a=a+v[i];
		result.at<uchar>(i,j)=floor(t*a);
	}
		return result;
}


int main(int argc, char** argv) {
	
	int media = 0;
	int desvio = 0.1;
	int tam=3;
	int Q=3;
	int cantidad_saco=2;
	
	cv::Mat img_nd= imread("img.tif",1);
	namedWindow("Imagen no degradada",WINDOW_NORMAL);
	imshow("Imagen no degradada",img_nd);
	
	cv::Mat img= imread("img_degradada.tif",1);
	namedWindow("Imagen degradada",WINDOW_NORMAL);
	imshow("Imagen degradada",img);
	
//	cv::Mat img= imread("sangre.jpg",1);
//	namedWindow("Imagen degradada",WINDOW_NORMAL);
//	imshow("Imagen degradada",img);
//	
	
	//FILTRO IMPULSIVO
	cvtColor(img,img,CV_RGB2GRAY);
	cv::Mat img_ruido=contaminarRuidoGaussiano(img,media,desvio);
	img_ruido.convertTo(img_ruido,CV_32F,1./255);
	img_ruido= contaminarRuidoImpulsivo(img_ruido,0.1,1);
	img_ruido.convertTo(img_ruido,CV_8UC1,1.*255);
	namedWindow("Imagen Ruido",WINDOW_NORMAL);
	imshow("Imagen Ruido",img_ruido);
	
	//FILTRO MEDIANA Y FILTRO PUNTO MEDIO (NO ANDA)
	cv::Mat img_limpia=filtro_mediana(img,tam);
	img_limpia=filtroPuntoMedio(img_limpia,tam);
	namedWindow("Imagen Limpia Mediana y Punto Medio",WINDOW_NORMAL);
	imshow("Imagen Limpia Mediana y Punto Medio",img_limpia);
	
	//FILTRO ALFA RECORTADO
	cv::Mat img_limpia2=FiltroAlfaRecortado(img,tam,cantidad_saco);
	namedWindow("Imagen Limpia Alfa Recortado",WINDOW_NORMAL);
	imshow("Imagen Limpia Alfa Recortado",img_limpia2);
	
	waitKey(0);
	return 0;
} 


