/*
 * Fichier source pour le projet d'unité
 *  INF-4101C
 *---------------------------------------------------------------------------------------------------
 * Pour compiler : g++ `pkg-config --cflags opencv` projet_base_imc.cpp `pkg-config --libs opencv` -o projet
 *---------------------------------------------------------------------------------------------------
 * auteur : Eva Dokladalova 09/2015
 * modification : Eva Dokladalova 10/2017
 * modificaiton : Eva Dokladalova 10/2018
 */


/* 
 * Libraries stantards 
 *
 */ 
#include <stdio.h>
#include <stdlib.h>

/* 
 * Libraries OpenCV "obligatoires" 
 *
 */ 
#include "highgui.h"
#include "cv.h"
#include "opencv2/opencv.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
  
/* 
 * Définition des "namespace" pour évite cv::, std::, ou autre
 *
 */  
using namespace std;
using namespace cv;
using std::cout;

/*
 * Some usefull defines
 * (comment if not used)
 */
#define PROFILE
#define VAR_KERNEL
#define N_ITER 100

#ifdef PROFILE
#include <time.h>
#include <sys/time.h>
#endif
int xGradient(Mat image, int x, int y)
{
	return image.at<uchar>(y - 1, x - 1) +
		2 * image.at<uchar>(y, x - 1) +
		image.at<uchar>(y + 1, x - 1) -
		image.at<uchar>(y - 1, x + 1) -
		2 * image.at<uchar>(y, x + 1) -
		image.at<uchar>(y + 1, x + 1);
}

// Computes the y component of the gradient vector
// at a given point in a image
// returns gradient in the y direction

int yGradient(Mat image, int x, int y)
{
	return image.at<uchar>(y - 1, x - 1) +
		2 * image.at<uchar>(y - 1, x) +
		image.at<uchar>(y - 1, x + 1) -
		image.at<uchar>(y + 1, x - 1) -
		2 * image.at<uchar>(y + 1, x) -
		image.at<uchar>(y + 1, x + 1);
}

/*
 *
 *--------------- MAIN FUNCTION ---------------
 *
 */
int main () {
//----------------------------------------------
// Video acquisition - opening
//----------------------------------------------
  VideoCapture cap(0); // le numéro 0 indique le point d'accès à la caméra 0 => /dev/video0
  if(!cap.isOpened()){
    cout << "Error"; return -1;
  }

//----------------------------------------------
// Déclaration des variables - imagesize
// Mat - structure contenant l'image 2D niveau de gris
// Mat3b - structure contenant l'image 2D en couleur (trois cannaux)
// Some examples of Mat copy :
//      Mat F = A.clone();
//      Mat G;
//      A.copyTo(G);
// Some usefum examples :
//      int ni = G.cols; // columns number
//      int nj = G.rows; // rows number
//      int pixel =  A.at<int>(i, j)); // access to one element of a matrix 
  
  Mat frame; // couleur
  Mat frame1; // niveau de gris 
  Mat frame_gray; // niveau de gris 
  Mat grad_x;
  Mat grad_y;
  Mat abs_grad_y;
  Mat abs_grad_x;
  Mat grad;
  Mat dst;
// variable contenant les paramètres des images ou d'éxécution  
  int ddepth = CV_16S;
  int scale = 1;
  int delta = 0;	
  unsigned char key = '0';
  int f = 0;

 #define PROFILE
  
#ifdef PROFILE
// profiling / instrumentation libraries
#include <time.h>
#include <sys/time.h>
#endif
  
//----------------------------------------------------
// Création des fenêtres pour affichage des résultats
// vous pouvez ne pas les utiliser ou ajouter selon ces exemple
// 
  cvNamedWindow("Video input", WINDOW_AUTOSIZE);
  cvNamedWindow("Video gray levels", WINDOW_AUTOSIZE);
  cvNamedWindow("Video Mediane", WINDOW_AUTOSIZE);
  cvNamedWindow("Video Edge detection", WINDOW_AUTOSIZE);
// placement arbitraire des  fenêtre sur écran 
// sinon les fenêtres sont superposée l'une sur l'autre
  cvMoveWindow("Video input", 10, 30);
  cvMoveWindow("Video gray levels", 800, 30);
  cvMoveWindow("Video Mediane", 10, 500);
  cvMoveWindow("Video Edge detection", 800, 500);
  
   FILE * fp;
  // --------------------------------------------------
  // boucle infinie pour traiter la séquence vidéo  
  //
  while(key!='q')
{
    //	  
    // acquisition d'une trame video - librairie OpenCV
    cap.read(frame);
    //conversion en niveau de gris - librairie OpenCV
    cvtColor(frame, frame_gray, CV_BGR2GRAY);    
    // image smoothing by median blur
    // 
    int k = 5;   
    medianBlur(frame_gray, frame1, k);
    
     // ------------------------------------------------
#ifdef PROFILE
 struct timeval start, end;
     gettimeofday(&start, NULL);
#endif  
     // ----------------REPLACE START------------------------------
  
	
// calcul du gradient- librairie OpenCV
     /// Gradient Y
     Sobel( frame1, grad_x, ddepth, 1, 0, 3, scale, delta, BORDER_DEFAULT );
     /// absolute value
     convertScaleAbs( grad_x, abs_grad_x );
     /// Gradient Y
    Sobel( frame1, grad_y, ddepth, 0, 1, 3, scale, delta, BORDER_DEFAULT );
    /// absolute value
    convertScaleAbs( grad_y, abs_grad_y );
    /// Total Gradient (approximate)
    addWeighted( abs_grad_x, 0.5, abs_grad_y, 0.5, 0, grad );
    // ----------------REPLACE END--------------------------------
#ifdef PROFILE
    gettimeofday(&end, NULL);
    double e = ((double) end.tv_sec * 1000000.0 + (double) end.tv_usec);
    double s = ((double) start.tv_sec * 1000000.0 + (double) start.tv_usec);
    printf("frame %d : sobel exec time : %lf us\n", f, (e - s));
   
    fp = fopen ("Provided_V.txt", "a");
    fprintf(fp, "frame %d : sobel exec time : %lf us\n", f, (e - s));
#endif
    
 // -------------------------------------------------
 // visualisation
 // taille d'image réduite pour meuilleure disposition sur écran
 
    imshow("Video input",frame);
   // imshow("Video gray levels",frame_gray);
   // imshow("Video Mediane",frame1);    
    //imshow("Video Edge detection",dst);  
    
    f++;
    key=waitKey(5);
  }
 fclose(fp);

}

    
