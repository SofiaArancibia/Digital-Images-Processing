#ifndef MISFUNCIONES_H
#define MISFUNCIONES_H

#include <iostream>
#include <opencv2/opencv.hpp>
#include <vector>
#include <opencv2/highgui.hpp>
#include "../pdi_functions-master/pdi_functions.h"
#include <string>
#include <cstdio>
#include <sstream>

using namespace std;
using namespace cv;

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
		
		///CROP the Image
		cv::Rect myROI(my_init_point, my_end_point);
		cv::Mat croppedImage = image(myROI); 
		
		///Guardo el recorte
		std::vector<int> compression_params;
		compression_params.push_back(CV_IMWRITE_JPEG_QUALITY);
		imwrite("C:\\Users\\ribeiro\\Desktop\\PDI- 2017\\Imagenes\\recorte.jpg", croppedImage,compression_params );		
		
	}
	
	void PerfilDeIntensidad(cv::Mat imagen,std::string segmento,bool graficar, std::vector<int> &intensidades)
	{
	///Param
	//imagen -> imagen de referencia
	//segmento -> puede ser una fila o una columna (únicamente, por ahora). Ej.: f100 o c9 (fila 100 o columna 8)
	//graficar -> indica si se quiere o no graficar el perfil de intensidad
	
	//1) Identificar si quiere fila o columna
		
	char tipo = segmento[0];
	int posicion = atoi((segmento.substr(1,segmento.size())).c_str());
//	std::cout<<"Tipo: "<<tipo<<" Posición: "<<posicion<<std::endl;
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
		printf("Parámetro %s mal ingresado!",segmento);
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
		
		//tamaño de la imagen de entrada
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
		//binarizo de otra forma para separar el líquido (?
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
		//se puede ver que el constraste mejora significativamente, ya que el rango dinámico es mucho mayor
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



	void filrado_por_mascara_difusa(cv::Mat img, cv::Mat &res, cv::Mat &mascara,cv::Mat &blurreada, int tamanio){
	
	cv::Mat copia_original1;
	copia_original1 = img.clone();
	
	//1) Blurreo por filtros de promediado
	cv::blur(img, blurreada, cv::Size(tamanio,tamanio)); 
	
	//2) Sustraer la imagen blurreada de la original
	mascara = resta(img, blurreada);
	
	//3) Sumar la máscara a la imagen original
	res = suma(copia_original1,mascara);

	}

	void filrado_por_highboost(cv::Mat img, cv::Mat &res, cv::Mat &mascara,cv::Mat &blurreada, int tamanio, int A){
		
		//Si A=1 entonces tngo el filtrado por máscara difusa
		
		cv::Mat copia_original1;
		copia_original1 = img.clone();
		
		//1) Blurreo por filtros de promediado
		cv::blur(img, blurreada, cv::Size(tamanio,tamanio)); 
		
		//2) Sustraer la imagen blurreada de la original
		mascara = resta(img, blurreada);
		
		//3) Sumar la máscara a la imagen original
		res = suma(copia_original1,A*mascara);
		
	}
	
};




#endif
