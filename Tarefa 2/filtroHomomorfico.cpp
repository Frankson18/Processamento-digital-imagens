#include <iostream>
#include <opencv2/opencv.hpp>
#include <vector>

using namespace std;
using namespace cv;

Mat complexImage;
Mat padded, filter;
Mat image, imagegray, magI;
Mat_<float> realInput, zeros, ones;
vector<Mat> planos;

float yl = 0, yh = 0, d0 = 0, c = 0;
float ylmax = 100, yhmax = 100, d0max = 256, cmax = 100;
int yl_slider = 1, yh_slider = 1, d0_slider = 1, c_slider = 1;

//para calculo da DFT
int dft_M, dft_N;


// troca os quadrantes da imagem da DFT
void deslocaDFT(Mat& image) {
  Mat tmp, A, B, C, D;

  // se a imagem tiver tamanho impar, recorta a regiao para
  // evitar cópias de tamanho desigual
  image = image(Rect(0, 0, image.cols & -2, image.rows & -2));
  int cx = image.cols / 2;
  int cy = image.rows / 2;

  // reorganiza os quadrantes da transformada
  // A B   ->  D C
  // C D       B A
  A = image(Rect(0, 0, cx, cy));
  B = image(Rect(cx, 0, cx, cy));
  C = image(Rect(0, cy, cx, cy));
  D = image(Rect(cx, cy, cx, cy));

  // A <-> D
  A.copyTo(tmp);
  D.copyTo(A);
  tmp.copyTo(D);

  // C <-> B
  C.copyTo(tmp);
  B.copyTo(C);
  tmp.copyTo(B);
}

void filtro(){
  Mat filter = Mat(padded.size(), CV_32FC2, Scalar(0));
  Mat tmp = Mat(dft_M, dft_N, CV_32F);
  float d2, exp, filtroH;
  for (int i = 0; i < dft_M; i++) {
      for (int j = 0; j < dft_N; j++) {
          d2 = pow(i - dft_M/2.0, 2) + pow(j - dft_N/2.0, 2);
          exp = - c*(d2/pow(d0, 2));
          filtroH = (yh - yl)*(1 - expf(exp) ) + yl;
          tmp.at<float> (i,j) = filtroH;
      }
  }

  Mat comps[] = {tmp, tmp};
  merge(comps, 2, filter);

  Mat dftClone = complexImage.clone();

  mulSpectrums(dftClone,filter,dftClone,0);

  // exibe o espectro 
  planos.clear();
  split(filter, planos);

  Mat magn, angl, anglInt, magnInt;

  magnitude(planos[0], planos[1], planos[0]);
  magI = planos[0];
  magI += Scalar::all(1);
  log(magI, magI);
  normalize(magI, magI, 0, 255, NORM_MINMAX);
  magI.convertTo(magnInt, CV_8U);
  imshow("Espectro", magnInt);

  deslocaDFT(dftClone);

  idft(dftClone, dftClone);

  split (dftClone, planos);

  normalize(planos[0], planos[0], 0, 1, CV_MINMAX);

  imshow("Filtro Homomorfico", planos[0]);
}


void on_trackbar_yl(int, void*){
    yl = (float) yl_slider;
    yl = ylmax*yl/100.0;
    filtro();
}

void on_trackbar_d0(int, void*){
    d0 = d0_slider*d0max/100.0;
    filtro();
}

void on_trackbar_yh(int, void*) {
    yh = yh_slider*yhmax/100.0;
    filtro();
}
void on_trackbar_c(int, void*) {
    c = c_slider*cmax / 100.0;
    filtro();
}


int main() {

  VideoCapture cap;

  // guarda tecla capturada
  char key;

  // abre a câmera
  cap.open(0);
  if (!cap.isOpened()){
    return -1;
  } 

  cap.set(CAP_PROP_FRAME_WIDTH, 640);
  cap.set(CAP_PROP_FRAME_HEIGHT, 480);

  // captura uma imagem para recuperar as
  // informacoes de gravação
  cap >> image;

  // identifica os tamanhos otimos para
  // calculo do FFT
  dft_M = getOptimalDFTSize(image.rows);
  dft_N = getOptimalDFTSize(image.cols);

  // realiza o padding da imagem
  copyMakeBorder(image, padded, 0, dft_M - image.rows, 0,
                     dft_N - image.cols,BORDER_CONSTANT,
                     Scalar::all(0));

  // parte imaginaria da matriz complexa (preenchida com zeros)
  zeros = Mat_<float>::zeros(padded.size());
  ones = Mat_<float>::zeros(padded.size());

  // prepara a matriz complexa para ser preenchida
  complexImage = Mat(padded.size(), CV_32FC2,Scalar(0));

  // a função de transferencia (filtro de frequencia) deve ter o
  // mesmo tamanho e tipo da matriz complexa
  filter = complexImage.clone();

  namedWindow("original", 0);
  namedWindow("Filtro Homomorfico", 1);

  /* Cria as barras de rolagem com todos os parâmetros. */
    createTrackbar( "YH", "Filtro Homomorfico",
                  &yh_slider,
                  100.0,
                  on_trackbar_yh);
    on_trackbar_yh(yh_slider,0);

    createTrackbar( "YL", "Filtro Homomorfico",
                  &yl_slider,
                  100.0,
                  on_trackbar_yl);
    on_trackbar_yl(yl_slider,0);

    createTrackbar( "D0", "Filtro Homomorfico",
                  &d0_slider,
                  100.0,
                  on_trackbar_d0 );
    on_trackbar_d0(d0_slider,0);

    createTrackbar( "C", "Filtro Homomorfico",
                  &c_slider,
                  100.0,
                  on_trackbar_c);
    on_trackbar_c(c_slider,0);

  for(;;){
    //namedWindow("original", 0);
    cap >> image;  // get a new frame from camera
    cvtColor(image, imagegray, COLOR_BGR2GRAY);
    flip(imagegray, imagegray, 1);
    imshow("original", imagegray);

    // realiza o padding da imagem
    copyMakeBorder(imagegray, padded, 0, dft_M - image.rows, 0,
                       dft_N - image.cols, BORDER_CONSTANT,
                       Scalar::all(0));

    // limpa o array de matrizes que vao compor a
    // imagem complexa
    planos.clear();
    // cria a compoente real
    realInput = Mat_<float>(padded);
    // insere as duas componentes no array de matrizes
    planos.push_back(realInput);
    planos.push_back(zeros);

    // combina o array de matrizes em uma unica
    // componente complexa
    merge(planos, complexImage);

    // calcula o dft
    dft(complexImage, complexImage);
    // realiza a troca de quadrantes
    deslocaDFT(complexImage);

    filtro();

    key = (char) waitKey(10);
    if (key == 27) break;

  }
 
  return 0;
}

