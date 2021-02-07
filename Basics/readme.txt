*VARIAS IMAGENES EN UNA SOLA VENTANA

//Leo varias imagenes y las guardo en un vector
	
	Mat image4 = imread("C:\\Users\\ribeiro\\Desktop\\PDI- 2017\\Imagenes\\huang1.jpg",CV_8UC1);
	Mat image1 = imread("C:\\Users\\ribeiro\\Desktop\\PDI- 2017\\Imagenes\\chairs.jpg",CV_8UC1);	
	Mat image2 = imread("C:\\Users\\ribeiro\\Desktop\\PDI- 2017\\Imagenes\\futbol.jpg",CV_8UC1);
	Mat image3 = imread("C:\\Users\\ribeiro\\Desktop\\PDI- 2017\\Imagenes\\ratoncillo.jpg",CV_8UC1);
	
	Mat model = Mat::zeros( 400, 600, CV_8UC(1) ); //crear imagen para copiar tamaño (creo que es al pedo)
	
	vector<Mat> images;
//Guardo las imagenes en el vector
	images.push_back(image1);
	images.push_back(image2);
	images.push_back(image3);
	images.push_back(image4);
	
///RESIZING: Cambio el tamaño para que todas tengan el mismo (el del modelo)
	for (int i=0; i<images.size(); i++){
		resize(images[i],images[i],model.size());
	}

//Creo el mosaico
	cv::Mat image_array = pdi::mosaic(images,2);


------------------------------------------------------------------------------------------------------------------

*GUARDAR IMAGEN

///GUARDAR la imagen
	vector<int> compression_params;
	compression_params.push_back(CV_IMWRITE_JPEG_QUALITY);
	imwrite("C:\\Users\\ribeiro\\Desktop\\PDI- 2017\\Imagenes\\escrito.jpg", image_array,compression_params );


*MOSTRAR IMAGEN
	
	namedWindow("Image Array"); //ventana para mostrar la imagen
	imshow("Image Array", image_array);

------------------------------------------------------------------------------------------------------------------
