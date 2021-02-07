#ifndef MISFUNCIONES_H
#define MISFUNCIONES_H

#include <iostream>
#include <opencv2/opencv.hpp>
#include <vector>
#include <opencv2/highgui.hpp>
#include "C:\Users\ribeiro\Desktop\PDI- 2017\PDI-Tps\TP8/pdi_functions.h"
#include <string>
#include <cstdio>
#include <sstream>

using namespace std;
using namespace cv;
using namespace pdi;


namespace misFunciones{		
	
	
	struct rectangulo{
		int derecha;
		int izquierda;
		int arriba;
		int abajo;
		float llenado; //para el ejer de las botellas. Borrar para cualquier otra cosa :B
	};
	
	//Capturar click del mouse
	void capturar_intensidades_con_click(int event, int x, int y,int flags, void* userdata)
	{
		if  ( event == CV_EVENT_LBUTTONDOWN )
		{
			
			cv::Mat *my_objetive = (cv::Mat*)userdata;
			cv::Mat image = *my_objetive;

			std::cout << "Left button of the mouse is clicked - position (" << x << ", " << y << ")" << std::endl;
			std::cout << "Intensity value at (" << x << ", " << y << ") is "<<(int)image.at<uchar>(x,y)<<std::endl;
		}
		else if  ( event == CV_EVENT_RBUTTONDOWN )
		{
			exit(0);
			
		}
		
	}
	
	
	///RECORTAR UNA IMAGEN
	cv::Point my_init_point, my_end_point;
	void capturar_ROI(int event, int x, int y,int flags, void* userdata)
	{
		
		cv::Mat *my_objetive = (cv::Mat*)userdata;
		cv::Mat image = *my_objetive;
		
		if (event == CV_EVENT_LBUTTONDOWN ){
		//click con boton izquierdo determina esquina superior izquierda
			my_init_point = cvPoint(x, y);
		}
		if (event == CV_EVENT_LBUTTONUP){ //soltar boton izquiero determina esquina inferior derecha
			my_end_point = cvPoint(x, y);
		}
		
//		///CROP the Image
//		cv::Rect myROI(my_init_point, my_end_point);
//		cv::Mat croppedImage = image(myROI); 
//		
//		///Guardo el recorte
//		std::vector<int> compression_params;
//		compression_params.push_back(CV_IMWRITE_JPEG_QUALITY);
//		imwrite("C:\\Users\\ribeiro\\Desktop\\PDI- 2017\\Imagenes\\recorte.jpg", croppedImage,compression_params );		
		
	}
	
	void PerfilDeIntensidad(cv::Mat imagen,std::string segmento,bool graficar, std::vector<int> &intensidades)
	{
	///Param
	//imagen -> imagen de referencia
	//segmento -> puede ser una fila o una columna (�nicamente, por ahora). Ej.: f100 o c9 (fila 100 o columna 8)
	//graficar -> indica si se quiere o no graficar el perfil de intensidad
	
	//1) Identificar si quiere fila o columna
		
	char tipo = segmento[0];
	int posicion = atoi((segmento.substr(1,segmento.size())).c_str());
//	std::cout<<"Tipo: "<<tipo<<" Posici�n: "<<posicion<<std::endl;
	if (tipo == 'f'){
		for (int j=0; j<imagen.cols; j++){
			intensidades.push_back((int)imagen.at<uchar>(posicion,j));
		}		
	}
	else if (tipo == 'c'){
		for (int j=0; j<imagen.rows; j++){
			intensidades.push_back((int)imagen.at<uchar>(j,posicion));
		}
	}
	else{
		printf("Par�metro %s mal ingresado!",segmento);
	}
//	std::cout<<"Cantidad de intesidades capturadas: "<<intensidades.size()<<std::endl;
	
	//falta agregar la grafica (si se hace o no depende del parametro 'graficar')
	
	
	
	
	return;
	}
	
	
	
	
	///NIVEL DE LLENADO DE BOTELLAS
	void nivel_de_llenado_botella(cv::Mat image)
	{		
		cv::Mat original = image.clone();
		cv::Mat clon = image.clone();
		
		std::vector<rectangulo> botellas; //aca voy a guardar los rectangulo que corresponden a las ubicaciones de botellas
		
		//tama�o de la imagen de entrada
		int filas = original.rows; 
		int columnas = original.cols;
		
		//binarizo (para buscar botellas)
		for (int i=0; i<filas; i++){
			for (int j=0; j<columnas; j++){
				if ((int)image.at<uchar>(i,j)>0){
					image.at<uchar>(i,j) = 255;
				}else{
					image.at<uchar>(i,j) = 0;
				}
	
			}
		}
		
		int medio = filas/2; //mitad de la imagen de entrada (por aca voy a "tirar una linea" para buscar saltos)
		
		std::stringstream tmp_medio;
		tmp_medio << medio;
		std::string posicion = tmp_medio.str();
		std::string segmento = "f"+posicion;
		
		std::vector<int> intensidades;
		
		PerfilDeIntensidad(image,segmento,false, intensidades); //perfil de intensidades de la imagen binarizada
		
		std::vector<int> bordes_derechos; //posicion 
		std::vector<int> bordes_izquierdos; //posicion 
		std::vector<int> orden_bordes; //0 -> borde izquierdo //1 -> borde derecho
		
		int borde_izquierdo_imagen=0;
		int borde_derecho_imagen=columnas;
		
		for (int i=1; i<intensidades.size()-1; i++){
			if (intensidades[i]==255 && intensidades[i-1]==255 && intensidades[i+1]==0){ //salto de blanco a negro -> bordes_derechos
				bordes_derechos.push_back(i);
				orden_bordes.push_back(1);
				line(image, cv::Point(i,0), cv::Point(i,filas), CV_RGB(255,255,255), 1, CV_AA);
			}
			else{
				if (intensidades[i]==0 && intensidades[i-1]==0 && intensidades[i+1]==255){ //salto de negro a blanco -> bordes_izquierdos
					bordes_izquierdos.push_back(i);					
					orden_bordes.push_back(0);
					line(image, cv::Point(i,0), cv::Point(i,filas), CV_RGB(255,255,255), 1, CV_AA);
				}
			}					
		}

		/// identifico borde superior		
		int borde_superior; //posicion 
		int cant_bordes = orden_bordes.size();				
		bool todas_enteras=false;
		
		if (orden_bordes[0]!=1 && orden_bordes[cant_bordes-1] !=0 ){
			todas_enteras = true;
		}		
		
		std::vector<rectangulo> posiciones_de_botellas;
		
		if (orden_bordes[0] == 1){ //la primera botella esta sobre el borde izquierdo de la imagen de entrada
			
			rectangulo temp;
			
			temp.izquierda=0;
			temp.derecha= bordes_derechos[0];
			
			int medio_v = (temp.izquierda + temp.derecha)/2;
			
			std::vector<int> intensidades_vertical;
			std::stringstream tmp_medio_vert;
			tmp_medio_vert << medio_v;
			std::string posicion = tmp_medio_vert.str();
			std::string segmento = "c"+posicion;
			
			PerfilDeIntensidad(image,segmento,false,intensidades_vertical);
			
			for (int i=0; i<intensidades_vertical.size();i++){
				if (intensidades_vertical[i-1]==0 && intensidades_vertical[i+1]==255){ //salto de negro a blanco -> bordes_izquierdos
					borde_superior=i;					
					break;
				}
			}
			
			temp.arriba = borde_superior;
			temp.abajo = filas;

			botellas.push_back(temp);
			
			bordes_derechos.erase(bordes_derechos.begin()); 
		}
		
		if (orden_bordes[cant_bordes-1] == 0){ //la ultima botella esta sobre el borde derecho de la imagen de entrada
			
			rectangulo temp;
			
			temp.izquierda=bordes_izquierdos[bordes_izquierdos.size()-1];
			temp.derecha= columnas;
			
			int medio_v = (temp.izquierda + temp.derecha)/2;
			
			std::vector<int> intensidades_vertical;
			std::stringstream tmp_medio_vert;
			tmp_medio_vert << medio_v;
			std::string posicion = tmp_medio_vert.str();
			std::string segmento = "c"+posicion;
			
			PerfilDeIntensidad(image,segmento,false,intensidades_vertical);
			
			for (int i=0; i<intensidades_vertical.size();i++){
				if (intensidades_vertical[i-1]==0 && intensidades_vertical[i+1]==255){ //salto de negro a blanco -> bordes_izquierdos
					borde_superior=i;					
					break;
				}
			}
			
			temp.arriba = borde_superior;
			temp.abajo = filas;
			
			botellas.push_back(temp);
			
			bordes_izquierdos.pop_back();
		}
		
		//en los if anteriores se borraron (si correspondia) los valores de borde correspondientes a botellas con un solo borde visible
		//entonces aca estoy recorriendo las botellas que estan completas (es decir, que se le ven los dos bordes laterales)
		int izq = 0;
		for (int d=0; d<bordes_derechos.size(); d++){
				
				rectangulo temp;
				temp.izquierda = bordes_izquierdos[izq];
				temp.derecha = bordes_derechos[d];
				temp.abajo = filas;
				
				int medio_v = (temp.izquierda + temp.derecha)/2;
				
				std::vector<int> intensidades_vertical;
				std::stringstream tmp_medio_vert;
				tmp_medio_vert << medio_v;
				std::string posicion = tmp_medio_vert.str();
				std::string segmento = "c"+posicion;
				
				PerfilDeIntensidad(image,segmento,false,intensidades_vertical);
				
				for (int i=0; i<intensidades_vertical.size();i++){
					if (intensidades_vertical[i]==0 && intensidades_vertical[i-1]==0 && intensidades_vertical[i+1]==255){ //salto de negro a blanco -> bordes_izquierdos
						borde_superior=i;					
						break;
					}
				}
				
				temp.arriba = borde_superior;

				botellas.push_back(temp);
				
				izq++;
		}
		//binarizo de otra forma para separar el l�quido (?
		for (int i=0; i<filas; i++){
			for (int j=0; j<columnas; j++){
				if ((int)clon.at<uchar>(i,j)>0 && (int)clon.at<uchar>(i,j)<170){
					clon.at<uchar>(i,j) = 255;
				}else{
					clon.at<uchar>(i,j) = 0;
				}
				
			}
		}

		std::vector<rectangulo> no_llenas;
		
		int liquido;			
		
		for (int bot=0; bot<botellas.size(); bot++){//recorro el vector de botellas
			
			rectangulo my_bottle = botellas[bot];
			
			int medio = (my_bottle.izquierda + my_bottle.derecha)/2;
			
			for (int f=my_bottle.arriba; f<my_bottle.abajo-2; f++){ //el 2 es para asegurarme que estoy tomando pixel del liquido y no
				//alguno que se pinto pero que no pertenece al liquido
				if ((int)clon.at<uchar>(f+2,medio)==255 && (int)clon.at<uchar>(f-2,medio)==0){
					liquido=f;
						break;
				}
			}

			int altura_botella = my_bottle.abajo - my_bottle.arriba;
			
			int altura_liquido = altura_botella - liquido;
			
			float porcentaje = (altura_liquido*100)/altura_botella;			
			
			if (porcentaje < 70){
				my_bottle.llenado=porcentaje;
				no_llenas.push_back(my_bottle);
			}
		}

		std::cout<<"Cantidad de botellas no llenas: "<<no_llenas.size()<<std::endl;

		for (int nll=0; nll<no_llenas.size(); nll++){
			cv::rectangle(original, cv::Point(no_llenas[nll].izquierda,no_llenas[nll].arriba), cv::Point(no_llenas[nll].derecha, no_llenas[nll].abajo), cv::Scalar(255, 255, 255));
			cv::namedWindow("Botellas No Llenas",1); //ventana para mostrar la imagen
			imshow("Botellas No Llenas", original);
			std::cout<<"Porcentaje de llenado: "<<no_llenas[nll].llenado<<std::endl;
		}
		

		
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
				img1.at<uchar>(i1,j1)= img1.at<uchar>(i1,j1) * img2.at<uchar>(i1,j1)/255;
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
	
	
	Mat ensuciar(Mat img,float media, float desvio ){	
		
		img.convertTo(img, CV_64F);	
		img = img/255.0;  //normalizada
		
		cv::Mat ruidosa(img.size(),CV_64F);
		randn(ruidosa,media,desvio);
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
	

	cv::Mat graficar_histograma(cv::Mat histograma, int niveles){ 
		//niveles es el valor que puse al calcular el histograma en pdi::histogram
		
		double maxVal;		
		minMaxLoc(histograma,NULL, &maxVal, NULL,NULL);
		
		int hist_w = 500; int hist_h = 400;
		int bin_w = cvRound( (double) hist_w/niveles );
		
		Mat imagen_del_histograma( hist_h, hist_w, CV_8UC1, Scalar( 255,255,255) ); //output
		
		/// Normalize the result to [ 0, imagen_del_histograma.rows ]
		normalize(histograma, histograma, 0, imagen_del_histograma.rows, NORM_MINMAX, -1, Mat() );
		
//		void cv::line	(InputOutputArray img,
//						 Point 	pt1,
//						 Point 	pt2,
//						 const Scalar & 	color,
//						 int 	thickness = 1,
//						 int 	lineType = LINE_8,
//						 int 	shift = 0 
//						 )	
//		
	for( int i = 1; i < niveles; i++ )
		{
			line( imagen_del_histograma, Point( bin_w*(i-1), hist_h - cvRound(histograma.at<float>(i-1)) ) ,
				 Point( bin_w*(i), hist_h - cvRound(histograma.at<float>(i)) ),
				 Scalar( 0, 0, 0), 1, 8, 0  );
		}
		
		return imagen_del_histograma;		
	}

	
	cv::Mat ecualizar_histograma(cv::Mat imagen, cv::MatND &histograma_ecualizado){
		//Esta funcion devuelve la imagen final y el histograma correspondiente
		cv::Mat output_image;		
		equalizeHist(imagen,output_image); //ouput image es la imagen final, una vez ecualizado su histograma
		//se puede ver que el constraste mejora significativamente, ya que el rango din�mico es mucho mayor
		//que el de la imagen original			
		histograma_ecualizado = pdi::histogram(output_image,256,cv::Mat()); //from pdi functions			
		return output_image;
	}
	
	

	void ejercicio_2_3(cv::Mat img, int tam){
		
		//FILTROS PROMEDIADO
		cv::Mat filtros_promediadores;
		cv::blur(img, filtros_promediadores, cv::Size(tam,tam)); 
		
		//Binarizar
		for (int i=0; i<filtros_promediadores.rows; i++){
			for (int j=0; j<filtros_promediadores.cols; j++){
				if ((int)filtros_promediadores.at<uchar>(i,j)<100){
					filtros_promediadores.at<uchar>(i,j) = 0;
				}else{
					filtros_promediadores.at<uchar>(i,j) = 255;
				}				
			}
		}
		
		cv::Mat resultado = producto_mascara(img,filtros_promediadores);
		
		namedWindow("Resultado", 1);
		imshow("Resultado", resultado);
		waitKey(0);		
	}

	
	
	
	
	
	

	void ejercicio_3_1_y_3_2(int tam, cv::Mat img, int suma){
		cv::Mat filtro = (-1)*cv::Mat::ones(tam,tam,CV_64F);
		
		int centro = (tam/2);
		
		if (suma==1){
			filtro.at<double>(centro,centro) = (tam*tam);
		}else{
			if(suma==0){
				filtro.at<double>(centro,centro) = (tam*tam)-1;
			}
		}
		
		img.convertTo(img, CV_64F);
		
		cv::Mat resultado = pdi::convolve(img,filtro);
		
		resultado.convertTo(resultado,CV_8UC1);
		
		namedWindow("Resultado", 1);
		imshow("Resultado", resultado);
		waitKey(0);		
		
		//		return resultado;
		
		
	}

	cv::Mat filtro_pasa_alto_RGB(int tam, cv::Mat img, int suma){ //Recibe HSV o RGB
		
		cv::Mat filtro = (-1)*cv::Mat::ones(tam,tam,CV_64F);
		
		int centro = (tam/2);
		
		if (suma==1){
			filtro.at<double>(centro,centro) = (tam*tam);
		}else{
			if(suma==0){
				filtro.at<double>(centro,centro) = (tam*tam)-1;
			}
		}
		
		cv::Mat resultado = pdi::convolve(img,filtro);
	
		return resultado;			
		
	}
	
	cv::Mat filtro_pasa_alto_HSV(int tam, cv::Mat img, int suma){ //Recibe HSV o RGB
		
		cv::Mat filtro = (-1)*cv::Mat::ones(tam,tam,CV_64F);
		
		int centro = (tam/2);
		
		if (suma==1){
			filtro.at<double>(centro,centro) = (tam*tam);
		}else{
			if(suma==0){
				filtro.at<double>(centro,centro) = (tam*tam)-1;
			}
		}
		
		std::vector< cv::Mat > hsv;
		cv::split(img, hsv);
		
		hsv[2] = pdi::convolve(hsv[2],filtro);
		cv::Mat resultado;
		cv::merge(hsv, resultado);	
		
		return resultado;			
		
	}

	void filrado_por_mascara_difusa(cv::Mat img, cv::Mat &res, cv::Mat &mascara,cv::Mat &blurreada, int tamanio){
	
	cv::Mat copia_original1;
	copia_original1 = img.clone();
	
	//1) Blurreo por filtros de promediado
	cv::blur(img, blurreada, cv::Size(tamanio,tamanio)); 
	
	//2) Sustraer la imagen blurreada de la original
	mascara = resta(img, blurreada);
	
	//3) Sumar la m�scara a la imagen original
	res = suma(copia_original1,mascara);

	}

	void filrado_por_highboost(cv::Mat img, cv::Mat &res, cv::Mat &mascara,cv::Mat &blurreada, int tamanio, int A){
		
		//Si A=1 entonces tngo el filtrado por m�scara difusa
		
		cv::Mat copia_original1;
		copia_original1 = img.clone();
		
		//1) Blurreo por filtros de promediado
		cv::blur(img, blurreada, cv::Size(tamanio,tamanio)); 
		
		//2) Sustraer la imagen blurreada de la original
		mascara = resta(img, blurreada);
		
		//3) Sumar la m�scara a la imagen original
		res = suma(copia_original1,A*mascara);
		
	}
	

// Estas dos funciones en rosado las tengo que corregir
	
	cv::Mat crear_imagen_con_linea_horizontal(int filas, int columnas, int pos_x, int grosor){
		
		cv::Mat imagen = cv::Mat::zeros(filas,columnas, CV_8UC1);
		
		for (int i=0; i<columnas; i++){
			imagen.at<uchar>(pos_x,i)=255;
			for (int g=1; g<grosor+1;g++){
				imagen.at<uchar>(pos_x+g,i)=255;
			}
		}
		
		return imagen;		
	}

	cv::Mat crear_imagen_con_linea_vertical(int filas, int columnas, int pos_y, int grosor){
		
		cv::Mat imagen = cv::Mat::zeros(filas,columnas, CV_8UC1);
		
		for (int i=0; i<filas; i++){
			imagen.at<uchar>(i, pos_y)=255;
			for (int g=1; g<grosor+1;g++){
				imagen.at<uchar>(i, pos_y+g)=255;
			}
		}
		
		return imagen;		
	}
	
	
	cv::Mat crear_imagen_con_cuadrado(int filas, int columnas,int centro_x, int centro_y, int lado){
		
		cv::Mat imagen = cv::Mat::zeros(filas,columnas, CV_8UC1);
		
		int arista_der = centro_y + lado/2;
		int arista_izq = centro_y - lado/2;
		int arista_inf = centro_x + lado/2;
		int arista_sup = centro_x - lado/2;
		
		for (int i=arista_izq; i<arista_der+1; i++){
			for (int j=arista_sup; j<arista_inf+1; j++){
				imagen.at<uchar>(j,i)=255;
				imagen.at<uchar>(j,i)=255;
			}
		}
		
		return imagen;		
	}
	
	cv::Mat crear_imagen_con_rectangulo(int filas, int columnas,int centro_x, int centro_y, int ladoH, int ladoB){
		
		cv::Mat imagen = cv::Mat::zeros(filas,columnas, CV_8UC1);
		
		if (ladoH==ladoB){
			imagen = crear_imagen_con_cuadrado(filas,columnas,centro_x,centro_y,ladoH);	
			return imagen;
		}
		
		int arista_der = centro_y + ladoB/2;
		int arista_izq = centro_y - ladoB/2;
		int arista_inf = centro_x + ladoH/2;
		int arista_sup = centro_x - ladoH/2;
		
		for (int i=arista_izq; i<arista_der+1; i++){
			for (int j=arista_sup; j<arista_inf+1; j++){
				imagen.at<uchar>(j,i)=255;
				imagen.at<uchar>(j,i)=255;
			}
		}
		
		return imagen;		
	}
	
	
	cv::Mat crear_imagen_con_circulo(int filas, int columnas,int centro_x, int centro_y, int radio){
		
		cv::Mat imagen = cv::Mat::zeros(filas,columnas, CV_8UC1);

//		cv::Point centro(centro_y,centro_x);
//		cv::Scalar color = 255;
//		circle(imagen, centro, radio, color); //circulo vacio
		
		
//		Ecuacion del circulo = r^2 = x^2 + y^2 				
		for (int i=0; i<filas; i++){
			for (int j=0; j<columnas; j++){				
				if( ((i*i) + (j*j) <= (radio*radio))){
					imagen.at<uchar>(i+centro_x,j+centro_y)=255;
					imagen.at<uchar>(-i+centro_x,j+centro_y)=255;
					imagen.at<uchar>(i+centro_x,-j+centro_y)=255;
					imagen.at<uchar>(-i+centro_x,-j+centro_y)=255;					
				}
				
			}
		}
		
		return imagen;
		
	}
	

	void ejercicio_1_3_guia5(int filas, int columnas){
//	Cree una imagen de 512x512 conteniendo una l�inea vertical blanca centrada
//	de un pixel de ancho sobre un fondo negro. Rote la imagen 20 grados y
//	extraiga una secci�on de 256x256 de la imagen original y de la imagen rotada,
//	de manera que las l�ineas tengan sus extremos en los bordes superior e inferior,
//	sin m�argenes. Visualice la TDF de ambas im�agenes. Explique, utilizando
//	argumentos intuitivos, por qu�e las magnitudes de Fourier aparecen como lo
//	hacen en las im�agenes, y a qu�e se deben las diferencias.
	cv::Mat imagen_con_linea_vertical_centrada = cv::Mat::zeros(filas,columnas, CV_8UC1);
	imagen_con_linea_vertical_centrada = crear_imagen_con_linea_vertical(filas, columnas,columnas/2,1);	
	cv::Mat imagen_con_linea_vertical_centrada_ROTADA = pdi::rotate(imagen_con_linea_vertical_centrada, 20);
	
	///CROP the Images
	cv::Point esq_sup_izq((filas/2)-128, (columnas/2)-128); 
	cv::Point esq_inf_der((filas/2)+128, (columnas/2)+128);	
	cv::Rect myROI(esq_sup_izq, esq_inf_der);
	
	cv::Mat seccion_255x256_de_imagen_con_linea_vertical_centrada = imagen_con_linea_vertical_centrada(myROI);
	cv::Mat seccion_255x256_de_imagen_con_linea_vertical_centrada_ROTADA = imagen_con_linea_vertical_centrada_ROTADA(myROI);
	
	namedWindow("Linea Vertical",1);
	namedWindow("Linea Vertical Rotada",1);
	imshow("Linea Vertical", imagen_con_linea_vertical_centrada);
	imshow("Linea Vertical Rotada", imagen_con_linea_vertical_centrada_ROTADA);
	waitKey(0);
	
	namedWindow("Linea Vertical Seccionada",1);
	namedWindow("Linea Vertical Rotada Seccionada",1);
	imshow("Linea Vertical Seccionada", seccion_255x256_de_imagen_con_linea_vertical_centrada);
	imshow("Linea Vertical Rotada Seccionada", seccion_255x256_de_imagen_con_linea_vertical_centrada_ROTADA);
	waitKey(0);
		
	//TDF
	imagen_con_linea_vertical_centrada.convertTo(imagen_con_linea_vertical_centrada, CV_32F);
	cv::Mat tfd_imagen_con_linea_vertical_centrada = pdi::spectrum(imagen_con_linea_vertical_centrada); //la imagen debe ser 32F
	namedWindow("TFD Linea Vertical",1);
	imshow("TFD Linea Vertical", tfd_imagen_con_linea_vertical_centrada);
	waitKey(0);
	
	//TDF
	imagen_con_linea_vertical_centrada_ROTADA.convertTo(imagen_con_linea_vertical_centrada_ROTADA, CV_32F);
	cv::Mat tfd_imagen_con_linea_vertical_centrada_ROTADA = pdi::spectrum(imagen_con_linea_vertical_centrada_ROTADA); //la imagen debe ser 32F
	namedWindow("TFD Linea Vertical Rotada",1);
	imshow("TFD Linea Vertical Rotada", tfd_imagen_con_linea_vertical_centrada_ROTADA);
	waitKey(0);
	
	//TDF
	seccion_255x256_de_imagen_con_linea_vertical_centrada.convertTo(seccion_255x256_de_imagen_con_linea_vertical_centrada, CV_32F);
	cv::Mat tfd_seccion_255x256_de_imagen_con_linea_vertical_centrada = pdi::spectrum(seccion_255x256_de_imagen_con_linea_vertical_centrada); //la imagen debe ser 32F
	namedWindow("TFD Linea Vertical Seccionada",1);
	imshow("TFD Linea Vertical Seccionada", tfd_seccion_255x256_de_imagen_con_linea_vertical_centrada);
	waitKey(0);
	
	//TDF
	seccion_255x256_de_imagen_con_linea_vertical_centrada_ROTADA.convertTo(seccion_255x256_de_imagen_con_linea_vertical_centrada_ROTADA, CV_32F);
	cv::Mat tfd_seccion_255x256_de_imagen_con_linea_vertical_centrada_ROTADA = pdi::spectrum(seccion_255x256_de_imagen_con_linea_vertical_centrada_ROTADA); //la imagen debe ser 32F
	namedWindow("TFD Linea Vertical Rotada Seccionada",1);
	imshow("TFD Linea Vertical Rotada Seccionada", tfd_seccion_255x256_de_imagen_con_linea_vertical_centrada_ROTADA);
	waitKey(0);
	
	}

	//Preguntar resultados de este ejercicios.. no se si estan bien
	
	void ejercicio_1_4_guia5(cv::Mat imagen){
		
		namedWindow("Imagen",1);
		imshow("Imagen", imagen);
		waitKey(0);		
		imagen.convertTo(imagen, CV_32F);
		cv::Mat tfd_imagen = pdi::spectrum(imagen); //la imagen debe ser 32F				
		namedWindow("TFD imagen",1);
		imshow("TFD imagen", tfd_imagen);
		waitKey(0);
		
	}
	

	
	

cv::Mat segmentacion_RGB(string nombre_ventana, cv::Mat imagen, cv::Mat &mascara){

		cv::Mat imagen_en_grayscale;
		cv::cvtColor(imagen, imagen_en_grayscale, CV_RGB2GRAY);
		
		//(b)
		namedWindow("Escala de grises",1);
		imshow("Escala de grises", imagen_en_grayscale);
		setMouseCallback("Escala de grises", capturar_ROI,(void*)&imagen_en_grayscale);		
		waitKey(0); //Se clickea, sin soltar vas hasta el punto inferior derecho, soltas
		//enter, enter
		
		cv::Rect myROI(my_init_point, my_end_point);
		cv::Mat croppedImage = imagen(myROI); 
		std::vector< cv::Mat > bgrCI, means; 
		cv::split(croppedImage, bgrCI);
			
		//Centro de la esfera - valores medios de cada canal
		cv::Scalar meanB = mean( bgrCI[0] );
		float meanBf = meanB.val[0];
		
		cv::Scalar meanG = mean( bgrCI[1] );
		float meanGf = meanG.val[0];
		
		cv::Scalar meanR = mean( bgrCI[2] );
		float meanRf = meanR.val[0];		
	
		//(c)
		cv::Mat histR = pdi::histogram(bgrCI[2],256,cv::Mat()); //histograma del canal R
		cv::Mat histG = pdi::histogram(bgrCI[1],256,cv::Mat()); //histograma del canal B
		cv::Mat histB = pdi::histogram(bgrCI[0],256,cv::Mat()); //histograma del canal B
		
		cv::Mat histR_para_graficar = graficar_histograma(histR,256);
		cv::Mat histG_para_graficar = graficar_histograma(histG,256);
		cv::Mat histB_para_graficar = graficar_histograma(histB,256);	
		
//		namedWindow("Histograma Canal R",1);
//		imshow("Histograma Canal R", histR_para_graficar);		
//		waitKey(0);
//		namedWindow("Histograma Canal G",1);
//		imshow("Histograma Canal G", histG_para_graficar);		
//		waitKey(0);
//		namedWindow("Histograma Canal B",1);
//		imshow("Histograma Canal B", histB_para_graficar);		
//		waitKey(0);
		
		//Radio de la esfera		
		int radio = 50; //prueba y error este valor.. No se bien como usar para esto el histograma
		
		mascara = cv::Mat::zeros(imagen.rows, imagen.cols,CV_8UC(1));			
		
		//(d)		
		std::vector< cv::Mat > bgrI; 
		cv::split(imagen, bgrI);
		
		for (int i=0; i<imagen.rows; i++){
			for (int j=0; j<imagen.cols; j++){	
			
				float r,g,b;
				r = pow((bgrI[2].at<uchar>(i,j) - meanRf),2);
				g = pow((bgrI[1].at<uchar>(i,j) - meanGf),2);
				b = pow((bgrI[0].at<uchar>(i,j) - meanBf),2);
				float dist = pow((r+g+b),0.5);
				if (dist > radio)
				{
					mascara.at<uchar>(i,j)=0;
				}
				else{
					mascara.at<uchar>(i,j)=255;
				}				
			}
		}
		
		namedWindow("Mascara",1);
		imshow("Mascara", mascara);		
		waitKey(0);
		
		//(e)		
		cv::Mat imagen_segmentada_CB = producto_mascara(bgrI[0],mascara);
		cv::Mat imagen_segmentada_CG = producto_mascara(bgrI[1],mascara);
		cv::Mat imagen_segmentada_CR = producto_mascara(bgrI[2],mascara);
		cv::Mat imagen_segmentada;
	
		vector<cv::Mat>canales_enmascarados;
		canales_enmascarados.push_back(imagen_segmentada_CB);
		canales_enmascarados.push_back(imagen_segmentada_CG);
		canales_enmascarados.push_back(imagen_segmentada_CR);
		
		cv::merge(canales_enmascarados, imagen_segmentada);

		namedWindow("Imagen Segmentada",1);
		imshow("Imagen Segmentada", imagen_segmentada);		
		waitKey(0);
		
		return imagen_segmentada;
		
	}


//	HSV is more meaningfully related to psychological perception of colour than 
//	RGB is - for example, if you want to segment on red, then in HSV all reds from 
//	lighest to darkest to least saturated to most saturated have the same hue, 
//	but in RGB if you filter by R > a certain amount you will catch bright 
//	oranges and miss dark reds, for example
	
cv::Mat segmentacion_HSV(string nombre_ventana, cv::Mat imagen, cv::Mat &mascara, cv::Mat ROI){
	
		cv::Mat imagen_en_grayscale;
		cv::cvtColor(imagen, imagen_en_grayscale, CV_RGB2GRAY);
		
		//Un rojo claro u oscuro, van a compartir el valor de H, lo que los diferenciar� ser�n los valores de
		//intensidad y saturaci�n
		
		//Entonces buscamos el valor de HUE aproximado que queremos segmentar, 
		//seleccionando una region y viendo su histograma
		
		//(b)
		namedWindow("Escala de grises",1);
		imshow("Escala de grises", imagen_en_grayscale);
//		setMouseCallback("Escala de grises", capturar_ROI,(void*)&imagen_en_grayscale);		
		waitKey(0); //Se clickea, sin soltar vas hasta el punto inferior derecho, soltas
		//enter, enter
		
//		cv::Rect myROI(my_init_point, my_end_point);
//		cv::Mat croppedImage = imagen(myROI); 
 
		
		std::vector< cv::Mat > hsvCI; 
		cv::Mat croppedImage_en_HSV;
		
		cv::cvtColor(ROI, croppedImage_en_HSV, CV_RGB2HSV);
		cv::split(croppedImage_en_HSV, hsvCI);
		
		//(c)
		cv::Mat histH = pdi::histogram(hsvCI[0],180,cv::Mat()); //histograma del canal H
		cv::Mat histS = pdi::histogram(hsvCI[1],256,cv::Mat()); //histograma del canal S
				
		cv::Mat histH_para_graficar = graficar_histograma(histH,180);
		cv::Mat histS_para_graficar = graficar_histograma(histS,256);			
		
		namedWindow("Histograma Canal H",1);
		imshow("Histograma Canal H", histH_para_graficar);		
		waitKey(0);
		namedWindow("Histograma Canal S",1);
		imshow("Histograma Canal S", histS_para_graficar);		
		waitKey(0);

		//Valores - determinados "experimentalmente" - aproximaci�n de lo que se ve en el histograma
		//esto se cambia segun el color que se quiere segmentar

		//Tolerancias		
		int tolH = 20;  
		int tolS = 40; 
		
		cv::Scalar meanH = mean( hsvCI[0] );
		float meanHf = meanH.val[0];
		
		cv::Scalar meanS = mean( hsvCI[1] );
		float meanSf = meanS.val[0];
		
		int H= meanHf; //Ejemplo para rojos
		int S= meanSf; 

		mascara = cv::Mat::zeros(imagen.rows, imagen.cols,CV_8UC(1));			
		
		//(d)	
		cv::Mat imagen_en_HSV;
		cv::cvtColor(imagen, imagen_en_HSV, CV_RGB2HSV);
		std::vector< cv::Mat > hsvI; 
		cv::split(imagen_en_HSV, hsvI);
		
		for (int i=0; i<imagen.rows; i++){
			for (int j=0; j<imagen.cols; j++){					
				float h,s;
				h = abs((int)hsvI[0].at<uchar>(i,j) - H);
				s = abs((int)hsvI[1].at<uchar>(i,j) - S);
				if ( (h <= tolH) and (s <= tolS) )
				{	
					mascara.at<uchar>(i,j)=255;					
				}
			}
		}
		
		namedWindow("Mascara",1);
		imshow("Mascara", mascara);		
		waitKey(0);
		
		//(e)		
		cv::Mat imagen_segmentada_CB = producto_mascara(hsvI[0],mascara);
		cv::Mat imagen_segmentada_CG = producto_mascara(hsvI[1],mascara);
		cv::Mat imagen_segmentada_CR = producto_mascara(hsvI[2],mascara);
		
		cv::Mat imagen_segmentada;

		vector<cv::Mat>canales_enmascarados;
		canales_enmascarados.push_back(imagen_segmentada_CB);
		canales_enmascarados.push_back(imagen_segmentada_CG);
		canales_enmascarados.push_back(imagen_segmentada_CR);

		cv::merge(canales_enmascarados, imagen_segmentada);
	
		cv::cvtColor(imagen_segmentada, imagen_segmentada, CV_HSV2RGB);
		namedWindow("Imagen Segmentada",1);
		imshow("Imagen Segmentada", imagen_segmentada);		
		waitKey(0);

		return imagen_segmentada;

	}
	
	
	
		
	
	void detector_de_bordes(cv::Mat img, cv::Mat &salida, int op){
		//1)Escriba una funci�on que implemente el detector de bordes de Roberts. La funci�on
		//debe obtener como salida una imagen binaria conteniendo los bordes detectados.
		//Apl�iquelo sobre la imagen �estanbul.tif�.
		//2)Incorpore a la funci�on anterior los detectores de bordes de Prewitt, Sobel, Laplaciano y LoG, 
		//permitiendo al usuario seleccionar cualquiera de ellos. Compare
		//los resultados obtenidos con los diferentes m�etodos
			
		switch (op){
			
			case 1: //Roberts
			{	
				
				cout<<"Roberts"<<endl;
				
				cv::Mat Gx = cv::Mat::zeros(2,2,CV_64F);
				cv::Mat Gy = cv::Mat::zeros(2,2,CV_64F);
				
				Gx.at<double>(0,0)=-1;
				Gx.at<double>(1,1)=1;
				
				Gy.at<double>(1,0)=1;
				Gy.at<double>(0,1)=-1;
				
				cout<<"Gx:"<<endl<<Gx<<endl;
				cout<<"Gy:"<<endl<<Gy<<endl;
				
				cv::Mat resX,resY;
				resX = pdi::convolve(img,Gx);
				resY = pdi::convolve(img,Gy);		
				
				namedWindow("Gx",1);	
				imshow("Gx",resX);
				waitKey(0);
				namedWindow("Gy",1);	
				imshow("Gy",resY);
				waitKey(0);
				
				salida = suma(resX,resY);
				
				break;
			}	
			case 2: //Prewitt
			{
				
				cout<<"Prewitt"<<endl;
				
				cv::Mat G1 = cv::Mat::zeros(3,3,CV_64F);
				cv::Mat G2 = cv::Mat::zeros(3,3,CV_64F);
				cv::Mat G3 = cv::Mat::zeros(3,3,CV_64F);
				cv::Mat G4 = cv::Mat::zeros(3,3,CV_64F);
				
				G1.at<double>(0,0)=-1;
				G1.at<double>(0,1)=-1;
				G1.at<double>(0,2)=-1;
				G1.at<double>(2,0)=1;
				G1.at<double>(2,1)=1;
				G1.at<double>(2,2)=1;
				
				G2.at<double>(0,0)=-1;
				G2.at<double>(1,0)=-1;
				G2.at<double>(2,0)=-1;
				G2.at<double>(0,2)=1;
				G2.at<double>(1,2)=1;
				G2.at<double>(2,2)=1;
				
				G3.at<double>(1,0)=-1;
				G3.at<double>(2,0)=-1;
				G3.at<double>(2,1)=-1;
				G3.at<double>(0,1)=1;
				G3.at<double>(0,2)=1;
				G3.at<double>(1,2)=1;
				
				G4.at<double>(0,0)=-1;
				G4.at<double>(1,0)=-1;
				G4.at<double>(0,1)=-1;
				G4.at<double>(1,2)=1;
				G4.at<double>(2,1)=1;
				G4.at<double>(2,2)=1;
				
				cout<<"G1:"<<endl<<G1<<endl;
				cout<<"G2:"<<endl<<G2<<endl;
				cout<<"G3:"<<endl<<G3<<endl;
				cout<<"G4:"<<endl<<G4<<endl;
				
				cv::Mat res1,res2,res3,res4;
				res1 = pdi::convolve(img,G1);
				res2 = pdi::convolve(img,G2);		
				res3 = pdi::convolve(img,G3);		
				res4 = pdi::convolve(img,G4);		
				
				salida = suma(suma(res1,res2),suma(res3,res4));
				
				break;
			}
			case 3: //Sobel
			{
				
				cout<<"Sobel"<<endl;				
				
				cv::Mat G1 = cv::Mat::zeros(3,3,CV_64F);
				cv::Mat G2 = cv::Mat::zeros(3,3,CV_64F);
				cv::Mat G3 = cv::Mat::zeros(3,3,CV_64F);
				cv::Mat G4 = cv::Mat::zeros(3,3,CV_64F);
				
				G1.at<double>(0,0)=-1;
				G1.at<double>(0,1)=-2;
				G1.at<double>(0,2)=-1;
				G1.at<double>(2,0)=1;
				G1.at<double>(2,1)=2;
				G1.at<double>(2,2)=1;
				
				G2.at<double>(0,0)=-1;
				G2.at<double>(1,0)=-2;
				G2.at<double>(2,0)=-1;
				G2.at<double>(0,2)=1;
				G2.at<double>(1,2)=2;
				G2.at<double>(2,2)=1;
				
				G3.at<double>(1,0)=-1;
				G3.at<double>(2,0)=-2;
				G3.at<double>(2,1)=-1;
				G3.at<double>(0,1)=1;
				G3.at<double>(0,2)=2;
				G3.at<double>(1,2)=1;
				
				G4.at<double>(0,0)=-2;
				G4.at<double>(1,0)=-1;
				G4.at<double>(0,1)=-1;
				G4.at<double>(1,2)=1;
				G4.at<double>(2,1)=1;
				G4.at<double>(2,2)=2;
				
				cout<<"G1:"<<endl<<G1<<endl;
				cout<<"G2:"<<endl<<G2<<endl;
				cout<<"G3:"<<endl<<G3<<endl;
				cout<<"G4:"<<endl<<G4<<endl;
				
				cv::Mat res1,res2,res3,res4;
				res1 = pdi::convolve(img,G1);
				res2 = pdi::convolve(img,G2);		
				res3 = pdi::convolve(img,G3);		
				res4 = pdi::convolve(img,G4);		
				
				salida = suma(suma(res1,res2),suma(res3,res4));
				
				break;
			}
			case 4: //Laplaciano
			{			
				
				cout<<"Laplaciano"<<endl;
				
				//Vecindad de 4				
//				cv::Mat G = cv::Mat::zeros(3,3,CV_64F);
//				G.at<double>(0,1)=-1;
//				G.at<double>(1,0)=-1;
//				G.at<double>(2,1)=-1;
//				G.at<double>(1,2)=-1;
//				G.at<double>(1,1)=4;
				
				//Vecindad de 8				
				cv::Mat G = (-1)*cv::Mat::ones(3,3,CV_64F);
				G.at<double>(1,1)=8;
				
				cout<<"G:"<<endl<<G<<endl;
				
				
				cv::Mat res;
				res = pdi::convolve(img,G);				
								
				salida = res;
				
				break;
			}
			case 5: //LoG
			{
				
				cout<<"LoG"<<endl;
			
				cv::Mat G = cv::Mat::zeros(5,5,CV_64F);
				
				G.at<double>(0,2)=-1;
				G.at<double>(1,1)=-1;
				G.at<double>(1,2)=-2;
				G.at<double>(1,3)=-1;
				G.at<double>(2,0)=-1;
				G.at<double>(2,1)=-2;
				G.at<double>(2,2)=16;
				G.at<double>(2,3)=-2;
				G.at<double>(2,4)=-1;
				G.at<double>(3,1)=-1;
				G.at<double>(3,2)=-2;
				G.at<double>(3,3)=-1;
				G.at<double>(4,2)=-1;
				
				cout<<"G:"<<endl<<G<<endl;
				
				
				cv::Mat res;
				res = pdi::convolve(img,G);				
				
				salida = res;
				
				break;
				
			}
			
		
		}
		
//		//umbralizar
//		
//		for (int i=0; i<salida.rows; i++){
//			for (int j=0; j<salida.cols; j++){
//				if ((int)salida.at<uchar>(i,j)>80){
//					salida.at<uchar>(i,j) = 255;
//				}
//				
//			}
//		}
		
		return;	
	}
	


//Transformada de Hough:
//in general, a line can be detected by finding the number of intersections 
//between curves.The more curves intersecting means that the line represented 
//by that intersection have more points. In general, we can define a 
//threshold of the minimum number of intersections needed to detect a line.
//This is what the Hough Line Transform does. It keeps track of the 
//with the parameters (\theta, r_{\theta}) of the intersection point.
//intersection between curves of every point in the image. If the number 
//of intersections is above some threshold, then it declares it as a line 
	
//OpenCV implements two kind of Hough Line Transforms:
///The Standard Hough Transform
//It consists in pretty much what we just explained in the previous section. 
//It gives you as result a vector of couples (\theta, r_{\theta})
//In OpenCV it is implemented with the function *HoughLines*
///The Probabilistic Hough Line Transform
//A more efficient implementation of the Hough Line Transform. It gives as output 
//the extremes of the detected lines (x_{0}, y_{0}, x_{1}, y_{1})
//In OpenCV it is implemented with the function *HoughLinesP*
	
///Terminar------------------------------------------------------------------
void ejercicio21_guia7(){
		
//Aplique la transformada Hough a patrones binarios de puntos blancos en 
//localizaciones determinadas de la imagen. Genere im�agenes con segmentos 
//blancos
//en diferentes direcciones y de distinta longitud y eval�ue la respuesta de la 
//transformada de Hough.
		
	//Generar los patrones binarios	
	//Puntos
	cv::Mat patron1 = cv::Mat::zeros(1000,1000,CV_LOAD_IMAGE_GRAYSCALE);			
	cv::Mat patron2 = cv::Mat::zeros(500,1000,CV_LOAD_IMAGE_GRAYSCALE);			
	patron1.at<uchar>(10,10)=255;
	patron1.at<uchar>(500,500)=255;
	patron1.at<uchar>(100,100)=255;
	patron1.at<uchar>(40,70)=255;
	patron1.at<uchar>(30,10)=255;
	patron1.at<uchar>(300,200)=255;
	patron1.at<uchar>(400,400)=255;
	patron1.at<uchar>(500,500)=255;
	patron1.at<uchar>(400,600)=255;
	patron1.at<uchar>(123,345)=255;
	
	//Rectas
	line( patron2, cvPoint(400,400) , cvPoint(60,600), Scalar(255,255,255), 1, CV_AA);
	line( patron2, cvPoint(0,0) , cvPoint(45,60), Scalar(255,255,255), 1, CV_AA);
	line( patron2, cvPoint(500,0) , cvPoint(0,600), Scalar(255,255,255), 1, CV_AA);

	namedWindow("Patron 2",1);
	imshow("Patron 2",patron2);
	waitKey(0);
	
	namedWindow("Patron 1",1);
	imshow("Patron 1",patron1);
	waitKey(0);
	
	//Patron 1
	Mat dst, cdst;
	cvtColor(patron1, cdst, CV_GRAY2BGR);	
	
	vector<Vec2f> lines;
	HoughLines(patron1, lines, 1, CV_PI/180, 100, 0, 0 );
	
//	srn � For the multi-scale Hough transform, it is a divisor for the distance resolution rho.
//	stn � For the multi-scale Hough transform, it is a divisor for the distance resolution theta.
	
	for( size_t i = 0; i < lines.size(); i++ )
	{
		float rho = lines[i][0], theta = lines[i][1];
		Point pt1, pt2;
		double a = cos(theta), b = sin(theta);
		double x0 = a*rho, y0 = b*rho;
		pt1.x = cvRound(x0 + 1000*(-b));
		pt1.y = cvRound(y0 + 1000*(a));
		pt2.x = cvRound(x0 - 1000*(-b));
		pt2.y = cvRound(y0 - 1000*(a));
		line( cdst, pt1, pt2, Scalar(0,0,255), 3, CV_AA);
	}
	
	namedWindow("Transformada del patron 1",1);
	imshow("Transformada del patron 1",cdst);
	waitKey(0);
	
	//Patron 2
	Mat dst2, cdst2;
	cvtColor(patron2, cdst2, CV_GRAY2BGR);	
	
	vector<Vec2f> lines2;
	HoughLines(patron2, lines2, 3, CV_PI/180, 150, 0, 0 );
	
	for( size_t i = 0; i < lines2.size(); i++ )
	{
		float rho = lines2[i][0], theta = lines2[i][1];
		Point pt1, pt2;
		double a = cos(theta), b = sin(theta);
		double x0 = a*rho, y0 = b*rho;
		pt1.x = cvRound(x0 + 1000*(-b));
		pt1.y = cvRound(y0 + 1000*(a));
		pt2.x = cvRound(x0 - 1000*(-b));
		pt2.y = cvRound(y0 - 1000*(a));
		line( cdst2, pt1, pt2, Scalar(0,255,0), 1, CV_AA);
	}
	
	namedWindow("Transformada del patron 2",1);
	imshow("Transformada del patron 2",cdst2);
	waitKey(0);
	
	}
	
	void ejercicio22_guia7(){
		
//Genere una funci�on que a partir de la transformada de Hough identifique l�ineas
//en un �angulo de inter�es y con longitud fijada por el usuario 
//(parametros a implementar). Aplique la funci�on sobre las im�agenes �letras1.tif�, �letras2.tif�
//y �snowman.png�.
		
		
		
		
	}
	
	
///--------------------------------------------------------------------------

Mat dilatar(Mat img,Mat ee, int iter){

	cout<<"Kernel Dilatar:"<<endl<<ee<<endl;	
	cv::Mat dilatada;
	dilate(img,dilatada,ee,cvPoint(-1,-1),iter);	
	
	return dilatada;
}

Mat erosionar(Mat img,Mat ee, int iter){
	
	cout<<"Kernel Erosionar:"<<endl<<ee<<endl;	
	cv::Mat erosionada;
	erode(img,erosionada,ee,cvPoint(-1,-1),iter);	
	
	return erosionada;
}

Mat apertura(Mat img,Mat ee, int iter){	
	cout<<"Kernel Apertura:"<<endl<<ee<<endl;	
	cv::Mat apertura, erosionada;
	erosionada = erosionar(img,ee,iter);	
	apertura = dilatar(erosionada,ee,iter);	
	
//	namedWindow("Apertura",1);
//	imshow("Apertura",apertura);
//	waitKey(0);
	
	return apertura;
}
Mat cierre(Mat img,Mat ee, int iter){	
	cout<<"Kernel Cierre:"<<endl<<ee<<endl;	
	cv::Mat cierre,dilatada;
	dilatada = dilatar(img,ee,iter);	
	cierre = erosionar(dilatada,ee,iter);		
	
//	namedWindow("Cierre",1);
//	imshow("Cierre",cierre);
//	waitKey(0);
	
	return cierre;
}

Mat ensuciar_sal_y_pimienta(Mat img, int s){ //Para imagenes binarias y color :)
	//el valor 's' es para regular la cantidad de puntos, mientras mas grande es 's', menos puntos hay en la imagen
	Mat img_sucia = Mat::zeros(img.rows, img.cols,CV_8UC(1));
	
	vector<Mat>canales;
	split(img,canales);
	
	for (int i=0;i<img.rows;i++){
		for (int j=0;j<img.cols;j++){
			int valor = rand()%2;
			if (rand()%s == s-1){
				if (canales.size()==3){
					if(valor==1){
						canales[0].at<uchar>(i,j)=255;
						canales[1].at<uchar>(i,j)=255;
						canales[2].at<uchar>(i,j)=255;
					}else{
						canales[0].at<uchar>(i,j)=0;
						canales[1].at<uchar>(i,j)=0;
						canales[2].at<uchar>(i,j)=0;
					}
				}else if(canales.size()==1){
					if(valor==1){
						canales[0].at<uchar>(i,j)=255;
					}else{
						canales[0].at<uchar>(i,j)=0;
					}
				}
				
				
			}
			
		}
	}
	
	merge(canales,img_sucia);
	
	namedWindow("Sal y pimienta",1);
	imshow("Sal y pimienta",img_sucia);
	waitKey(0);
	
	return img_sucia;
	
}
	


};




#endif
