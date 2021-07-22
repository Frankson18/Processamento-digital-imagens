#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main(int argc, char **argv)
{
  Mat image, realce;
  int width, height;
  int nobjects;

  Point p;
  image = imread(argv[1], IMREAD_GRAYSCALE);

  if (!image.data)
  {
    cout << "imagem nao carregou corretamente\n";
    return (-1);
  }

  width = image.cols;
  height = image.rows;

  p.x = 0;
  p.y = 0;

  //excluindo objetos que tocam nas bordas superiores e inferiores
  for (int i = 0; i < height; i++)
  {
    if (image.at<uchar>(0,i) == 255)
    {
      p.x = i;
      p.y = 0;
      floodFill(image, p,0);
    }
    if (image.at<uchar>(width-1,i) == 255)
    {
      p.x = i;
      p.y = width-1;
      floodFill(image, p,0);
    }
  }

  //excluindo objetos que tocam nas bordas laterais
  for (int i = 0; i < width; i++)
  {
    if (image.at<uchar>(i,0) == 255)
    {
      p.x = 0;
      p.y = i;
      floodFill(image, p,0);
    }
    if (image.at<uchar>(i,height-1) == 255)
    {
      p.x = height-1;
      p.y = i;
      floodFill(image, p,0);
    }
  }

  imshow("objetos da bordas removidos", image);

  //mudando a cor do backgrpund para pode identificar os buraco
  p.x=0;
  p.y=0;
  floodFill(image,p,100);
  imshow("bolhas", image);

  // contando bolhas
  int buracos = 0;
  for (int i = 0; i < height; i++)
  {
    for (int j = 0; j < width; j++)
    {
      if (image.at<uchar>(i, j) == 0)
      {
        // achou um objeto
        buracos++;
        p.x = j;
        p.y = i;
        // preenche o objeto com o contador
        floodFill(image, p, 255);
      }
    }
  }
  cout<<"numero de buracos: "<<buracos<<endl;

  //rotulando todos os objetos
  nobjects = 0;
  for (int i = 0; i < height; i++)
  {
    for (int j = 0; j < width; j++)
    {
      if (image.at<uchar>(i, j) == 255)
      {
        // achou um objeto
        nobjects++;
        p.x = j;
        p.y = i;
        // preenche o objeto com o contador
        floodFill(image, p, nobjects);
      }
    }
  }

  cout<<"numero de bolhas: "<<nobjects-buracos<<endl;

  imshow("labeling", image);
  imwrite("labeling.png", image);
  waitKey();
  return 0;
}