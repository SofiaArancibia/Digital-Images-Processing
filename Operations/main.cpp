#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <vector>
#include "../pdi_functions-master/pdi_functions.h"
#include <opencv2/core.hpp>


using namespace cv;
using namespace std;

//r -> vlor de entrada
//a-> factor de ganancia
//c-> offset
//s-> salida



//LUT lineal
vector<int> lut(int a, int c){	
	
	vector<int> lut(256);
	
	for (int i=0; i<256; i++){
		int value = a*i + c;
		
		if (value > 255){
			value = 255;	
		}
		else{
			if (value < 0){
				value = 0;
			}
		}
		
		lut[i] = value;
		
	}
	
	return lut;	
}



// LUT para negativo de la imagen
vector<int> negativo(){	
	
	vector<int> lut(256);
	
	for (int i=0; i<256; i++){
		int value = -1*i;				
		lut[i] = value;
		
	}
	
	return lut;	
}




//LUT por tramos lineales
//Llamada -> tramos_lineales( [0, 100, 150, 255], [ [1,1],[1,0],[10,3] ] );
vector<int> tramos_lineales(vector<int> tramos, vector<vector<int> > constantes){	
	
	vector<int> lut(256);
	
	int raices = tramos.capacity();
	
	for (int r=0; r<raices-1; r++){
		
		vector<int> tmp_consts = constantes[r];
		for (int i = tramos[r]; i<tramos[r+1]+1; i++){				
			
			int value = tmp_consts[0]*i + tmp_consts[1];
			lut[i] = value;
			
		}	
		
	}
	return lut;
	
}

//Transformaciones no lineales
//Transformacion logaritmica s=log(1+r)
vector<int> logaritmo(){
	vector<int> lut(256);
	
	for (int i=0; i<256; i++){
		int value = log(1+i);				
		lut[i] = value;
		
	}
	
	return lut;	
}

//Transformacion de potencia s=r^g (c=1)
vector<int> potencia(int gamma){
	vector<int> lut(256);
	
	for (int i=0; i<256; i++){
		int value = i^gamma;				
		lut[i] = value;
		
	}
	
	return lut;	
}



//Operaciones aritmeticas

Mat suma(Mat img1, Mat img2){

	int img1_filas = img1.rows;
	int img1_columnas = img1.cols;
		
			for (int i1=0; i1<img1_filas; i1++){
				for (int j1=0; j1<img1_columnas; j1++){
							img1.at<uchar>(i1,j1)= (img1.at<uchar>(i1,j1) + img2.at<uchar>(i1,j1))/2; // ->  /2;
				}
			}
	
	return img1;
	
}


Mat resta(Mat img1, Mat img2){
	
	int img1_filas = img1.rows;
	int img1_columnas = img1.cols;
	
	for (int i1=0; i1<img1_filas; i1++){
		for (int j1=0; j1<img1_columnas; j1++){
			img1.at<uchar>(i1,j1)= (img1.at<uchar>(i1,j1) - img2.at<uchar>(i1,j1) + 255)/2;
		}
	}
	
	return img1 ;
	
}

Mat producto_mascara(Mat img1, Mat img2){
	
	int img1_filas = img1.rows;
	int img1_columnas = img1.cols;
	
	for (int i1=0; i1<img1_filas; i1++){
		for (int j1=0; j1<img1_columnas; j1++){
			img1.at<uchar>(i1,j1)= img1.at<uchar>(i1,j1) * img2.at<uchar>(i1,j1);
		}
	}
	
	return img1 ;
	
}

Mat division(Mat img1, Mat img2){
	
	int img1_filas = img1.rows;
	int img1_columnas = img1.cols;
	
	for (int i1=0; i1<img1_filas; i1++){
		for (int j1=0; j1<img1_columnas; j1++){
			img1.at<uchar>(i1,j1)= img1.at<uchar>(i1,j1) * (255 - img2.at<uchar>(i1,j1)) / 255;
		}
	}
	
	return img1 ;
	
}


Mat ensuciar(Mat img){	
	
	img.convertTo(img, CV_64F);	
	img = img/255.0;  //normalizada
	
	cv::Mat ruidosa(img.size(),CV_64F);
	cv::randn(ruidosa,0,0.05);
//	std::cout<<"Ruido"<<ruidosa;  //valores negativos por la media cero
	
	Mat sucia = (img+ruidosa);//divido por dos por sumar dos imagenes

//	namedWindow("Ruido",1);
//	imshow("Ruido",ruidosa);

	sucia*=255;
	
	sucia.convertTo(sucia,CV_8UC1);	
	
	return sucia;		
}


cv::Mat limpiar(vector<cv::Mat> vsucias){	
	
	cv::Mat accum=vsucias[0];
	accum.convertTo(accum,CV_64F);	
	
	for (int i=1; i<vsucias.size(); i++){		
		vsucias[i].convertTo(vsucias[i],CV_64F);
		accum+=vsucias[i];		
	}
	
	accum = accum / vsucias.size();
	
	accum.convertTo(accum,CV_8UC1);	
	
	return accum;
}


vector<int> convertir_binario(int pixel){
	
	int val = pixel;	
	vector<int>p;	
	
	for (int i=7;i>=0;i--){
		if ( val< pow(2,i+1) and val >= pow(2,i)) {
			p.push_back(1);
			val = val - pow(2,i);
		}else{
			p.push_back(0);
		}
	}
	
	return p;
	
}


vector<cv::Mat> separar_plano_de_bits(cv::Mat imag){
	
	int row=imag.rows; int col=imag.cols;
	vector<int> vbinario;
	
	vector<cv::Mat> planos;
	
	for (int p=0; p<8; p++){
		planos.push_back(cv::Mat::zeros(row,col,CV_8UC1));
	}		
	
	for (int r=0; r<row;r++){
		for (int c=0; c<col;c++){
			vbinario=convertir_binario((int)imag.at<uchar>(r,c));	
//			std::cout<<"Valor: "<<(int)imag.at<uchar>(r,c)<<"En binario: ";
			for (int i=0; i<vbinario.size(); i++){
//					cout<<vbinario[i]<<endl;
					planos[i].at<uchar>(r,c)=255*vbinario[i];				
			}
			
		}
	}
	
	return planos;
}


	



cv::Mat reconstruir_a_partir_de_planos_de_bits(vector<cv::Mat> planos_de_bit){
	///La reconstruccion se hace multiplicando los pixeles del plano n  por la constante 2^(n-1)
	
	int cantidad_de_planos = planos_de_bit.size();
	int filas = planos_de_bit[0].rows;
	int columnas = planos_de_bit[0].cols;
	cv::Mat reconstruccion = 0*planos_de_bit[0];	
	cv::Mat temporal = 0*planos_de_bit[0];	
	int contador_hacia_atras = cantidad_de_planos;
	
	for (int plano=0; plano<cantidad_de_planos; plano++){ //recorro los planos_de_bit				
		for (int c=0; c<columnas; c++){ //recorro las columnas
			for (int f=0; f<filas; f++){ //recorro las filas
			
				temporal.at<uchar>(f,c) = (((int)planos_de_bit[plano].at<uchar>(f,c))/255)*pow(2,contador_hacia_atras-plano-1);			
				
			}
		}
		namedWindow("Temp",1);
		imshow("Temp",temporal);
		waitKey(0);
		
//		pdi::stats(temporal);
		
		reconstruccion += temporal;
		pdi::stats(reconstruccion);
		
	}
	
	return reconstruccion;	
}




void resaltar_zonas_ocultas(cv::Mat img, cv::Point esq_sup_izq, cv::Point esq_inf_der)
{

	//Dependiendo del ejercicio cambias los intervalos y las constantes
	//De lo demas no hay que tocar nada
	
	//armo LUT de tramos lineales-------------
	vector<int> intervalos;
	vector< vector<int> >constantes;	
	intervalos.push_back(0);
	intervalos.push_back(40);
	intervalos.push_back(255);	
	vector<int> p1; p1.push_back(50); p1.push_back(0);
	vector<int> p2; p2.push_back(1); p2.push_back(0);		
	constantes.push_back(p1);
	constantes.push_back(p2);	
	vector<int> mytLUT = tramos_lineales(intervalos,constantes);
	//----------------------------------------
		
	for (int i=esq_sup_izq.x; i<esq_inf_der.x; i++){
		for (int j=esq_sup_izq.y; j<esq_inf_der.y; j++){
			int img_value =img.at<uchar>(i,j); 
//			cout<<"img_value: "<<img_value<<endl;
			img.at<uchar>(i,j) = mytLUT[img_value];				
//			cout<<"img.at<uchar>(i,j): "<<(int)img.at<uchar>(i,j)<<endl;
		}
	}
	
		
	namedWindow("Lineal por tramos", 1);
	imshow("Lineal por tramos",img);
	waitKey(0);
	
	
	return;
}



int main(int argc, char** argv) {
	
//	namedWindow("Original1",1);
//	Mat imagen1 = imread("C:\\Users\\ribeiro\\Desktop\\PDI- 2017\\Imagenes\\chairs.jpg", CV_LOAD_IMAGE_GRAYSCALE);
//	imshow("Original1", imagen1);
//	waitKey(0);
//	
//	namedWindow("Original2",1);
//	Mat imagen2 = imread("C:\\Users\\ribeiro\\Desktop\\PDI- 2017\\Imagenes\\chairs_oscura.jpg", CV_LOAD_IMAGE_GRAYSCALE);
//	imshow("Original2", imagen2);
//	waitKey(0);
	
	//Imagen pra ensuciar y limpiar
	
//	Mat imagen3 = imread("C:\\Users\\ribeiro\\Desktop\\PDI- 2017\\Imagenes\\earth.bmp", CV_LOAD_IMAGE_GRAYSCALE);
//	namedWindow("Original3",1);
//	imshow("Original3", imagen3);
//	waitKey(0);
//	
//	namedWindow("Sucia",1);
//	Mat sucia = ensuciar(imagen3);	
//	imshow("Sucia", sucia);
//	waitKey(0);
	
//	vector<cv::Mat> imagenes_sucias;
//	cv::Mat tmp_sucia;	
//	
//	for (int i=0; i<50; i++){
//		tmp_sucia = ensuciar(imagen3);
//		imagenes_sucias.push_back(tmp_sucia);
//	}	
//	
//	cv::Mat limpia = limpiar(imagenes_sucias);	
//	
//	vector<cv::Mat> planos_de_bits = separar_plano_de_bits(limpia);
	
//	namedWindow("Plano de bits"); //ventana para mostrar la imagen
//	imshow("Plano de bits", planos_de_bits[0]);
//	waitKey(0);
	
//	cout<<planos_de_bits[0];
	
//	std::cout<<"Planos de bits: "<<planos_de_bits.size();
//	
//	planos_de_bits.push_back(limpia); //agrego al vector la imagen original asi me las grafica todas juntas
	
//	Mat model = Mat::zeros( 200, 400, CV_8UC(1) ); //crear imagen para copiar tamaño (creo que es al pedo)
//	for (int i=0; i<planos_de_bits.size(); i++){
//		resize(planos_de_bits[i],planos_de_bits[i],model.size());
//	}	
//
//	cv::Mat image_array = pdi::mosaic(planos_de_bits,3);
//	namedWindow("Image Array"); //ventana para mostrar la imagen
//	imshow("Image Array", image_array);
//	waitKey(0);
	
//	cv::Mat reconstruida = reconstruir_a_partir_de_planos_de_bits(planos_de_bits);
//	
//	namedWindow("Recontruida"); //ventana para mostrar la imagen
//	imshow("Recontruida", reconstruida);
//	waitKey(0);
	
	//Ejercicio de aplicacion: earth.bmp
//	int filas = imagen3.rows;
//	int columnas = imagen3.cols;	
//	cv::Point p1(0,columnas/2);
//	cv::Point p2(filas/2,columnas);	
//	resaltar_zonas_ocultas(imagen3,p1,p2);
	//--------------------------------------
	
	//Ejercicio de aplicacion: placas madre
	Mat placa1 = imread("C:\\Users\\ribeiro\\Desktop\\PDI- 2017\\Imagenes\\a7v600-SE(RImpulsivo).jpg", CV_LOAD_IMAGE_GRAYSCALE);
	Mat placa2 = imread("C:\\Users\\ribeiro\\Desktop\\PDI- 2017\\Imagenes\\a7v600-SE.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	Mat placa3 = imread("C:\\Users\\ribeiro\\Desktop\\PDI- 2017\\Imagenes\\a7v600-X(RImpulsivo).jpg", CV_LOAD_IMAGE_GRAYSCALE);
	Mat placa4 = imread("C:\\Users\\ribeiro\\Desktop\\PDI- 2017\\Imagenes\\a7v600-X.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	
	namedWindow("Placa"); //ventana para mostrar la imagen
	imshow("Placa", placa2);
	waitKey(0);
	
	
	

	//--------------------------------------
	
	return 0;
} 
