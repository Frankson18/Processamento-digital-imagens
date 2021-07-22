#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main(int, char** argv){
  Mat image;
  int x1,y1,x2,y2;

  namedWindow("Original",WINDOW_AUTOSIZE);
  image = imread(argv[1],IMREAD_GRAYSCALE);

  if(!image.data){
    cout << "nao abriu bolhas.png" << std::endl;
  }
  imshow("Original", image);
  waitKey();

  cout << "Ditite as coordenadas do primeiro ponto: "<<endl;
  cin >> x1 >> y1;
  cout << "Ditite as coordenadas do primeiro ponto: "<<endl;
  cin >> x2 >> y2; 
  
  //Verifica se os pontos são validos
  if(x1!=x2 && y1!=y2 &&x1>=0 && x2<=image.rows && y1>=0 && y2<=image.rows){
    //faz a o negativo da região escolhida
    for(int i=x1;i<x2;i++){
      for(int j=y1;j<y2;j++){
        image.at<uchar>(i,j)=255 - image.at<uchar>(i,j);
      }
    }

    namedWindow("Negativo", WINDOW_AUTOSIZE);
    imshow("Negativo", image);
    waitKey(); 
       
  }else{
    cout<<"coordenadas invalidas"<<endl;
  }

  
  return 0;
}