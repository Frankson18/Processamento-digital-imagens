#include <iostream>
#include <opencv2/opencv.hpp>
#include <fstream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <numeric>
#include <ctime>
#include <cstdlib>

using namespace std;
using namespace cv;

int STEP = 5;
int JITTER = 3;
int RAIO = 3;

int top_slider = 10;
int top_slider_max = 200;

Mat image, points,imageGray, border;

void pontilhismo(){
  vector<int> yrange;
  vector<int> xrange;
  Vec3b color;

  int width, height;
  int x, y;
  
  width=image.size().width;
  height=image.size().height;

  xrange.resize(height/STEP);
  yrange.resize(width/STEP);

  iota(xrange.begin(), xrange.end(), 0);
  iota(yrange.begin(), yrange.end(), 0);

  for(uint i=0; i<xrange.size(); i++){
    xrange[i]= xrange[i]*STEP+STEP/2;
  }

  for(uint i=0; i<yrange.size(); i++){
    yrange[i]= yrange[i]*STEP+STEP/2;
  }

  random_shuffle(xrange.begin(), xrange.end());

  for(auto i : xrange){
    random_shuffle(yrange.begin(), yrange.end());
    for(auto j : yrange){
      if (border.at<uchar>(i, j) == 255){
        x = i+rand()%(2*JITTER)-JITTER+1;
        y = j+rand()%(2*JITTER)-JITTER+1;
        color = image.at<Vec3b>(x,y);
        circle(points,
             Point(y,x),
             RAIO,
             CV_RGB(color[2],color[1],color[0]),
             -1,
             CV_AA);
      }
      else{
        x = i+rand()%(2*JITTER)-JITTER+1;
        y = j+rand()%(2*JITTER)-JITTER+1;
        color = image.at<Vec3b>(x,y);
        circle(points,
             Point(y,x),
             2,
             CV_RGB(color[2],color[1],color[0]),
             -1,
             CV_AA);

      }
      
    }
  }
  imshow("cannypoints", points);
}

void on_trackbar_canny(int, void*){
  Canny(image, border, top_slider, 3*top_slider);
  pontilhismo();
  namedWindow("canny",1);
  imshow("canny",border);
}

void on_trackbar_cannypoints(int,void*){
  pontilhismo();
}

int main(int argc, char** argv){
  
  image= imread(argv[1],CV_LOAD_IMAGE_COLOR);

  cvtColor(image, imageGray, CV_BGR2GRAY);

  points = image.clone();
  border = imageGray.clone();

  srand(time(0));

  if(!image.data){
	cout << "nao abriu" << argv[1] << endl;
    cout << argv[0] << " imagem.jpg";
    exit(0);
  }
  namedWindow("cannypoints",0);
  createTrackbar("Threshold inferior", "cannypoints",
                &top_slider,
                top_slider_max,
                on_trackbar_canny );

  on_trackbar_canny(top_slider, 0 );
  createTrackbar("STEP", "cannypoints",
                &STEP,
                100,
                on_trackbar_cannypoints);

  on_trackbar_cannypoints(STEP,0);

  createTrackbar("JITTER", "cannypoints",
                &JITTER,
                100,
                on_trackbar_cannypoints);

  on_trackbar_cannypoints(JITTER,0);

  createTrackbar("RAIO", "cannypoints",
                &RAIO,
                100,
                on_trackbar_cannypoints);

  on_trackbar_cannypoints(RAIO,0);
  
  waitKey(0);
  return 0;
}