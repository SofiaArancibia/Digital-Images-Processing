////create a gui window:
//namedWindow("Original",1);
////initialize a 120X350 matrix of black pixels:
//Mat imagen = imread("C:\\Users\\ribeiro\\Desktop\\PDI- 2017\\Imagenes\\futbol.jpg", CV_LOAD_IMAGE_GRAYSCALE);
////display the image:
//imshow("Original", imagen);
//waitKey(0);
//
//int img_filas = imagen.rows;
//int img_columnas = imagen.cols;
//
////Imagen de salida
//Mat salida = Mat::zeros(img_filas,img_columnas,CV_8UC1);
//
////Coeficientes para el LUT (look-up table)
//int a_coef = 6.5;
//int c_coef = 1;
//
//vector<int> myLUT = lut(a_coef, c_coef);
//
//for (int f = 0; f<img_filas; f++){
//	for (int c = 0; c<img_columnas; c++){
//		
//		int img_value = imagen.at<uchar>(f,c); //ver como es
//		salida.at<uchar>(f,c)= myLUT[img_value];	
//		
//		
//	}
//}	
//
//namedWindow("Transformada", 1);
//imshow("Transformada",salida);
//waitKey(0);
//
//vector<int> myiLUT = negativo();
//
//for (int f = 0; f<img_filas; f++){
//	for (int c = 0; c<img_columnas; c++){
//		
//		int img_value = imagen.at<uchar>(f,c); //ver como es
//		salida.at<uchar>(f,c)= myiLUT[img_value];	
//		
//		
//	}
//}
//
//namedWindow("Invertida", 1);
//imshow("Invertida",salida);
//waitKey(0);
//
//vector<int> tramos; 
//vector<vector<int> > constantes;
//
////Extremos de los tramos
//tramos.push_back(0);
//tramos.push_back(100);
//tramos.push_back(150);
//tramos.push_back(255);	
////--------------------------
//
////Constantes para cada tramo
//vector<int> p1; p1.push_back(0.5); p1.push_back(0);
//vector<int> p2; p2.push_back(1.5); p2.push_back(0);
//vector<int> p3; p3.push_back(1.5); p3.push_back(0);
//
//constantes.push_back(p1);
//constantes.push_back(p2);
//constantes.push_back(p3);
////--------------------------
//
//vector<int> mytLUT = tramos_lineales(tramos, constantes);		
//
//for (int f = 0; f<img_filas; f++){
//	for (int c = 0; c<img_columnas; c++){
//		
//		int img_value = imagen.at<uchar>(f,c); 
//		salida.at<uchar>(f,c)= mytLUT[img_value];				
//		
//	}
//}
//
//namedWindow("Lineal por tramos", 1);
//imshow("Lineal por tramos",salida);
//waitKey(0);
//
//Mat imagen2 = imread("C:\\Users\\ribeiro\\Desktop\\PDI- 2017\\Imagenes\\rmn.jpg", CV_LOAD_IMAGE_GRAYSCALE);
//
//imshow("Original2", imagen2);
//waitKey(0);
//
//int img2_filas = imagen2.rows;
//int img2_columnas = imagen2.cols;
//
////Imagen de salida
//Mat salida2 = Mat::zeros(img2_filas,img2_columnas,CV_8UC1);
//
//vector<int> mylLUT = logaritmo();		
//
//for (int f = 0; f<img2_filas; f++){
//	for (int c = 0; c<img2_columnas; c++){
//		
//		int img2_value = imagen2.at<uchar>(f,c); 
//		salida2.at<uchar>(f,c)= mylLUT[img2_value];				
//		
//	}
//}
//
//namedWindow("Logaritmo", 1);
//imshow("Logaritmo",salida2);
//waitKey(0);
//
//vector<int> mypLUT = potencia(1000);		
//
//for (int f = 0; f<img2_filas; f++){
//	for (int c = 0; c<img2_columnas; c++){
//		
//		int img2_value = imagen2.at<uchar>(f,c); 
//		salida2.at<uchar>(f,c)= mypLUT[img2_value];				
//		
//	}
//}
//
//namedWindow("Potencia", 1);
//imshow("Potencia",salida2);
//waitKey(0);

//
//namedWindow("Resultado Suma",1);
//Mat resultadoS = suma(imagen1, imagen2);
//imshow("Resultado Suma", resultadoS);
//waitKey(0);
//
//namedWindow("Resultado Resta",1);
//Mat resultadoR = resta(imagen1, imagen2);
//imshow("Resultado Resta", resultadoR);
//waitKey(0);
//
//namedWindow("Resultado Division",1);
//Mat resultadoD = division(imagen1, imagen2);
//imshow("Resultado Division", resultadoD);
//waitKey(0);
//
//int img_filas = imagen1.rows;
//int img_columnas = imagen1.cols;
//
//namedWindow("Mascara",1);
//Mat mascara = Mat::zeros(img_filas,img_columnas,CV_8UC1);
//
//for (int i=0; i<img_filas; i++){
//	for (int j=0; j<img_columnas; j++){
//		if (i>100 and i<200 and j>100 and j<200){
//			mascara.at<uchar>(i,j) = 1;
//		}			
//	}
//}	
//imshow("Mascara", mascara); //no debería ver un cuadrado blanco y todo lo demas negro?
//waitKey(0);
//
//namedWindow("Resultado Producto",1);
//Mat resultadoP = producto_mascara(imagen1, mascara);
//imshow("Resultado Producto", resultadoP);
//waitKey(0);
