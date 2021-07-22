#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main(int argc, char **argv)
{
  Mat image;
  VideoCapture cap;
  vector<Mat> rgb;
  Mat histR, histanterior;
  int nbins = 64;
  float range[] = {0, 255};
  const float *histrange = {range};
  bool uniform = true;
  bool acummulate = false;
  int key;

  cap.open(0);

  if (!cap.isOpened())
  {
    std::cout << "cameras indisponiveis";
    return -1;
  }

  cap.set(CAP_PROP_FRAME_WIDTH, 640);
  cap.set(CAP_PROP_FRAME_HEIGHT, 480);

  int histw = nbins, histh = nbins / 2;
  Mat histImgR(histh, histw, CV_8UC3, Scalar(0,0,0));

  cap >> image;

  split(image, rgb);

  //calculando histograma imagem do canal vermelho
  calcHist(&rgb[0], 1, 0, Mat(), histR, 1,
           &nbins, &histrange,
           uniform, acummulate);

  //normalizando
  normalize(histR, histR, 0, histImgR.rows, NORM_MINMAX, -1, Mat());

  while (1)
  {
    histR.copyTo(histanterior);

    cap >> image;

    split(image, rgb);

    //calculando histograma imagem do canal vermelho
    calcHist(&rgb[0], 1, 0, Mat(), histR, 1,
             &nbins, &histrange,
             uniform, acummulate);

    //normalizando
    normalize(histR, histR, 0, histImgR.rows, NORM_MINMAX, -1, Mat());

    histImgR.setTo(Scalar(0));

    double correlacao = compareHist(histR, histanterior,CV_COMP_CORREL);

    if(correlacao < 0.99){
      circle(image, Point(image.cols - 50, image.rows-50), 20, Scalar(0, 255, 0), CV_FILLED);
    }

    for(int i=0; i<nbins; i++){
      line(histImgR,
               Point(i, histh),
               Point(i, histh-cvRound(histR.at<float>(i))),
               Scalar(255, 255, 255), 1, 8, 0);
    }
    histImgR.copyTo(image(Rect(0,0,nbins, histh)));

    imshow("imagem", image);
    key = waitKey(30);
    if (key == 27)
      break;
  }
  return 0;
}