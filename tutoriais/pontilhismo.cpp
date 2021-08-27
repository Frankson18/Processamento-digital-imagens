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

vector<int> yrange;
vector<int> xrange;

Mat image, frame, points;

int width, height, gray;
int x, y;

void on_trackbar_pontilhismo(int,void*){
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

  points = Mat(height, width, CV_8U, Scalar(255));

  random_shuffle(xrange.begin(), xrange.end());

  for(auto i : xrange){
    random_shuffle(yrange.begin(), yrange.end());
    for(auto j : yrange){
      x = i+rand()%(2*JITTER)-JITTER+1;
      y = j+rand()%(2*JITTER)-JITTER+1;
      gray = image.at<uchar>(x,y);
      circle(points,
             cv::Point(y,x),
             RAIO,
             CV_RGB(gray,gray,gray),
             -1,
             CV_AA);
    }
  }
  imshow("points", points);
}

int main(int argc, char** argv){
  
  image= imread(argv[1],CV_LOAD_IMAGE_GRAYSCALE);

  srand(time(0));

  if(!image.data){
	cout << "nao abriu" << argv[1] << endl;
    cout << argv[0] << " imagem.jpg";
    exit(0);
  }
  namedWindow("points",0);
  createTrackbar("STEP", "points",
                &STEP,
                100,
                on_trackbar_pontilhismo);

  on_trackbar_pontilhismo(STEP,0);

  createTrackbar("JITTER", "points",
                &JITTER,
                100,
                on_trackbar_pontilhismo);

  on_trackbar_pontilhismo(JITTER,0);

  createTrackbar("RAIO", "points",
                &RAIO,
                100,
                on_trackbar_pontilhismo);

  on_trackbar_pontilhismo(RAIO,0);
  
  waitKey();
  return 0;
}