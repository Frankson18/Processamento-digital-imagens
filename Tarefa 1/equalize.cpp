#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main(int argc, char** argv){
  Mat image, icinza, iequalizado;
  VideoCapture cap;
  vector<Mat> planes;
  Mat histcinza, histequalizado;
  int nbins = 64;
  float range[] = {0, 255};
  const float *histrange = { range };
  bool uniform = true;
  bool acummulate = false;
  int key;

	cap.open(0);

  if(!cap.isOpened()){
    std::cout << "cameras indisponiveis";
    return -1;
  }

  cap.set(CAP_PROP_FRAME_WIDTH, 640);
  cap.set(CAP_PROP_FRAME_HEIGHT, 480);

  int histw = nbins, histh = nbins/2;
  Mat histImgcinza(histh, histw, CV_8UC1,Scalar(0));
  Mat histImgequalizado(histh, histw, CV_8UC1,Scalar(0)); 

  while(1){
    cap >> image;
    
    //Convertendo para cinza
    cvtColor(image,icinza,CV_BGR2GRAY);

    //equalizando histograma
    equalizeHist(icinza,iequalizado);

    //calculando histograma imagem cinza original
    calcHist(&icinza, 1, 0,Mat(), histcinza, 1,
                 &nbins, &histrange,
                 uniform, acummulate);
    // calculando histograma da imagem cinza equalizada
    calcHist(&iequalizado, 1, 0,Mat(),histequalizado, 1,
                 &nbins, &histrange,
                 uniform, acummulate);
  
    //normalizando
    normalize(histcinza,histcinza, 0,histImgcinza.rows,NORM_MINMAX, -1,Mat());
    normalize(histequalizado,histequalizado, 0, histImgequalizado.rows,NORM_MINMAX, -1,Mat());

    histImgcinza.setTo(Scalar(0));
    histImgequalizado.setTo(Scalar(0));

    for(int i=0; i<nbins; i++){
      line(histImgcinza,
               Point(i, histh),
               Point(i, histh-cvRound(histcinza.at<float>(i))),
               Scalar(255, 255, 255), 1, 8, 0);
      line(histImgequalizado,
               Point(i, histh),
               Point(i, histh-cvRound(histequalizado.at<float>(i))),
               Scalar(255, 255, 255), 1, 8, 0);
    }

    histImgcinza.copyTo(icinza(Rect(0,0,nbins, histh)));
    histImgequalizado.copyTo(iequalizado(Rect(0,0,nbins, histh)));
    
    imshow("image cinza", icinza);
    imshow("image cinza euqalizada", iequalizado);
    key = waitKey(30);
    if(key == 27) break;
  }
  return 0;
}