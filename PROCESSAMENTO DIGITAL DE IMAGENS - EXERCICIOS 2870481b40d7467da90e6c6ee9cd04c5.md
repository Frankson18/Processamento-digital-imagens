# PROCESSAMENTO DIGITAL DE IMAGENS - EXERCICIOS

# INTRODUÇÃO

Resolução de exercícios para a matéria de Processamento Digital de Imagens da UFRN. Utilizando a biblioteca `OpenCV` na linguagem `C++`.

# PRIMEIRA UNIDADE

# MANIPULANDO PIXEIS EM UMA IMAGEM

### EXERCÍCIO 1.1

Utilizando o programa [exemplos/pixels.cpp](https://agostinhobritojr.github.io/tutorial/pdi/exemplos/pixels.cpp) como referência, implemente um programa `regions.cpp`. Esse programa deverá solicitar ao usuário as coordenadas de dois pontos P1 e P2 localizados dentro dos limites do tamanho da imagem e exibir que lhe for fornecida. Entretanto, a região definida pelo retângulo de vértices opostos definidos pelos pontos P1 e P2 será exibida com o negativo da imagem na região correspondente. 

---

### SOLUÇÃO

Para resolver foi preciso implementar entradas para o usuário escolher qual região ele queria deixar em negativo. Para deixar deixar a região em negativo foi feito um `for` para percorrer área escolhida e fazer a operação `image.at<uchar>(i,j)=255 - image.at<uchar>(i,j);` que faz com que o pixel da imagem se torne negativo. 

![PROCESSAMENTO%20DIGITAL%20DE%20IMAGENS%20-%20EXERCICIOS%202870481b40d7467da90e6c6ee9cd04c5/Untitled.png](PROCESSAMENTO%20DIGITAL%20DE%20IMAGENS%20-%20EXERCICIOS%202870481b40d7467da90e6c6ee9cd04c5/Untitled.png)

![PROCESSAMENTO%20DIGITAL%20DE%20IMAGENS%20-%20EXERCICIOS%202870481b40d7467da90e6c6ee9cd04c5/Untitled%201.png](PROCESSAMENTO%20DIGITAL%20DE%20IMAGENS%20-%20EXERCICIOS%202870481b40d7467da90e6c6ee9cd04c5/Untitled%201.png)

```cpp
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
```

### EXERCÍCIO 1.2

Utilizando o programa [exemplos/pixels.cpp](https://agostinhobritojr.github.io/tutorial/pdi/exemplos/pixels.cpp) como referência, implemente um programa `trocaregioes.cpp`. Seu programa deverá trocar os quadrantes em diagonal na imagem. Explore o uso da classe Mat e seus construtores para criar as regiões que serão trocadas.

---

### SOLUÇÃO

Diferente do regions, esse agora não precisa de interação com o usuário, é simplesmente manipulação da imagem. E para fazer isso foi usado a classe `Mat` e as funções `Rect` para pegar pedaços da imagem e salva-los em uma matriz e o método `copyTo` para fazer a modificação na imagem atual, assim conseguindo modificar as áreas da imagem.

![PROCESSAMENTO%20DIGITAL%20DE%20IMAGENS%20-%20EXERCICIOS%202870481b40d7467da90e6c6ee9cd04c5/Untitled.png](PROCESSAMENTO%20DIGITAL%20DE%20IMAGENS%20-%20EXERCICIOS%202870481b40d7467da90e6c6ee9cd04c5/Untitled.png)

![PROCESSAMENTO%20DIGITAL%20DE%20IMAGENS%20-%20EXERCICIOS%202870481b40d7467da90e6c6ee9cd04c5/Untitled%202.png](PROCESSAMENTO%20DIGITAL%20DE%20IMAGENS%20-%20EXERCICIOS%202870481b40d7467da90e6c6ee9cd04c5/Untitled%202.png)

```cpp
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
```

---

# PREENCHENDO REGIÕES

### EXERCÍCIO 2.1

Observando-se o programa [labeling.cpp](https://agostinhobritojr.github.io/tutorial/pdi/exemplos/labeling.cpp) como exemplo, é possível verificar que caso existam mais de 255 objetos na cena, o processo de rotulação poderá ficar comprometido. Identifique a situação em que isso ocorre e proponha uma solução para este problema.

### SOLUÇÃO

Para resolver o problema de casos que a imagem tenha mais que 255 objetos a serem rotulados, podemos usar uma estrategia de fazer o rotulo ser decimal, ou rotula usando a operação mod de 255. 

### EXERCÍCIO 2.2

Aprimore o algoritmo de contagem apresentado para identificar regiões com ou sem buracos internos que existam na cena. Assuma que objetos com mais de um buraco podem existir. Inclua suporte no seu algoritmo para não contar bolhas que tocam as bordas da imagem. Não se pode presumir, a priori, que elas tenham buracos ou não.

### SOLUÇÃO

Para retirar as bolhas e buracos que estão nas bordas basta percorrer as bordas da imagem e quando acha um pixel com a cor do objeto usar o ponto como semente no `floodFill` para fazer com que a bolhas tenha a mesma tonalidade do fundo da imagem. Já para conta os buracos, usei uma estrategia de pinta o fundo da imagem de cinza usando o `floodFill` assim, a parte de dentro dos buracos ainda ficaria com a cor do fundo original e eu poderia conta agora quantos buracos tem. Sabendo a quantidade de buracos é só aplicar o `floodFill` na imagem, ver quantos objetos ele encontrou e diminuir do numero de buracos, assim nos temos a quantidade de bolhas e buracos. 

![PROCESSAMENTO%20DIGITAL%20DE%20IMAGENS%20-%20EXERCICIOS%202870481b40d7467da90e6c6ee9cd04c5/Untitled%203.png](PROCESSAMENTO%20DIGITAL%20DE%20IMAGENS%20-%20EXERCICIOS%202870481b40d7467da90e6c6ee9cd04c5/Untitled%203.png)

![PROCESSAMENTO%20DIGITAL%20DE%20IMAGENS%20-%20EXERCICIOS%202870481b40d7467da90e6c6ee9cd04c5/Untitled%204.png](PROCESSAMENTO%20DIGITAL%20DE%20IMAGENS%20-%20EXERCICIOS%202870481b40d7467da90e6c6ee9cd04c5/Untitled%204.png)

![PROCESSAMENTO%20DIGITAL%20DE%20IMAGENS%20-%20EXERCICIOS%202870481b40d7467da90e6c6ee9cd04c5/Untitled%205.png](PROCESSAMENTO%20DIGITAL%20DE%20IMAGENS%20-%20EXERCICIOS%202870481b40d7467da90e6c6ee9cd04c5/Untitled%205.png)

![PROCESSAMENTO%20DIGITAL%20DE%20IMAGENS%20-%20EXERCICIOS%202870481b40d7467da90e6c6ee9cd04c5/Untitled%206.png](PROCESSAMENTO%20DIGITAL%20DE%20IMAGENS%20-%20EXERCICIOS%202870481b40d7467da90e6c6ee9cd04c5/Untitled%206.png)

```cpp
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
```

---

# MANIPULAÇÃO DE HISTOGRAMAS

### EXERCÍCO 3.1

Utilizando o programa [exemplos/histogram.cpp](https://agostinhobritojr.github.io/tutorial/pdi/exemplos/histogram.cpp) como referência, implemente um programa `equalize.cpp`. Este deverá, para cada imagem capturada, realizar a equalização do histogram antes de exibir a imagem. Teste sua implementação apontando a câmera para ambientes com iluminações variadas e observando o efeito gerado. Assuma que as imagens processadas serão em tons de cinza. 

### SOLUÇÃO

Para simular uma entrada em tons de cinza tive que para cinza usando a função `cvtColor`. Para fazer a equalização do histograma utiizei a função `equalizeHist`, logo depois fiz propriamente dito o hitrograma da imagem original e da equalizada, assim tendo uma comparação entre as duas. 

![PROCESSAMENTO%20DIGITAL%20DE%20IMAGENS%20-%20EXERCICIOS%202870481b40d7467da90e6c6ee9cd04c5/ezgif.com-gif-maker.gif](PROCESSAMENTO%20DIGITAL%20DE%20IMAGENS%20-%20EXERCICIOS%202870481b40d7467da90e6c6ee9cd04c5/ezgif.com-gif-maker.gif)

```cpp
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
```

### EXERCÍCIO 3.2

Utilizando o programa [exemplos/histogram.cpp](https://agostinhobritojr.github.io/tutorial/pdi/exemplos/histogram.cpp) como referência, implemente um programa `motiondetector.cpp`. Este deverá continuamente calcular o histograma da imagem (apenas uma componente de cor é suficiente) e compará-lo com o último histograma calculado. Quando a diferença entre estes ultrapassar um limiar pré-estabelecido, ative um alarme. Utilize uma função de comparação que julgar conveniente. 

### SOLUÇÃO

Para solucionar esse exercício tiver que criar um histograma que ficasse sempre salvando o ultimo histograma do ultimo frame e comparando com o histograma mais recente. Para fazer a comparação dos histogramas utilizei a função `compareHis` que me devolve a correlação entre os histogramas, assim consigo criar um `if` e verificar se esse correlação é alta ou baixar e criar um alerta em verde no canto inferior da tela, avisando que teve movimento.

![PROCESSAMENTO%20DIGITAL%20DE%20IMAGENS%20-%20EXERCICIOS%202870481b40d7467da90e6c6ee9cd04c5/ezgif.com-gif-maker_(1).gif](PROCESSAMENTO%20DIGITAL%20DE%20IMAGENS%20-%20EXERCICIOS%202870481b40d7467da90e6c6ee9cd04c5/ezgif.com-gif-maker_(1).gif)

```cpp
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
```

---

### EXERCÍCIO 4

Utilizando o programa [exemplos/filtroespacial.cpp](https://agostinhobritojr.github.io/tutorial/pdi/exemplos/filtroespacial.cpp) como referência, implemente um programa `laplgauss.cpp`. O programa deverá acrescentar mais uma funcionalidade ao exemplo fornecido, permitindo que seja calculado o laplaciano do gaussiano das imagens capturadas. Compare o resultado desse filtro com a simples aplicação do filtro laplaciano.

### SOLUÇÃO

Para solucionar foi simples, foi somente adicionar a mascara do laplaciano do gaussiano junto as mascaras dos outros filtro e colocar a opção de escolher digitando a tecla `p`. Analisando o filtro laplaciano com o laplaciano do gaussiano percebi-se uma acentuação dos contornos, deixando a linda mais espessa e também mais contornos visíveis. No vídeo foi primeiro aplicado o filtro laplaciano e depois o filtro laplaciano do gaussiano.

![PROCESSAMENTO%20DIGITAL%20DE%20IMAGENS%20-%20EXERCICIOS%202870481b40d7467da90e6c6ee9cd04c5/ezgif.com-gif-maker%201.gif](PROCESSAMENTO%20DIGITAL%20DE%20IMAGENS%20-%20EXERCICIOS%202870481b40d7467da90e6c6ee9cd04c5/ezgif.com-gif-maker%201.gif)

```cpp
#include <iostream>
#include <opencv2/opencv.hpp>

void printmask(cv::Mat &m) {
  for (int i = 0; i < m.size().height; i++) {
    for (int j = 0; j < m.size().width; j++) {
      std::cout << m.at<float>(i, j) << ",";
    }
    std::cout << "\n";
  }
}

int main(int, char **) {
  cv::VideoCapture cap;  // open the default camera
  float media[] = {0.1111, 0.1111, 0.1111, 0.1111, 0.1111,
                   0.1111, 0.1111, 0.1111, 0.1111};
  float gauss[] = {0.0625, 0.125,  0.0625, 0.125, 0.25,
                   0.125,  0.0625, 0.125,  0.0625};
  float horizontal[] = {-1, 0, 1, -2, 0, 2, -1, 0, 1};
  float vertical[] = {-1, -2, -1, 0, 0, 0, 1, 2, 1};
  float laplacian[] = {0, -1, 0, -1, 4, -1, 0, -1, 0};
  float boost[] = {0, -1, 0, -1, 5.2, -1, 0, -1, 0};
  float laplgauss [] ={0,0,-1,0,0,0,-1,-2,-1,0,-1,-2,16,-2,-1,
                      0,-1,-2,-1,0,0,0,-1,0,0};

  cv::Mat frame, framegray, frame32f, frameFiltered;
  cv::Mat mask(3, 3, CV_32F), mask_scale;
  cv::Mat result;
  double width, height;
  int absolut;
  char key;

  cap.open(0);

  if (!cap.isOpened())  // check if we succeeded
    return -1;

  cap.set(cv::CAP_PROP_FRAME_WIDTH, 640);
  cap.set(cv::CAP_PROP_FRAME_HEIGHT, 480);
  width = cap.get(cv::CAP_PROP_FRAME_WIDTH);
  height = cap.get(cv::CAP_PROP_FRAME_HEIGHT);
  std::cout << "largura=" << width << "\n";
  ;
  std::cout << "altura =" << height << "\n";
  ;
  std::cout << "fps    =" << cap.get(cv::CAP_PROP_FPS) << "\n";
  std::cout << "format =" << cap.get(cv::CAP_PROP_FORMAT) << "\n";

  cv::namedWindow("filtroespacial", cv::WINDOW_NORMAL);
  cv::namedWindow("original", cv::WINDOW_NORMAL);

  mask = cv::Mat(3, 3, CV_32F, media);
  absolut = 1;  // calcs abs of the image

  for (;;) {
    cap >> frame;  // get a new frame from camera
    cv::cvtColor(frame, framegray, cv::COLOR_BGR2GRAY);
    cv::flip(framegray, framegray, 1);
    cv::imshow("original", framegray);
    framegray.convertTo(frame32f, CV_32F);
    cv::filter2D(frame32f, frameFiltered, frame32f.depth(), mask,
                 cv::Point(1, 1), 0);
    if (absolut) {
      frameFiltered = cv::abs(frameFiltered);
    }

    frameFiltered.convertTo(result, CV_8U);

    cv::imshow("filtroespacial", result);

    key = (char)cv::waitKey(10);
    if (key == 27) break;  // esc pressed!
    switch (key) {
      case 'a':
        absolut = !absolut;
        break;
      case 'm':
        mask = cv::Mat(3, 3, CV_32F, media);
        printmask(mask);
        break;
      case 'g':
        mask = cv::Mat(3, 3, CV_32F, gauss);
        printmask(mask);
        break;
      case 'h':
        mask = cv::Mat(3, 3, CV_32F, horizontal);
        printmask(mask);
        break;
      case 'v':
        mask = cv::Mat(3, 3, CV_32F, vertical);
        printmask(mask);
        break;
      case 'l':
        mask = cv::Mat(3, 3, CV_32F, laplacian);
        printmask(mask);
        break;
        case 'p':
        mask = cv::Mat(5, 5, CV_32F, laplgauss);
        printmask(mask);
        break;
      case 'b':
        mask = cv::Mat(3, 3, CV_32F, boost);
        break;
      default:
        break;
    }
  }
  return 0;
}
```

---

# SEGUNDA UNIDADE

# FILTRAGEM NO DOMÍNIO DA FREQUÊNCIA

### EXERCÍCIO 7

Utilizando o programa [exemplos/dft.cpp](https://agostinhobritojr.github.io/tutorial/pdi/exemplos/dft.cpp) como referência, implemente o filtro homomórfico para melhorar imagens com iluminação irregular. Crie uma cena mal iluminada e ajuste os parâmetros do filtro homomórfico para corrigir a iluminação da melhor forma possível. Assuma que a imagem fornecida é em tons de cinza.

### SOLUÇÃO

Usando o programa `dft.cpp` como exemplo criei uma função `filtro` para implementar o filtro homomórfico que se da com essa equação:

![Untitled](PROCESSAMENTO%20DIGITAL%20DE%20IMAGENS%20-%20EXERCICIOS%202870481b40d7467da90e6c6ee9cd04c5/Untitled%207.png)

Ainda dentro da função do filtro, utilizei o código de plotar o espectro do exemplo para plota o espectro do filtro e assim poder ajusta-lo melhor baseado em seu espectro. 

O filtro precisa que seus parâmetros sejam ajustados conforme a cena, sendo assim criei, barras deslizantes com seus parâmetros: `yl` , `yh` , `d0` e `c` .

Aproveitei também do código exemplo a função `deslocaDFT` para poder trabalhar melhor com o filtro e o espectro. 

![ezgif.com-gif-maker(1).gif](PROCESSAMENTO%20DIGITAL%20DE%20IMAGENS%20-%20EXERCICIOS%202870481b40d7467da90e6c6ee9cd04c5/ezgif.com-gif-maker(1).gif)

```cpp
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
```

---

# DETECÇÃO DE BORDAS COM O ALGORITMO DE CANNY E PONTILHISMO

### EXERCÍCIO 8

Utilizando os programas [exemplos/canny.cpp](https://agostinhobritojr.github.io/tutorial/pdi/exemplos/canny.cpp) e [exemplos/pontilhismo.cpp](https://agostinhobritojr.github.io/tutorial/pdi/exemplos/pontilhismo.cpp) como referência, implemente um programa `cannypoints.cpp`. A ideia é usar as bordas produzidas pelo algoritmo de Canny para melhorar a qualidade da imagem pontilhista gerada. A forma como a informação de borda será usada é livre. Entretanto, são apresentadas algumas sugestões de técnicas que poderiam ser utilizadas:

- Desenhar pontos grandes na imagem pontilhista básica;
- Usar a posição dos pixels de borda encontrados pelo algoritmo de Canny para desenhar pontos nos respectivos locais na imagem gerada.
- Experimente ir aumentando os limiares do algoritmo de Canny e, para cada novo par de limiares, desenhar círculos cada vez menores nas posições encontradas.
- Escolha uma imagem de seu gosto e aplique a técnica que você
desenvolveu.
- Descreva no seu relatório detalhes do procedimento usado para criar
sua técnica pontilhista.

### SOLUÇÃO

Utilizando os códigos dos exemplos juntei as duas funcionalidades. Para isso peguei os parâmetros fixos do código do pontilhismo transformei em inteiros para poder fazer deles barras deslizantes. 

Após isso peguei o código para formação do pontilhismo e criei uma função `pontilhismo` e dentro dela modifiquei o `for` para só aplicar o pontilhismo desejado quando o pixel comparado com o pixel da matriz  `border` gerada pela função `canny` fosse `= 255` , e se não fosse eu ainda aplicaria o pontilhismo mais com o `raio` fixo em `2`. Para a função canny eu utilizo exatamente o código exemplo.

![ezgif.com-gif-maker.gif](PROCESSAMENTO%20DIGITAL%20DE%20IMAGENS%20-%20EXERCICIOS%202870481b40d7467da90e6c6ee9cd04c5/ezgif.com-gif-maker%202.gif)

```cpp
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
```

---

# QUANTIZAÇÃO VETORIAL COM K-MEANS

### EXERCÍCIO 9

Utilizando o programa [kmeans.cpp](https://agostinhobritojr.github.io/tutorial/pdi/exemplos/kmeans.cpp) como exemplo prepare um programa exemplo onde a execução do código se dê usando o parâmetro `nRodadas=1` e inciar os centros de forma aleatória usando o parâmetro `KMEANS_RANDOM_CENTER` ao invés de `KMEANS_PP_CENTERS`. Realize 10 rodadas diferentes do algoritmo e compare as imagens produzidas. Explique porque elas podem diferir tanto.

### SOLUÇÃO

Utilizando o exemplo modifiquei o parâmetro `KMEANS_PP_CENTERS` para o `KMEANS_RANDOM_CENTERS`     e mudei o numero de rodadas para 1. Logo apos isso para fazer as 10 rodadas diferentes coloquei um `for` englobando todo o código e a cada rodada salvar a foto gerada e por fim mostro a ultima gerada na tela. 

Para demostra o resultado transformei 10 fotos em um `gif` para fica mais dinâmico. O motivo da mudança entra uma rodada e outra é por causa da mudança do parâmetro `KMEANS_PP_CENTERS` que faz com que os centros gerados inicialmente sejam aleatórios. A variável `nRodadas` também pode interferir nesse resultado pois o algoritmo é feito uma unica vez e é pego o primeiro resultado 

![comida.jpg](PROCESSAMENTO%20DIGITAL%20DE%20IMAGENS%20-%20EXERCICIOS%202870481b40d7467da90e6c6ee9cd04c5/comida.jpg)

I**magem original** 

![5ky89f.gif](PROCESSAMENTO%20DIGITAL%20DE%20IMAGENS%20-%20EXERCICIOS%202870481b40d7467da90e6c6ee9cd04c5/5ky89f.gif)

**Gif do resultado do K-means**

```cpp
#include <opencv2/opencv.hpp>
#include <cstdlib>

using namespace cv;

int main( int argc, char** argv ){
  int nClusters = 8;
  Mat rotulos;
  int nRodadas = 1;
  Mat centros;
  char name[15];
  Mat img = imread( argv[1], CV_LOAD_IMAGE_COLOR);
  Mat samples(img.rows * img.cols, 3, CV_32F);
  Mat rotulada( img.size(), img.type() );

  for (int i=0; i<10; i++){
  
    for( int y = 0; y < img.rows; y++ ){
      for( int x = 0; x < img.cols; x++ ){
        for( int z = 0; z < 3; z++){
          samples.at<float>(y + x*img.rows, z) = img.at<Vec3b>(y,x)[z];
	      }
	    }
    }

    kmeans(samples,
		  nClusters,
		  rotulos,
		  TermCriteria(CV_TERMCRIT_ITER|CV_TERMCRIT_EPS, 10000, 0.0001),
		  nRodadas,
		  KMEANS_RANDOM_CENTERS,
		  centros);

    for( int y = 0; y < img.rows; y++ ){
      for( int x = 0; x < img.cols; x++ ){
	      int indice = rotulos.at<int>(y + x*img.rows,0);
	      rotulada.at<Vec3b>(y,x)[0] = (uchar) centros.at<float>(indice, 0);
	      rotulada.at<Vec3b>(y,x)[1] = (uchar) centros.at<float>(indice, 1);
	      rotulada.at<Vec3b>(y,x)[2] = (uchar) centros.at<float>(indice, 2);
	    }
    }
    sprintf(name,"k-imagem%d.jpg",i);
    imwrite(name, rotulada);
  }
    imshow( "clustered image", rotulada);
    waitKey( 0 );
}
```