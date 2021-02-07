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
		
	
};



#endif
