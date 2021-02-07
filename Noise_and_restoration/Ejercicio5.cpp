#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "pdi_functions/pdi_functions.h"

using namespace cv;

cv::Mat invertirMatrizCompleja(cv::Mat img){
	complex<float> nro;
	Mat rta;
	img.copyTo(rta);
	float tol = 0.0001;
	for (int i=0 ; i<img.rows ; i++){
		for (int j=0 ; j<img.cols ; j++){
			nro = img.at<complex<float> >(i,j);
			if (norm(nro)>tol)rta.at<complex<float> >(i,j) = 1.f/nro;
			else rta.at<complex<float> >(i,j) = 0;
		}
	}
	return rta;
}

int main(int argc, char** argv) {
	
	cv::Mat img= imread("huang3.tif",CV_LOAD_IMAGE_GRAYSCALE);
	namedWindow("Imagen Original",WINDOW_NORMAL);
	imshow("Imagen Original",img);
	
	cv::Mat img_m= imread("huang3_movida.tif",CV_LOAD_IMAGE_GRAYSCALE);
	namedWindow("Imagen Movida",WINDOW_NORMAL);
	imshow("Imagen Movida",img_m);
	
	img_m.convertTo(img_m,CV_32F,1./255);
	img.convertTo(img,CV_32F,1./255);
	
	Mat motion = pdi::motion_blur(img_m.size(),10,0);
	motion = invertirMatrizCompleja(motion);
	dft(img_m,tf,DFT_COMPLEX_OUTPUT);
	
	Mat motionada,filtrada;
	mulSpectrums(tf,motion,motionada,DFT_ROWS);
	
	idft(motionada,filtrada,DFT_REAL_OUTPUT|DFT_SCALE);
	normalize(filtrada,filtrada,0,1,CV_MINMAX);
	imshow("Filtrada",filtrada);
	
	waitKey(0);
	return 0;
} 


