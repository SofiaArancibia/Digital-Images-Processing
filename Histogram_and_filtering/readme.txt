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

