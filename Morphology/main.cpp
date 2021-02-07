#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "C:\Users\ribeiro\Desktop\PDI- 2017\PDI-Tps\TP8/pdi_functions.h"
#include "MisFunciones.h"
#include <cstdlib>

using namespace cv;
using namespace misFunciones;

Mat ejercicio_aplicacion_1(){
		
	Mat original = imread("C:\\Users\\ribeiro\\Desktop\\PDI- 2017\\PDI-Tps\\TP8\\Morfologia_imagenes\\Tarjeta.jpeg",CV_LOAD_IMAGE_GRAYSCALE);	
		
	//Porque necesito que sea blanco con fondo negro
	for(int i=0; i<original.rows;i++){
		for(int j=0; j<original.cols;j++){
			if(original.at<uchar>(i,j)<200){
				original.at<uchar>(i,j)=255;
			}else{
				original.at<uchar>(i,j)=0;
			}
		}
	}
	
	namedWindow("Original",1);
	imshow("Original",original);
	waitKey(0);
	
	Mat kernel1 = getStructuringElement(MORPH_RECT,Size(3,2),cvPoint(-1,-1));
	Mat kernel2 = getStructuringElement(MORPH_RECT,Size(2,2),cvPoint(-1,-1));
	Mat kernel3 = getStructuringElement(MORPH_CROSS,Size(3,2),cvPoint(-1,-1));	
	
	//Erosion (porque parece que falta eso para que me de bien (?)  )
	Mat eros1 = erosionar(original,kernel1,1);
//	imshow("Primera Erosion",eros1);
//	waitKey(0);
	Mat dilat1 = dilatar(eros1,kernel2,1);
//	imshow("Primera Dilatacion",dilat1);
//	waitKey(0);			
	Mat filtrado;
	medianBlur(dilat1,filtrado,3);
//	imshow("Filtro Mediana",filtrado);
//	waitKey(0);
	Mat eros2 = erosionar(filtrado,kernel2,1);
	imshow("Segunda Erosion",eros2);
	waitKey(0);

	return eros2;
	
}
Mat ejercicio_aplicacion_2_a(){
		
	Mat original = imread("C:\\Users\\ribeiro\\Desktop\\PDI- 2017\\PDI-Tps\\TP8\\Morfologia_imagenes\\Caracteres.jpeg",CV_LOAD_IMAGE_GRAYSCALE);	
		
	//Porque necesito que sea blanco con fondo negro
	for(int i=0; i<original.rows;i++){
		for(int j=0; j<original.cols;j++){
			if(original.at<uchar>(i,j)<200){
				original.at<uchar>(i,j)=255;
			}else{
				original.at<uchar>(i,j)=0;
			}
		}
	}
	
//	namedWindow("Original",1);
//	imshow("Original",original);
//	waitKey(0);
	
	Mat kernel1 = getStructuringElement(MORPH_RECT,Size(3,2),cvPoint(-1,-1));
	Mat kernel2 = getStructuringElement(MORPH_RECT,Size(2,2),cvPoint(-1,-1));
	Mat kernel3 = getStructuringElement(MORPH_CROSS,Size(3,2),cvPoint(-1,-1));	
	
	//Erosion (porque parece que falta eso para que me de bien (?)  )
	Mat eros1 = erosionar(original,kernel1,1);
//	imshow("Primera Erosion",eros1);
//	waitKey(0);
	Mat dilat1 = dilatar(eros1,kernel2,1);
//	imshow("Primera Dilatacion",dilat1);
//	waitKey(0);			
	Mat filtrado;
	medianBlur(dilat1,filtrado,3);
//	imshow("Filtro Mediana",filtrado);
//	waitKey(0);
	Mat eros2 = erosionar(filtrado,kernel2,1);
	imshow("Segunda Erosion",eros2);
	waitKey(0);

	return eros2;
	
}
Mat ejercicio_aplicacion_2_b(){
		
	Mat original = imread("C:\\Users\\ribeiro\\Desktop\\PDI- 2017\\PDI-Tps\\TP8\\Morfologia_imagenes\\Caracteres.jpeg",CV_LOAD_IMAGE_GRAYSCALE);	
	imshow("original",original);
	waitKey(0);
	
	Mat original_invert = original.clone();
	for(int i=0; i<original.rows;i++){
		for(int j=0; j<original.cols;j++){
			if(original.at<uchar>(i,j)<200){
				original_invert.at<uchar>(i,j)=255;
			}else{
				original_invert.at<uchar>(i,j)=0;
			}
		}
	}

	Mat res;
	Mat kernel=getStructuringElement(MORPH_RECT,Size(3,3),Point(-1,-1));
	res=erosionar(original_invert,kernel,1);
	res=dilatar(res,kernel,1);
	imshow("Letras",res);
	res=original_invert-res;	
	imshow("Simbolos",res);
	waitKey(0);
	
	return res;
	
}



Mat ejercicio_aplicacion_7(){
	
	Mat src = imread("C:\\Users\\ribeiro\\Desktop\\PDI- 2017\\PDI-Tps\\TP8\\Morfologia_imagenes\\Melanoma.jpg",CV_LOAD_IMAGE_GRAYSCALE);	
	
	imshow("Original",src);
	waitKey(0);
//	
	Mat resul,masc,blurr;
	filrado_por_mascara_difusa(src,resul,masc,blurr,13);
	
	//Binarizar	
	Mat threshold_output = src.clone();
	for (int i=0; i<src.rows;i++){
		for (int j=0; j<src.cols;j++){
			if (blurr.at<uchar>(i,j)<160){
				threshold_output.at<uchar>(i,j)=255;
			}else{
				threshold_output.at<uchar>(i,j)=0;
			}
		}
	}
		
	imshow("Binarizada",threshold_output);
	waitKey(0);	
	
	//De aca para abajo -> Internet
	
	// Convex Hull implementation
	Mat src_copy = src.clone();
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
	
	findContours( threshold_output, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );
	
	// Find the convex hull object for each contour
	vector<vector<Point> >hull( contours.size() );
	for( int i = 0; i < contours.size(); i++ )
	{  convexHull( Mat(contours[i]), hull[i], false ); }
	
	// Draw contours + hull results
	RNG rng;
	Mat drawing = Mat::zeros( threshold_output.size(), CV_8UC3 );
	for( int i = 0; i< contours.size(); i++ )
	{
		Scalar color_hull = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
		Scalar color_borde = Scalar( rng.uniform(0, 0), rng.uniform(0,255), rng.uniform(0,255) );
		drawContours( drawing, contours, i, color_borde, 1, 8, vector<Vec4i>(), 0, Point() );
		drawContours( drawing, hull, i, color_hull, 1, 8, vector<Vec4i>(), 0, Point() );
	}
	
	// Show in a window
	namedWindow( "Hull demo", CV_WINDOW_AUTOSIZE );
	imshow( "Hull demo", drawing );
	waitKey(0);
	
	return Mat();
	
}



Mat ejercicio_aplicacion_8(){
	Mat img = imread("C:\\Users\\ribeiro\\Desktop\\PDI- 2017\\PDI-Tps\\TP8\\Morfologia_imagenes\\Cuerpos.jpg", 0);
	imshow("Original",img);
	waitKey(0);
	
	Mat resul,masc,blurr;
	filrado_por_mascara_difusa(img,resul,masc,blurr,13);	
	
	Mat original_invert = blurr.clone();
	for(int i=0; i<img.rows;i++){
		for(int j=0; j<img.cols;j++){
			if(blurr.at<uchar>(i,j)<200){
				original_invert.at<uchar>(i,j)=255;
			}else{
				original_invert.at<uchar>(i,j)=0;
			}
		}
	}
	
	imshow("Binarizada",original_invert);
	waitKey(0);
	
//	We now need an image to store the skeleton and also a temporary image in 
//	order to store intermediate computations in the loop. The skeleton image is 
//	filled with black at the beginning.
	Mat skel(img.size(), CV_8UC1, Scalar(0));
	Mat temp(img.size(), CV_8UC1);
//	We have to declare the structuring element we will use for our 
//	morphological operations, here we use a 3x3 cross-shaped structure 
//	element (i.e. we use 4-connexity).
	Mat element = getStructuringElement(MORPH_CROSS, cv::Size(3, 3));
//	And now the core of the algorithm, the main loop. We need a boolean variable 
//	in order to check if there is at least one pixel remaining. Operations are 
//	done in-place when possible.
	bool done;
	do
	{
		morphologyEx(original_invert, temp, MORPH_OPEN, element);
		bitwise_not(temp, temp);
		bitwise_and(original_invert, temp, temp);
		bitwise_or(skel, temp, skel);
		erode(original_invert, original_invert, element);		
		double max;
		cv::minMaxLoc(original_invert, 0, &max);
		done = (max == 0);
	} while (!done);
//	The use of the minMaxLoc function deserves an explanation. We want to check 
//	if there is still at least one pixel in the image, unfortunately I have not 
//	found a function for this task in OpenCV, therefore I just check if the 
//	maximum value is 0. minMaxLoc stores the minimum value in the second parameter 
//	(ignored if NULL pointer) and the maximum in the third parameter. A short-circuit
//	OR function would be nice for this task.		
//	The loop is over, we have our skeleton, let's display it!
	imshow("Skeleton", skel);
	waitKey(0);
	
	return skel;
	
};

int main(int argc, char** argv) {

//	Mat original = imread("Ejemplo.png",CV_LOAD_IMAGE_GRAYSCALE);	
//	Mat original = imread("Ejemplo2.png",CV_LOAD_IMAGE_GRAYSCALE);	
	
//	Mat res1 = ejercicio_aplicacion_1();
//	Mat res2 = ejercicio_aplicacion_2_a();
//	Mat res3 = ejercicio_aplicacion_2_b();
//	Mat res4 = ejercicio_aplicacion_7();
	Mat res5 = ejercicio_aplicacion_8();
	
	
	
	
	return 0;
}
