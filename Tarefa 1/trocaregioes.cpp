#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main(int, char** argv){
  Mat image;

  namedWindow("Original",WINDOW_AUTOSIZE);
  image = imread(argv[1],IMREAD_GRAYSCALE);

  if(!image.data){
    cout << "nao abriu bolhas.png" << std::endl;
  }
  imshow("Original", image);
  waitKey();

  //recorte superior esquerdo da imagem
  Mat rec1 = image(Rect(0,0,image.rows/2,image.cols/2)); 
  //recorte superior direito da imagem
  Mat rec2 = image(Rect(image.rows/2,0,image.rows/2,image.cols/2));
  // recorte inferior esquerdo
  Mat rec3 = image(Rect(0,image.cols/2,image.rows/2,image.cols/2));
  // recorte inferior direito
  Mat rec4 = image(Rect(image.rows/2,image.cols/2,image.rows/2,image.cols/2));

  Mat troca(image.rows,image.cols,image.type());

  // trocando inferior direito para o superior esquerdo
  rec4.copyTo(troca(Rect(0,0,image.rows/2,image.cols/2)));
  // trocando inferior esquerdo para o superior direito
  rec3.copyTo(troca(Rect(image.rows/2,0,image.rows/2,image.cols/2)));
  // trocando superior direito para inferior direito
  rec2.copyTo(troca(Rect(0,image.cols/2,image.rows/2,image.cols/2)));
  // trocando superior esquerdo para inferior direito 
  rec1.copyTo(troca(Rect(image.rows/2,image.cols/2,image.rows/2,image.cols/2)));

  namedWindow("regioes trocadas", WINDOW_AUTOSIZE);
  imshow("regioes trocadas", troca);
  waitKey();    
  

  
  return 0;
}