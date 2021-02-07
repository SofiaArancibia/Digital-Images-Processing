Ejercicio 1:
1.1 y 1.2

	int filas = 300;
	int columnas = 500;	
	
	namedWindow("Linea horizontal",1);	
	cv::Mat l_horizontal = crear_imagen_con_linea_horizontal(filas, columnas,filas/4, 3);	
	imshow("Linea horizontal", l_horizontal);
	waitKey(0);
	//Mientras más ancha hago la linea, más "tramitos" se marcan en la TDF
	//Cambiando de posicion la linea, no veo cambios en la TDF -> Propiedad de TDF: desplazamiento en espacio acarrea 
																					//desplazamiento en a frecuencia
	//TDF
	l_horizontal.convertTo(l_horizontal, CV_32F);
	cv::Mat tfd_linea_horizontal = pdi::spectrum(l_horizontal); //la imagen debe ser 32F
	namedWindow("TFD linea horizontal",1);
	imshow("TFD linea horizontal", tfd_linea_horizontal);
	waitKey(0);
	
	namedWindow("Linea vertical",1);
	cv::Mat l_vertical = crear_imagen_con_linea_vertical(filas, columnas, columnas/2, 3);	
	imshow("Linea vertical", l_vertical);
	waitKey(0);
	
	//TDF
	l_vertical.convertTo(l_vertical, CV_32F);
	cv::Mat tfd_linea_vertical = pdi::spectrum(l_vertical); //la imagen debe ser 32F		
	namedWindow("TFD linea vertical",1);
	imshow("TFD linea vertical", tfd_linea_vertical);
	waitKey(0);
	
	namedWindow("Cuadrado1",1);
	namedWindow("Cuadrado2",1);
	namedWindow("Cuadrado3",1);
	namedWindow("Cuadrado4",1);
	cv::Mat cuadrado1 = crear_imagen_con_cuadrado(filas,columnas,filas/2,columnas/2,50);	
	cv::Mat cuadrado2 = crear_imagen_con_cuadrado(filas,columnas,filas/2,columnas/2,100);	
	cv::Mat cuadrado3 = crear_imagen_con_cuadrado(filas,columnas,filas/4,columnas/4,50);	
	cv::Mat cuadrado4 = crear_imagen_con_cuadrado(filas,columnas,filas/4,columnas/4,50);
	cuadrado4 = pdi::rotate(cuadrado4, 45);
	imshow("Cuadrado1", cuadrado1);
	imshow("Cuadrado2", cuadrado2);
	imshow("Cuadrado3", cuadrado3);
	imshow("Cuadrado4", cuadrado4);
	waitKey(0);
	
	//TDF
	cuadrado1.convertTo(cuadrado1, CV_32F);
	cv::Mat tfd_cuadrado1 = pdi::spectrum(cuadrado1); //la imagen debe ser 32F		
	cuadrado2.convertTo(cuadrado2, CV_32F);
	cv::Mat tfd_cuadrado2 = pdi::spectrum(cuadrado2); //la imagen debe ser 32F		
	cuadrado3.convertTo(cuadrado3, CV_32F);
	cv::Mat tfd_cuadrado3 = pdi::spectrum(cuadrado3); //la imagen debe ser 32F		
	cuadrado4.convertTo(cuadrado4, CV_32F);
	cv::Mat tfd_cuadrado4 = pdi::spectrum(cuadrado4); //la imagen debe ser 32F		
	namedWindow("TFD cuadrado1",1);
	imshow("TFD cuadrado1", tfd_cuadrado1);
	waitKey(0);
	namedWindow("TFD cuadrado2",1);
	imshow("TFD cuadrado2", tfd_cuadrado2);
	waitKey(0);
	namedWindow("TFD cuadrado3",1);
	imshow("TFD cuadrado3", tfd_cuadrado3);
	waitKey(0);
	namedWindow("TFD cuadrado4",1);
	imshow("TFD cuadrado4", tfd_cuadrado4);
	waitKey(0);
	
	Al desplazar el cuadrado, no se modifica la TDF. Al aumentar el tamaño del cuadrado, las lineas de la TDF se afinan
	Para el rectangulo es lo mismo
	Al rotar el cuadrado un angulo de, por ejemplo, 45 grados, la TDF rota la misma cantidad
	
	
	namedWindow("Rectangulo",1);
	cv::Mat rectangulo = crear_imagen_con_rectangulo(filas,columnas,filas/2,columnas/2,50,200);	
	imshow("Rectangulo", rectangulo);
	waitKey(0);
	
	//TDF
	rectangulo.convertTo(rectangulo, CV_32F);
	cv::Mat tfd_rectangulo = pdi::spectrum(rectangulo); //la imagen debe ser 32F		
	namedWindow("TFD rectangulo",1);
	imshow("TFD rectangulo", tfd_rectangulo);
	waitKey(0);
	
	namedWindow("Circulo1",1);
	namedWindow("Circulo2",1);
	namedWindow("Circulo3",1);
	cv::Mat circulo1 = crear_imagen_con_circulo(filas,columnas,filas/2,columnas/2,50);	
	cv::Mat circulo2 = crear_imagen_con_circulo(filas,columnas,filas/2,columnas/2,100);	
	cv::Mat circulo3 = crear_imagen_con_circulo(filas,columnas,filas/4,columnas/4,50);	
	imshow("Circulo1", circulo1);
	imshow("Circulo2", circulo2);
	imshow("Circulo3", circulo3);
	waitKey(0);
	
	//TDF
	circulo1.convertTo(circulo1, CV_32F);
	circulo2.convertTo(circulo2, CV_32F);
	circulo3.convertTo(circulo3, CV_32F);
	cv::Mat tfd_circulo1 = pdi::spectrum(circulo1); //la imagen debe ser 32F		
	cv::Mat tfd_circulo2 = pdi::spectrum(circulo2); //la imagen debe ser 32F		
	cv::Mat tfd_circulo3 = pdi::spectrum(circulo3); //la imagen debe ser 32F		
	namedWindow("TFD circulo1",1);
	namedWindow("TFD circulo2",1);
	namedWindow("TFD circulo3",1);
	imshow("TFD circulo1", tfd_circulo1);
	imshow("TFD circulo2", tfd_circulo2);
	imshow("TFD circulo3", tfd_circulo3);
	waitKey(0);
	La TDF no cambia al desplazar el circulo en el espacio. Se observan anillos concentricos
	Al aumentar el radio del circulo, los anillos concéntricos se ven más juntos y el punto central mas chico
	
	vector<cv::Mat> figuras_y_tfd;
	
	figuras_y_tfd.push_back(l_horizontal);
	figuras_y_tfd.push_back(tfd_linea_horizontal);
	figuras_y_tfd.push_back(l_vertical);
	figuras_y_tfd.push_back(tfd_linea_vertical);
	figuras_y_tfd.push_back(cuadrado);
	figuras_y_tfd.push_back(tfd_cuadrado);
	figuras_y_tfd.push_back(rectangulo);
	figuras_y_tfd.push_back(tfd_rectangulo);
	figuras_y_tfd.push_back(circulo);
	figuras_y_tfd.push_back(tfd_circulo);
	
	Mat model = Mat::zeros( 400, 600, CV_8UC(1) ); //crear imagen para copiar tamaño (creo que es al pedo)
	///RESIZING: Cambio el tamaño para que todas tengan el mismo (el del modelo)
	for (int i=0; i<figuras_y_tfd.size(); i++){
		resize(figuras_y_tfd[i],figuras_y_tfd[i],model.size());
	}	
	//Creo el mosaico
	cv::Mat image_array = pdi::mosaic(figuras_y_tfd,5);
		
	///GUARDAR la imagen
	vector<int> compression_params;
	compression_params.push_back(CV_IMWRITE_PNG_BILEVEL);
	imwrite("C:\\Users\\ribeiro\\Desktop\\PDI- 2017\\Imagenes\\tfds.png", image_array,compression_params );

	namedWindow("Image Array"); //ventana para mostrar la imagen
	imshow("Image Array", image_array);
	waitKey(0);

//1.3
	ejercicio_1_3_guia5(filas,columnas);
	
	
	
	
